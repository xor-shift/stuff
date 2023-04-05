#include <stuff/thread/channel.hpp>

#include <gtest/gtest.h>

struct watchdog {
    template<typename Rep, typename Period>
    watchdog(std::chrono::duration<Rep, Period> fail_after)
        : m_thread([this, fail_after] { worker(fail_after); }) {}

    ~watchdog() {
        cancel();
        m_thread.join();
    }

    void cancel() {
        std::unique_lock lock{m_mutex};
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

template<usize ChannelSize>
static void test_basic_consumer_producer(usize expected_recv_count) {
    watchdog watchdog{std::chrono::seconds(expected_recv_count)};

    stf::channel<usize, ChannelSize> channel{};
    usize recv_count = 0;

    std::thread producer{[&] {
        for (usize i = 0; i < expected_recv_count; i++) {
            std::cout << "[producer] pushing " << i << std::endl;
            channel.emplace_back(i);
        }
        channel.close();
        std::cout << "[producer] closing channel and exiting" << std::endl;
    }};

    std::thread consumer{[&] {
        for (;;) {
            if (auto res = channel.pop_front(); res) {
                ++recv_count;
                std::cout << "[consumer] popped " << *res << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            } else {
                break;
            }
        }
        std::cout << "[consumer] exiting" << std::endl;
    }};

    producer.join();
    consumer.join();

    ASSERT_EQ(recv_count, expected_recv_count);
}

TEST(channel, test_0_unbuffered) {
    test_basic_consumer_producer<0>(5);
}

TEST(channel, test_0_buffered) {
    test_basic_consumer_producer<1>(10);
}

template<usize ChannelSize>
static void test_single_producer_multiple_consumer(usize expected_recv_count) {
    watchdog watchdog{std::chrono::seconds(expected_recv_count)};

    stf::channel<usize, ChannelSize> channel{};
    std::atomic_size_t recv_count = 0;

    std::thread producer_thread{[&] {
        for (usize i = 0; i < expected_recv_count; i++) {
            std::cout << "[producer] pushing " << i << std::endl;
            channel.emplace_back(i);
        }
        channel.close();
        std::cout << "[producer] closing channel and exiting" << std::endl;
    }};

    auto consumer = [&]<typename Rep, typename Period>(usize id, std::chrono::duration<Rep, Period> sleep_period) {
        for (;;) {
            if (auto res = channel.pop_front(); res) {
                ++recv_count;
                std::cout << "[consumer " << id << "] popped " << *res << std::endl;
                std::this_thread::sleep_for(sleep_period);
            } else {
                break;
            }
        }
        std::cout << "[consumer " << id << "] exiting" << std::endl;
    };

    std::thread consumer_0{[&] { consumer(0, std::chrono::milliseconds(250)); }};
    std::thread consumer_1{[&] { consumer(1, std::chrono::milliseconds(500)); }};
    std::thread consumer_2{[&] { consumer(2, std::chrono::milliseconds(750)); }};

    producer_thread.join();
    consumer_0.join();
    consumer_1.join();
    consumer_2.join();

    ASSERT_EQ(recv_count, expected_recv_count);
}

TEST(channel, test_1_unbuffered) {
    test_single_producer_multiple_consumer<0>(20);
}

TEST(channel, test_1_buffered) {
    test_single_producer_multiple_consumer<1>(25);
}

TEST(channel, test_2) {
    watchdog watchdog{std::chrono::seconds(5)};

    stf::channel<int, 1> chan_0{};
    stf::channel<float, 0> chan_1{};
    stf::channel<int, 0> chan_notifier{};

    std::thread producer{[&] {
        for (int i = 0; i < 20; i++) {
            std::cout << "[producer] pushing " << i << " to chan_" << (i % 2) << std::endl;

            if (i % 2 == 0) {
                chan_0.emplace_back(i);
            } else {
                chan_1.emplace_back(i);
            }
        }

        chan_notifier.close();
        std::cout << "[producer] closing chan_notifier and exiting" << std::endl;
    }};

    auto consumer = [&]<typename Rep, typename Period>(usize id, std::chrono::duration<Rep, Period> sleep_period) {
        bool closing = false;

        for (int i = 0;; i++) {
            std::this_thread::sleep_for(sleep_period);

            stf::select(
              stf::channel_selector{
                chan_0,
                [&](std::optional<int> i) {
                    if (i) {
                        std::cout << "[consumer " << id << "] popped " << *i << " from chan_0" << std::endl;
                    } else {
                        std::cout << "[consumer " << id << "] chan_0 closed" << std::endl;
                    }
                },
              },
              stf::channel_selector{
                chan_1,
                [&](std::optional<float> i) {
                    if (i) {
                        std::cout << "[consumer " << id << "] popped " << *i << " from chan_1" << std::endl;
                    } else {
                        std::cout << "[consumer " << id << "] chan_1 closed" << std::endl;
                    }
                },
              },
              stf::channel_selector{
                chan_notifier,
                [&](std::optional<int> i) {
                    if (i) {
                        std::cout << "[consumer " << id << "] popped " << *i << " from chan_notifier" << std::endl;
                    } else {
                        std::cout << "[consumer " << id << "] chan_notifier closed" << std::endl;
                    }
                    closing = true;
                },
              }
            );

            if (closing) {
                break;
            }
        }

        std::cout << "[consumer] exiting" << std::endl;
    };

    std::thread consumer_0{[&] { consumer(0, std::chrono::milliseconds(250)); }};
    std::thread consumer_1{[&] { consumer(1, std::chrono::milliseconds(250)); }};

    producer.join();
    consumer_0.join();
    consumer_1.join();
}
