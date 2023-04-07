#pragma once

namespace stf {

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

template<typename Channel>
static auto receive(Channel& chan) -> std::optional<typename Channel::value_type> {
    auto recv_sync = std::make_shared<detail::chan_receive_syncer>();
    std::optional<typename Channel::value_type> recv_buffer = std::nullopt;

    std::unique_lock recv_lock{recv_sync->recv_mutex};

    if (chan.attach_receiver(recv_sync, recv_buffer)) {
        recv_sync->recv_cv.wait(recv_lock, [&] { return recv_sync->recv_fulfilled; });
    }

    return recv_buffer;
}

template<typename Channel, typename... Args>
static auto send(Channel& chan, Args&&... args) -> bool {
    return chan.emplace_back(std::forward<Args>(args)...);
}

}
