#pragma once

namespace stf {

template<typename T, usize N, typename Allocator>
channel<T, N, Allocator>::channel(allocator_type const& allocator)
    : m_allocator(allocator)
    , m_storage(nullptr) {
    m_storage = m_allocator.allocate(N + 1);
}

template<typename T, usize N, typename Allocator>
channel<T, N, Allocator>::~channel() noexcept {
    this->close();
    if (m_storage != nullptr) {
        m_allocator.deallocate(m_storage, N + 1);
        m_storage = nullptr;
    }
}

template<typename T, usize N, typename Allocator>
template<typename... Args>
constexpr void channel<T, N, Allocator>::emplace_immediately(Args&&... args) {
    size_t index = (m_read_head + this->m_buffer_usage) % (N + 1);
    std::construct_at(m_storage + index, std::forward<Args>(args)...);
    ++this->m_buffer_usage;
}

template<typename T, usize N, typename Allocator>
constexpr auto channel<T, N, Allocator>::pop_immediately() -> value_type {
    usize index = m_read_head;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdivision-by-zero"
    ++m_read_head %= (N + 1);
#pragma GCC diagnostic pop
    --this->m_buffer_usage;

    value_type ret(std::move(m_storage[index]));
    std::destroy_at(m_storage + index);
    return ret;
}

}  // namespace stf
