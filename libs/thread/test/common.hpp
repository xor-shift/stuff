#pragma once

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <stuff/core.hpp>
#include <stuff/random.hpp>

struct watchdog {
    template<typename Rep, typename Period>
    watchdog(std::chrono::duration<Rep, Period> fail_after) {
        m_thread = std::thread([this, fail_after] { worker(fail_after); });
    }

    ~watchdog() {
        cancel();
        m_thread.join();
    }

    void cancel() {
        const auto lock = std::unique_lock(m_mutex);
        m_cancelled = true;
        m_cancel_cv.notify_one();
    }

private:
    std::thread m_thread;

    std::mutex m_mutex{};
    std::condition_variable m_cancel_cv{};
    bool m_cancelled = false;

    template<typename Rep, typename Period>
    void worker(std::chrono::duration<Rep, Period> fail_after) {
        std::unique_lock lock{m_mutex};
        m_cancel_cv.wait_for(lock, fail_after, [&] { return m_cancelled; });

        if (m_cancelled) {
            return;
        }

        std::terminate();
    }
};

template<typename... Args>
void locking_print(Args&&... args) {
    static auto mutex = std::mutex{};
    const auto lock = std::unique_lock(mutex);

    (std::cout << ... << std::forward<Args>(args)) << std::endl;
}

template<std::signed_integral T = i64, typename Ratio = std::milli>
static void delay_a_bit(std::chrono::duration<T, Ratio> min, std::chrono::duration<T, Ratio> range = std::chrono::milliseconds(0)) {
    static auto rand_device = std::random_device{};
    static auto generator = stf::random::xoshiro_256pp{rand_device()};
    static auto mutex = std::mutex{};

    const auto range_repr = range.count();
    auto range_repr_dist = std::uniform_int_distribution<T>{-range_repr, range_repr};

    const auto offset = ({
        const auto _ = std::unique_lock(mutex);
        range_repr_dist(generator);
    });

    const auto dura_repr = min.count() + offset;
    const auto dura = std::chrono::duration<T, Ratio>(dura_repr);

    std::this_thread::sleep_for(dura);
}