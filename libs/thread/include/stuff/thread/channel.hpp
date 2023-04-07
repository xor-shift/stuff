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

}  // namespace detail

template<typename Selector, typename... Selectors>
static auto select(Selector&& selector_arg, Selectors&&... selectors_arg);

template<typename Channel>
static auto receive(Channel& chan) -> std::optional<typename Channel::value_type>;

template<typename Channel, typename... Args>
static auto send(Channel& chan, Args&&... args) -> bool;

template<typename ValueType, usize N, typename T>
struct channel_base {
    using value_type = ValueType;

    friend T;

    constexpr void close();

protected:
    template<typename... Args>
    constexpr auto emplace_back(Args&&... args) -> bool;

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

    /// Cancels a receive (should only be used by select())
    void detach_receiver();

    template<typename Selector, typename... Selectors>
    friend auto select(Selector&& selector_arg, Selectors&&... selectors_arg);

    template<typename Channel>
    friend auto receive(Channel& chan) -> std::optional<typename Channel::value_type>;

    template<typename Channel, typename... Args>
    friend auto send(Channel& chan, Args&&... args) -> bool;

    template<typename Channel, typename Fn>
    friend struct channel_selector;

private:
    mutable std::mutex m_mutex{};

    bool m_closed = false;

    std::condition_variable m_cv_to_receivers{};
    std::condition_variable m_cv_to_senders{};

    std::shared_ptr<detail::chan_receive_syncer> m_receive_sync = nullptr;
    usize m_receive_fulfill_id = 0;
    std::optional<value_type>* m_receiver_buffer = nullptr;

    usize m_buffer_usage = 0;

    constexpr auto get_self() -> T& { return *static_cast<T*>(this); }

    constexpr auto get_self() const -> T const& { return *static_cast<const T*>(this); }

    constexpr auto closed() const -> bool { return get_self().m_closed; }

    constexpr auto full() const -> bool { return get_self().m_buffer_usage == N + 1; }

    constexpr auto empty() const -> bool { return get_self().m_buffer_usage == 0; }

    constexpr auto have_receiver() const -> bool { return get_self().m_receive_sync != nullptr; }

    constexpr void fulfill_receiver(std::optional<value_type> value = std::nullopt);

    template<typename... Args>
    constexpr void emplace_immediately(Args&&... args) {
        return get_self().emplace_immediately(std::forward<Args>(args)...);
    }

    constexpr auto pop_immediately() -> value_type { return get_self().pop_immediately(); }
};

template<typename T, usize N = 0, typename Allocator = std::allocator<T>>
struct channel : channel_base<T, N, channel<T, N, Allocator>> {
    using value_type = T;
    using allocator_type = Allocator;

    friend struct channel_base<T, N, channel<T, N, Allocator>>;

    channel(allocator_type const& allocator = {});

    ~channel() noexcept;

private:
    allocator_type m_allocator;

    usize m_read_head = 0;  // next read index
    T* m_storage = nullptr;

    template<typename... Args>
    constexpr void emplace_immediately(Args&&... args);

    constexpr auto pop_immediately() -> value_type;
};

template<usize N, typename Allocator>
struct channel<void, N, Allocator> : channel_base<empty, N, channel<void, N, Allocator>> {
    using value_type = empty;
    using allocator_type = Allocator;

    friend struct channel_base<empty, N, channel<void, N, Allocator>>;

    channel() = default;

    ~channel() { this->close(); }

private:
    template<typename... Args>
    constexpr void emplace_immediately(Args&&... args) {
        ++this->m_buffer_usage;
    }

    constexpr auto pop_immediately() -> value_type {
        --this->m_buffer_usage;
        return {};
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

}  // namespace stf

#include <stuff/thread/detail/channel/channel_base.ipp>

#include <stuff/thread/detail/channel/channel.ipp>

#include <stuff/thread/detail/channel/select.ipp>
