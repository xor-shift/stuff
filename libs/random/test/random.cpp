#include <stuff/random.hpp>

#include <gtest/gtest.h>

#include <random>

TEST(random, get_entropy) {
    // https://en.wikipedia.org/wiki/Linear_congruential_generator#Parameters_in_common_use
    std::linear_congruential_engine<uint64_t, 0x5DEECE66DU, 11, 1ull << 48> gen{0};

    struct drand48_data rand_data {};
    uint16_t rand_vector[3]{0};
    long rand_out;
    auto discarded = nrand48_r(rand_vector, &rand_data, &rand_out);

    for (int i = 0; i < 512; i++, nrand48_r(rand_vector, &rand_data, &rand_out)) {
        uint64_t std_res = 0;
        std_res |= rand_vector[2];
        std_res <<= 16;
        std_res |= rand_vector[1];
        std_res <<= 16;
        std_res |= rand_vector[0];

        auto stf_res = stf::random::get_entropy<uint64_t>(gen, 48);

        ASSERT_EQ(std_res, stf_res) << "iteration #" << i;
    }
}

template<std::floating_point T, stf::random::concepts::uniform_random_bit_generator G>
static void test_distribution(size_t n_samples = 1024 * 128) {
    T sum = 0.;

    G gen{0};
    stf::random::erand48_distribution<T> dist{};

    for (size_t i = 0; i < n_samples; i++) {
        sum += dist(gen);
    }

    T avg = sum / static_cast<T>(n_samples);

    ASSERT_TRUE(std::abs(avg - 0.5) < 0.001);
}

TEST(random, erand_double_distribution) {
    test_distribution<double, stf::random::erand_double_generator>();
}

TEST(random, erand_float_distribution) {
    test_distribution<float, stf::random::erand_float_generator>();
}

TEST(random, xoshiro_double_distribution) {
    test_distribution<double, stf::random::xoshiro_double_generator>();
}

TEST(random, xoshiro_float_distribution) {
    test_distribution<float, stf::random::xoshiro_float_generator>();
}

template<typename G, typename Fn>
static void test_splitmix(Fn&& next) {
    typename G::result_type state = 0;
    G rng{state};

    for (int i = 0; i < 100; i++) {
        auto baseline = next(state);
        auto got = rng();

        ASSERT_EQ(baseline, got) << "test#" << i;
    }
}

TEST(random, splitmix_64) {
    auto next = [](uint64_t& x) {
        uint64_t z = (x += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    };

    test_splitmix<stf::random::splitmix_64_generator>(next);
}

TEST(random, splitmix_32) {
    auto next = [](uint32_t& x) {
        x = 1664525u * (x + 314159265u);
        uint32_t z = x;
        z = (z ^ (z >> 15)) * 0x5ce4e5b9u;
        z = (z ^ (z >> 13)) * 0x1331c1ebu;
        return z ^ (z >> 15);
    };

    test_splitmix<stf::random::splitmix_32_generator>(next);
}

template<stf::random::concepts::random_number_engine E, typename FnNext>
static void test_xoroshiro(FnNext&& fn_next) {
    E gen{0xDEADBEEFCAFEBABEull};
    uint64_t state[2];
    std::copy_n(gen.state().begin(), 2, state);

    for (int i = 0; i < 100; i++) {
        auto baseline = fn_next(state);
        auto got = gen();

        ASSERT_EQ(baseline, got) << "test#" << i;
    }
};

template<stf::random::concepts::random_number_engine E, typename FnNext, typename FnJmp, typename MemJmp>
static void test_xoroshiro_jmp(FnNext&& fn_next, FnJmp&& fn_jmp, MemJmp&& mem_jmp) {
    E gen{0xDEADBEEFCAFEBABEull};
    uint64_t state[2];
    std::copy_n(gen.state().begin(), 2, state);

    for (int i = 0; i < 16; i++) {
        fn_jmp(state);
        (gen.*mem_jmp)();

        for (int j = 0; j < 100; j++) {
            auto baseline = fn_next(state);
            auto got = gen();

            ASSERT_EQ(baseline, got) << "test#(" << i << ',' << j << ')';
        }
    }
};

template<stf::random::concepts::random_number_engine E, typename FnNext, typename FnJmpShort, typename FnJmpLong>
static void test_xoroshiro(FnNext&& fn_next, FnJmpShort&& fn_jmp_short, FnJmpLong&& fn_jmp_long) {
    test_xoroshiro<E>(std::forward<FnNext>(fn_next));

    test_xoroshiro_jmp<E>(std::forward<FnNext>(fn_next), std::forward<FnJmpShort>(fn_jmp_short), &E::jump_64);

    test_xoroshiro_jmp<E>(std::forward<FnNext>(fn_next), std::forward<FnJmpLong>(fn_jmp_long), &E::jump_96);
};

TEST(random, xoroshiro_128p) {
    // BASELINE CODE FROM: https://prng.di.unimi.it/xoroshiro128plus.c

    auto next = [](uint64_t(&s)[2]) -> uint64_t {
        const uint64_t s0 = s[0];
        uint64_t s1 = s[1];
        const uint64_t result = s0 + s1;

        s1 ^= s0;
        s[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16);
        s[1] = std::rotl(s1, 37);

        return result;
    };

    auto s_jmp = [&next](uint64_t(&s)[2]) {
        static const uint64_t JUMP[] = {0xdf900294d8f554a5, 0x170865df4b3201fc};

        uint64_t s0 = 0;
        uint64_t s1 = 0;
        for (int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
            for (int b = 0; b < 64; b++) {
                if (JUMP[i] & UINT64_C(1) << b) {
                    s0 ^= s[0];
                    s1 ^= s[1];
                }
                next(s);
            }

        s[0] = s0;
        s[1] = s1;
    };

    auto l_jmp = [&next](uint64_t(&s)[2]) {
        static const uint64_t LONG_JUMP[] = {0xd2a98b26625eee7b, 0xdddf9b1090aa7ac1};

        uint64_t s0 = 0;
        uint64_t s1 = 0;
        for (int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
            for (int b = 0; b < 64; b++) {
                if (LONG_JUMP[i] & UINT64_C(1) << b) {
                    s0 ^= s[0];
                    s1 ^= s[1];
                }
                next(s);
            }

        s[0] = s0;
        s[1] = s1;
    };

    test_xoroshiro<stf::random::xoroshiro_128p>(next, s_jmp, l_jmp);
}

TEST(random, xoroshiro_128pp) {
    auto next = [](uint64_t(&s)[2]) -> uint64_t {
        const uint64_t s0 = s[0];
        uint64_t s1 = s[1];
        const uint64_t result = std::rotl(s0 + s1, 17) + s0;

        s1 ^= s0;
        s[0] = std::rotl(s0, 49) ^ s1 ^ (s1 << 21);  // a, b
        s[1] = std::rotl(s1, 28);                    // c

        return result;
    };

    test_xoroshiro<stf::random::xoroshiro_128pp>(next);
}

TEST(random, xoroshiro_128ss) {
    auto next = [](uint64_t(&s)[2]) -> uint64_t {
        const uint64_t s0 = s[0];
        uint64_t s1 = s[1];
        const uint64_t result = std::rotl(s0 * 5, 7) * 9;

        s1 ^= s0;
        s[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16);  // a, b
        s[1] = std::rotl(s1, 37);                    // c

        return result;
    };

    test_xoroshiro<stf::random::xoroshiro_128ss>(next);
}
