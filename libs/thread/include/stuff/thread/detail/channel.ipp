#pragma once

namespace stf {

template<typename T, usize N, typename Allocator>
channel<T, N, Allocator>::channel(allocator_type const& allocator)
    : m_allocator(allocator)
    , m_storage(nullptr) {
    if constexpr (!std::is_void_v<T>) {
        m_storage = m_allocator.allocate(N + 1);
    }
}

template<typename T, usize N, typename Allocator>
channel<T, N, Allocator>::~channel() noexcept {
    close();

    if constexpr (!std::is_void_v<T>) {
        if (m_storage != nullptr) {
            m_allocator.deallocate(m_storage, N + 1);
            m_storage = nullptr;
        }
    }
}

template<typename T, usize N, typename Allocator>
constexpr void channel<T, N, Allocator>::close() {
    std::unique_lock lock{m_mutex};
    m_closed = true;

    m_cv_to_senders.notify_all();
    m_cv_to_receivers.notify_all();

    if (have_receiver()) {
        fulfill_receiver();
    }
}

template<typename T, usize N, typename Allocator>
template<typename... Args>
constexpr auto channel<T, N, Allocator>::emplace_back(Args&&... args) -> bool {
    std::unique_lock lock{m_mutex};

    if (closed()) {
        return false;
        // throw std::runtime_error("sending on a closed channel");
    }

    if (full()) {
        // block until someone receives the data
        // or until the channel is closed
        m_cv_to_senders.wait(lock, [this] { return !full() || closed(); });

        if (closed()) {
            return false;
        }
    }

    emplace_immediately(std::forward<Args>(args)...);

    if (have_receiver()) {
        fulfill_receiver(std::move(pop_immediately()));
        return true;
    }

    while (full()) {
        m_cv_to_senders.wait(lock, [this] { return !full() || closed(); });

        if (closed()) {
            return false;
        }

        // receive was cancelled, we *have* to be still full()
        /*if (!have_receiver()) {
            continue;
        }*/

        break;
    }

    return true;
}

template<typename T, usize N, typename Allocator>
auto channel<T, N, Allocator>::attach_receiver(
  std::shared_ptr<detail::chan_receive_syncer> recv_sync,
  std::optional<value_type>& recv_buffer,
  usize id
) -> bool {
    // please excuse the comments, i am kind of dumb (very dumb) and can't keep track of everything without hints

    std::unique_lock lock{m_mutex};

    if (closed()) {
        recv_sync->recv_fulfilled = id;
        if (empty()) {
            recv_buffer = std::nullopt;
        } else {
            recv_buffer = pop_immediately();
        }
        return false;
    }

    if (have_receiver()) {
        m_cv_to_receivers.wait(lock, [this] { return !have_receiver() || closed(); });

        if (closed()) {
            recv_sync->recv_fulfilled = id;
            recv_buffer = std::nullopt;
            return false;
        }
    }

    if (empty()) {  // no data at hand, register the sync object for the next emplace_back to use
        m_receive_sync = recv_sync;
        m_receiver_buffer = &recv_buffer;
        m_receive_fulfill_id = id;

        // this should do nothing as there must be some data if there is a sender
        // oh well
        m_cv_to_senders.notify_all();

        return true;
    }

    // oops, the mutex should be held anyway
    // std::unique_lock recv_lock { recv_sync->recv_mutex };

    recv_sync->recv_fulfilled = id;
    recv_buffer = pop_immediately();

    // there might be a sender waiting (if we are currently full(), more specifically), notify them
    m_cv_to_senders.notify_all();

    return false;
}

template<typename T, usize N, typename Allocator>
constexpr auto channel<T, N, Allocator>::pop_front() -> std::optional<value_type> {
    auto recv_sync = std::make_shared<detail::chan_receive_syncer>();
    std::optional<value_type> recv_buffer = std::nullopt;

    std::unique_lock recv_lock{recv_sync->recv_mutex};

    if (attach_receiver(recv_sync, recv_buffer)) {
        recv_sync->recv_cv.wait(recv_lock, [&] { return recv_sync->recv_fulfilled; });
    }

    return recv_buffer;
}

template<typename Selector, typename... Selectors>
static auto select(Selector&& selector_arg, Selectors&&... selectors_arg) {
    using value_type = typename std::decay_t<Selector>::value_type;

    detail::channel_selector_base<value_type>* selectors[] = {&selector_arg, (&selectors_arg)...};

    auto recv_sync = std::make_shared<detail::chan_receive_syncer>();
    std::unique_lock recv_lock{recv_sync->recv_mutex};

    usize attach_i = 0;

    for (; attach_i < std::size(selectors); attach_i++) {
        if (selectors[attach_i]->attach(recv_sync, attach_i + 1)) {
            continue;
        }

        /*for (usize j = 0; j < i; j++) {
            selectors[j]->fulfilled_by_someone_else();
        }

        return selectors[i]->fulfilled();*/

        break;
    }

    auto cancel_others = [&](usize upto) {
        for (usize i = 0; i < std::min(std::size(selectors), upto); i++) {
            if (i == recv_sync->recv_fulfilled - 1) {
                continue;
            }

            selectors[i]->fulfilled_by_someone_else();
        }
    };

    if (attach_i == std::size(selectors)) {
        recv_sync->recv_cv.wait(recv_lock, [&] { return recv_sync->recv_fulfilled; });
    }

    if constexpr (std::is_void_v<value_type>) {
        selectors[recv_sync->recv_fulfilled - 1]->fulfilled();

        recv_lock.unlock();
        cancel_others(attach_i);
    } else {
        auto res = selectors[recv_sync->recv_fulfilled - 1]->fulfilled();

        recv_lock.unlock();
        cancel_others(attach_i);

        return res;
    }
}

}  // namespace stf
