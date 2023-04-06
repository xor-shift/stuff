#pragma once

namespace stf {

template<typename T, usize N, typename Allocator>
channel<T, N, Allocator>::channel(allocator_type const& allocator)
    : m_allocator(allocator)
    , m_storage(N == 0 ? nullptr : m_allocator.allocate(N)) {}

template<typename T, usize N, typename Allocator>
channel<T, N, Allocator>::~channel() noexcept {
    close();

    if (m_storage != nullptr) {
        m_allocator.deallocate(m_storage, N);
        m_storage = nullptr;
    }
}

template<typename T, usize N, typename Allocator>
constexpr auto channel<T, N, Allocator>::is_buffered() const -> bool {
#ifndef NDEBUG
    if constexpr (N == 0) {
        assume(m_storage == nullptr);
    }
#endif
    return N != 0 || m_storage != nullptr;
}

template<typename T, usize N, typename Allocator>
constexpr void channel<T, N, Allocator>::close() {
    std::unique_lock lock{m_mutex};

    if (have_receiver()) {
        std::unique_lock sync_lock{m_receive_sync->recv_mutex};
        m_receive_sync->recv_fulfilled = true;
        *m_receiver_buffer = std::nullopt;
        m_receive_sync->recv_cv.notify_all();
        detach_receiver_impl();
    }

    m_closed = true;
}

template<typename T, usize N, typename Allocator>
template<typename... Args>
constexpr auto channel<T, N, Allocator>::emplace_back(Args&&... args) -> bool {
    std::unique_lock lock{m_mutex};

    if (closed()) {
        throw std::runtime_error("sending on a closed channel");
    }

    // we can buffer instead of blocking
    if (!full() && !have_receiver()) {
        emplace_immediately(std::forward<Args>(args)...);
        return true;
    }

    std::unique_lock<std::mutex> receiver_lock{};

    for (;;) {
        if (!have_receiver()) {
            m_receiver_update_cv.wait(lock, [this] { return have_receiver() || !full(); });
        }

        // the wake-up was to notify that there's free buffer space available
        // OR that the receive was cancelled
        if (!have_receiver()) {
            if (full()) {
                continue;
            } else {
                emplace_immediately(std::forward<Args>(args)...);
                detach_receiver_impl();
                return true;
            }
        }

        std::unique_lock receiver_lock_local{m_receive_sync->recv_mutex};

        if (m_receive_sync->recv_fulfilled) {  // someone else fulfilled this recv-op
            detach_receiver_impl();
            continue;
        }

        std::swap(receiver_lock, receiver_lock_local);
        break;
    }

    m_receive_sync->recv_fulfilled = m_receive_fulfill_id;
    m_receive_sync->recv_cv.notify_one();
    *m_receiver_buffer = T(std::forward<Args>(args)...);

    detach_receiver_impl();

    return true;
}

template<typename T, usize N, typename Allocator>
auto channel<T, N, Allocator>::attach_receiver(
  std::shared_ptr<detail::chan_receive_syncer> recv_sync,
  std::optional<T>& recv_buffer,
  usize id
) -> bool {
    std::unique_lock lock{m_mutex};

    if (have_receiver()) {
        m_receiver_update_cv.wait(lock, [this] { return !have_receiver(); });
    }

    if (!empty()) {
        // see the first remark
        // std::unique_lock sync_lock{recv_sync->recv_mutex};

        if (recv_sync->recv_fulfilled) {
            return false;
        }

        // hint at the senders that they might be able to buffer data
        // DO NOT use notify_one. it gives a false impression that these wake-ups are any different from spurious
        // ones. the fact that the mutex is held by senders will sort out races between waiting emplace_back calls.
        m_receiver_update_cv.notify_all();

        recv_sync->recv_fulfilled = id;
        recv_buffer = pop_immediately();
        return false;
    }

    if (closed()) {
        return false;
    }

    m_receive_fulfill_id = id;
    m_receive_sync = recv_sync;
    m_receiver_buffer = &recv_buffer;

    m_receiver_update_cv.notify_all();

    return true;
}

template<typename T, usize N, typename Allocator>
constexpr auto channel<T, N, Allocator>::pop_front() -> std::optional<T> {
    /*
     * Semantic notes:
     */

    auto recv_sync = std::make_shared<detail::chan_receive_syncer>();
    std::optional<T> recv_buffer = std::nullopt;

    std::unique_lock recv_lock{recv_sync->recv_mutex};

    if (attach_receiver(recv_sync, recv_buffer)) {
        recv_sync->recv_cv.wait(recv_lock, [&] { return recv_sync->recv_fulfilled; });
    }

    return recv_buffer;
}

template<typename... Selectors>
static void select(Selectors&&... selectors_arg) {
    detail::channel_selector_base* selectors[] = {(&selectors_arg)...};

    auto recv_sync = std::make_shared<detail::chan_receive_syncer>();
    std::unique_lock recv_lock{recv_sync->recv_mutex};

    for (usize i = 0; i < std::size(selectors); i++) {
        if (selectors[i]->attach(recv_sync, i + 1)) {
            continue;
        }

        for (usize j = 0; j < i; j++) {
            selectors[j]->fulfilled_by_someone_else();
        }

        selectors[i]->fulfilled();
        return;
    }

    recv_sync->recv_cv.wait(recv_lock, [&] { return recv_sync->recv_fulfilled; });
    selectors[recv_sync->recv_fulfilled - 1]->fulfilled();

    recv_lock.unlock();

    for (usize i = 0; i < std::size(selectors); i++) {
        if (i == recv_sync->recv_fulfilled - 1) {
            continue;
        }

        selectors[i]->fulfilled_by_someone_else();
    }
}

}  // namespace stf
