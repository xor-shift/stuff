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

/*template<typename T, usize N, typename Allocator = std::allocator<T>>
struct channel;

template<typename T>
struct channel<T, 0, void> {};*/

template<typename T, usize N, typename Allocator = std::allocator<T>>
struct channel {
    using value_type = T;
    using allocator_type = Allocator;

    channel(allocator_type const& allocator = {});

    ~channel() noexcept;

    constexpr auto is_buffered() const -> bool;

    constexpr void close();

    template<typename... Args>
    constexpr auto emplace_back(Args&&... args) -> bool;

    constexpr auto pop_front() -> std::optional<T>;

    /// \remark
    /// The attacher MUST be holding the lock within recv_sync.
    /// \return
    /// If true, then the receiver should block on the condition variable.\n
    /// If false and !recv_buffer, the channel is closed and no data can be received OR the recv_sync was already
    /// fulfilled.\n
    /// If false and !!recv_buffer, there was buffered data prepared.
    auto
    attach_receiver(std::shared_ptr<detail::chan_receive_syncer> recv_sync, std::optional<T>& recv_buffer, usize id = 1)
      -> bool;

    /// Forcibly detaches a receiver
    void detach_receiver() {
        std::unique_lock lock {m_mutex};
        detach_receiver_impl();
        m_receiver_ready_cv.notify_all();
    }

private:
    allocator_type m_allocator;

    bool m_closed = false;

    mutable std::mutex m_mutex{};

    std::condition_variable m_receiver_ready_cv{};
    std::condition_variable m_emplace_complete_cv{};

    std::shared_ptr<detail::chan_receive_syncer> m_receive_sync = nullptr;
    usize m_receive_fulfill_id = 0;
    std::optional<T>* m_receiver_buffer = nullptr;

    usize m_read_head = 0;  // next read index
    usize m_buffer_usage = 0;
    T* m_storage = nullptr;

    constexpr auto closed() const -> bool { return m_closed; }

    constexpr auto full() const -> bool { return is_buffered() ? m_buffer_usage == N : true; }

    constexpr auto empty() const -> bool { return is_buffered() ? m_buffer_usage == 0 : true; }

    constexpr auto have_receiver() const -> bool { return m_receive_sync != nullptr; }

    constexpr void detach_receiver_impl() {
        m_receive_sync = nullptr;
        m_receiver_buffer = nullptr;
    }

    template<typename... Args>
    constexpr void emplace_immediately(Args&&... args) {
        assume(N != 0);
        std::construct_at(m_storage + m_read_head, std::forward<Args>(args)...);
        ++m_buffer_usage;
    }

    constexpr auto pop_immediately() -> T {
        assume(N != 0);

        T ret(std::move(m_storage[m_read_head]));
        std::destroy_at(m_storage + m_read_head);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdivision-by-zero"
        ++m_read_head %= N;
#pragma GCC diagnostic pop
        --m_buffer_usage;

        return ret;
    }
};

namespace detail {

struct channel_selector_base {
    virtual constexpr ~channel_selector_base() = default;

    virtual auto attach(std::shared_ptr<detail::chan_receive_syncer> recv_sync, usize id) -> bool = 0;

    virtual void fulfilled() = 0;

    virtual void fulfilled_by_someone_else() = 0;
};

}  // namespace detail

template<typename Channel, typename Fn>
struct channel_selector final : detail::channel_selector_base {
    template<typename Fnn>
    constexpr channel_selector(Channel& channel, Fnn&& fn)
        : m_channel(channel)
        , m_fn(std::forward<Fnn>(fn)) {}

    // if false, selection should conclude without waiting (the channel just closed or was closed or a buffered result
    // existed)
    auto attach(std::shared_ptr<detail::chan_receive_syncer> recv_sync, usize id) -> bool final override {
        return m_channel.attach_receiver(recv_sync, m_recv_buffer, id);
    }

    void fulfilled() final override { m_fn(m_recv_buffer); }

    void fulfilled_by_someone_else() final override {
        m_channel.detach_receiver();
    }

private:
    Channel& m_channel;
    Fn m_fn;

    std::optional<typename Channel::value_type> m_recv_buffer;
};

template<typename Channel, typename Fn>
channel_selector(Channel&, Fn&&) -> channel_selector<Channel, std::decay_t<Fn>>;

template<typename... Selectors>
static void select(Selectors&&... selectors_arg);

struct selector_builder {};

}  // namespace stf

#include <stuff/thread/detail/channel.ipp>
