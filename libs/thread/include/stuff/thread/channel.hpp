#pragma once

#include <stuff/core.hpp>
#include <stuff/scope.hpp>

#include <condition_variable>
#include <mutex>
#include <optional>

namespace stf {

namespace detail {

struct chan_receive_syncer {
    std::mutex recv_mutex{};
    usize recv_fulfilled = 0;
    std::condition_variable recv_cv{};
};

struct empty {};

}  // namespace detail

template<typename T, usize N = 0, typename Allocator = std::allocator<T>>
struct channel {
    using value_type = std::conditional_t<std::is_void_v<T>, detail::empty, T>;
    using allocator_type = Allocator;

    channel(allocator_type const& allocator = {});

    ~channel() noexcept;

    constexpr void close();

    template<typename... Args>
    constexpr auto emplace_back(Args&&... args) -> bool;

    constexpr auto pop_front() -> std::optional<value_type>;

    /// \remark
    /// The attacher MUST be holding the lock within recv_sync.
    /// \return
    /// If true, then the receiver should block on the condition variable.\n
    /// If false and !recv_buffer, the channel is closed and no data can be received OR the recv_sync was already
    /// fulfilled.\n
    /// If false and !!recv_buffer, there was buffered data prepared.
    auto attach_receiver(
      std::shared_ptr<detail::chan_receive_syncer> recv_sync,
      std::optional<value_type>& recv_buffer,
      usize id = 1
    ) -> bool;

    /// Forcibly detaches a receiver
    void detach_receiver() {
        std::unique_lock lock{m_mutex};

        m_receive_sync = nullptr;
        m_receiver_buffer = nullptr;

        m_cv_to_receivers.notify_all();
    }

private:
    allocator_type m_allocator;

    bool m_closed = false;

    mutable std::mutex m_mutex{};

    std::condition_variable m_cv_to_receivers{};
    std::condition_variable m_cv_to_senders{};

    std::shared_ptr<detail::chan_receive_syncer> m_receive_sync = nullptr;
    usize m_receive_fulfill_id = 0;
    std::optional<value_type>* m_receiver_buffer = nullptr;

    usize m_read_head = 0;  // next read index
    usize m_buffer_usage = 0;
    T* m_storage = nullptr;

    constexpr auto closed() const -> bool { return m_closed; }

    constexpr auto full() const -> bool { return m_buffer_usage == N + 1; }

    constexpr auto empty() const -> bool { return m_buffer_usage == 0; }

    constexpr auto have_receiver() const -> bool { return m_receive_sync != nullptr; }

    constexpr void fulfill_receiver(std::optional<value_type> value = std::nullopt) {
        std::unique_lock lock{m_receive_sync->recv_mutex};
        m_receive_sync->recv_fulfilled = m_receive_fulfill_id;
        m_receive_sync->recv_cv.notify_all();
        *m_receiver_buffer = value;

        m_receive_sync = nullptr;
        m_receiver_buffer = nullptr;

        m_cv_to_receivers.notify_all();
    }

    template<typename... Args>
    constexpr void emplace_immediately(Args&&... args) {
        size_t index = (m_read_head + m_buffer_usage) % (N + 1);
        if constexpr (!std::is_void_v<T>) {
            std::construct_at(m_storage + index, std::forward<Args>(args)...);
        }
        ++m_buffer_usage;
    }

    constexpr auto pop_immediately() -> value_type {
        usize index = m_read_head;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdivision-by-zero"
        ++m_read_head %= (N + 1);
#pragma GCC diagnostic pop
        --m_buffer_usage;

        if constexpr (!std::is_void_v<T>) {
            value_type ret(std::move(m_storage[m_read_head]));
            std::destroy_at(m_storage + index);
            return ret;
        } else {
            return {};
        }
    }
};

namespace detail {

template<typename Ret>
struct channel_selector_base {
    using value_type = Ret;

    virtual constexpr ~channel_selector_base() = default;

    virtual auto attach(std::shared_ptr<detail::chan_receive_syncer> recv_sync, usize id) -> bool = 0;

    virtual auto fulfilled() -> Ret = 0;

    virtual void fulfilled_by_someone_else() = 0;
};

}  // namespace detail

template<typename Channel, typename Fn>
struct channel_selector final
    : detail::channel_selector_base<std::invoke_result_t<Fn, std::optional<typename Channel::value_type>>> {
    using value_type = std::invoke_result_t<Fn, std::optional<typename Channel::value_type>>;

    template<typename Fnn>
    constexpr channel_selector(Channel& channel, Fnn&& fn)
        : m_channel(channel)
        , m_fn(std::forward<Fnn>(fn)) {}

    // if false, selection should conclude without waiting (the channel just closed or was closed or a buffered result
    // existed)
    auto attach(std::shared_ptr<detail::chan_receive_syncer> recv_sync, usize id) -> bool final override {
        return m_channel.attach_receiver(recv_sync, m_recv_buffer, id);
    }

    auto fulfilled() -> value_type final override {
        if constexpr (std::is_void_v<value_type>) {
            m_fn(m_recv_buffer);
            m_recv_buffer = std::nullopt;
        } else {
            value_type ret = m_fn(m_recv_buffer);
            m_recv_buffer = std::nullopt;
            return ret;
        }
    }

    void fulfilled_by_someone_else() final override {
        m_channel.detach_receiver();
        m_recv_buffer = std::nullopt;
    }

private:
    Channel& m_channel;
    Fn m_fn;

    std::optional<typename Channel::value_type> m_recv_buffer;
};

template<typename Channel, typename Fn>
channel_selector(Channel&, Fn&&) -> channel_selector<Channel, std::decay_t<Fn>>;

template<typename Fn>
struct default_channel_selector final : detail::channel_selector_base<std::invoke_result_t<Fn>> {
    using value_type = std::invoke_result_t<Fn>;

    template<typename Fnn>
    constexpr default_channel_selector(Fnn&& fn)
        : m_fn(std::forward<Fnn>(fn)) {}

    auto attach(std::shared_ptr<detail::chan_receive_syncer> sync, usize id) -> bool final override {
        sync->recv_fulfilled = id;
        return false;
    }

    auto fulfilled() -> value_type final override { return m_fn(); }

    void fulfilled_by_someone_else() final override {}

private:
    Fn m_fn;
};

template<typename Fn>
default_channel_selector(Fn&&) -> default_channel_selector<std::decay_t<Fn>>;

template<typename Selector, typename... Selectors>
static auto select(Selector&& selector_arg, Selectors&&... selectors_arg);

}  // namespace stf

#include <stuff/thread/detail/channel.ipp>
