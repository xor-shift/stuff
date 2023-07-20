#pragma once

#include <stuff/core.hpp>

#include <atomic>
#include <condition_variable>
#include <mutex>

namespace stf {

struct wait_group {
    wait_group() = default;

    constexpr wait_group(wait_group const&) = delete;
    constexpr wait_group(wait_group&&) = delete;

    void add(u32 delta) noexcept { m_count.fetch_add(delta, std::memory_order::release); }

    void done() {
        const auto res = m_count.fetch_sub(1, std::memory_order::seq_cst);

        if (res == 0) {
            throw std::runtime_error("called done() on a wait_group with a counter of 0");
        }

        if (res == 1) {
            const auto _ = std::unique_lock(m_mutex);
            m_cv.notify_all();
        }
    }

    void wait() {
        auto lock = std::unique_lock(m_mutex);

        m_cv.wait(lock, [this] { return m_count.load(std::memory_order::acquire) == 0; });
    }

private:
    std::mutex m_mutex;

    std::atomic_uint32_t m_count;
    std::condition_variable m_cv;
};

}  // namespace stf
