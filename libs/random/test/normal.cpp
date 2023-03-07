#include <stuff/random.hpp>

#include <gtest/gtest.h>

template<typename Dist, typename Gen>
static void test_normal_distribution(Dist& distribution, Gen& generator, size_t sample_ct = 1024 * 32) {
    using T = typename Dist::result_type;

    T expected_mean = 0;
    T expected_stdev = 1;
    T max_error = 0.01;

    std::vector<T> samples{};
    samples.reserve(sample_ct);

    for (size_t i = 0uz; i < sample_ct; i++) {
        samples.emplace_back(distribution(generator));
    }

    T mean = std::accumulate(samples.begin(), samples.end(), T(0)) / static_cast<T>(sample_ct);

    ASSERT_TRUE(std::abs(mean - expected_mean) <= max_error);

    T stdev = 0;

    for (T v : samples) {
        T t = v - mean;
        stdev += t * t;
    }

    stdev = std::sqrt(stdev / static_cast<T>(sample_ct));

    ASSERT_TRUE(std::abs(stdev - expected_stdev) <= max_error);
}

TEST(random, normal_dist_bm) {
    stf::random::xoshiro_float_generator gen{};
    stf::random::normal_distribution_box_muller<float> dist{};

    test_normal_distribution(dist, gen);
}

TEST(random, normal_dist_marsaglia) {
    stf::random::xoshiro_float_generator gen{};
    stf::random::normal_distribution_marsaglia<float> dist{};

    test_normal_distribution(dist, gen);
}
