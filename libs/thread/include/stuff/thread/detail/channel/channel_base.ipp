#pragma once

namespace stf {

template<typename ValueType, usize N, typename T>
constexpr void channel_base<ValueType, N, T>::close() {
    std::unique_lock lock{m_mutex};
    m_closed = true;

    m_cv_to_senders.notify_all();
    m_cv_to_receivers.notify_all();

    if (have_receiver()) {
        fulfill_receiver();
    }
}

template<typename ValueType, usize N, typename T>
template<typename... Args>
constexpr auto channel_base<ValueType, N, T>::emplace_back(Args&&... args) -> bool {
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

template<typename ValueType, usize N, typename T>
auto channel_base<ValueType, N, T>::attach_receiver(
  std::shared_ptr<detail::chan_receive_syncer> recv_sync,
  std::optional<value_type>& recv_buffer,
  usize id
) -> bool {
    // please excuse the comments, i am kind of dumb (very dumb) and can't keep track of everything without hints

    T& self = *static_cast<T*>(this);

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
        m_cv_to_receivers.wait(lock, [this, &self] { return !have_receiver() || closed(); });

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

template<typename ValueType, usize N, typename T>
void channel_base<ValueType, N, T>::detach_receiver() {
    std::unique_lock lock{m_mutex};

    m_receive_sync = nullptr;
    m_receiver_buffer = nullptr;

    m_cv_to_receivers.notify_all();
}

template<typename ValueType, usize N, typename T>
constexpr void channel_base<ValueType, N, T>::fulfill_receiver(std::optional<value_type> value) {
    std::unique_lock lock{m_receive_sync->recv_mutex};
    m_receive_sync->recv_fulfilled = m_receive_fulfill_id;
    m_receive_sync->recv_cv.notify_all();
    *m_receiver_buffer = value;

    m_receive_sync = nullptr;
    m_receiver_buffer = nullptr;

    m_cv_to_receivers.notify_all();
}

}
