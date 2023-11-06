#include <stuff/hash.hpp>
#include <stuff/random.hpp>

#include <benchmark/benchmark.h>

#include <random>
#include <ranges>
#include <unordered_set>

namespace {

inline auto rand_seed = std::random_device{}();

struct definitely_not_an_int {
    int oops_an_int;

    constexpr operator int() const { return oops_an_int; }

    friend constexpr auto operator==(definitely_not_an_int const& lhs, definitely_not_an_int const& rhs) -> bool { return (int)lhs == (int)rhs; }
};

struct bad_int_hash {
    constexpr auto operator()(definitely_not_an_int not_v) const -> usize {
        auto v = not_v.oops_an_int;
        v = v > 0 ? v : -v;

        static constexpr int primes[]{2, 3, 5, 7, 11, 13, 17, 19};
        return primes[v % 8];
    }
};

struct good_int_hash {
    auto operator()(definitely_not_an_int not_v) const -> usize {
        auto v = not_v.oops_an_int;
        return std::hash<int>{}(v);
    }
};

}  // namespace

static void sanity_check_good_hash(benchmark::State& state) {
    auto rand_engine = stf::random::xoshiro_256pp{rand_seed};
    auto rand_dist = std::uniform_int_distribution<int>{0, 32};

    auto set_reference = std::unordered_set<definitely_not_an_int, good_int_hash>{};

    for (auto i = 0; auto _ : state) {
        set_reference.emplace(i);

        if (rand_dist(rand_engine) == 31) {
            set_reference.erase({i / 2});
        }

        ++i;
    }
}

static void sanity_check_bad_hash(benchmark::State& state) {
    auto rand_engine = stf::random::xoshiro_256pp{rand_seed};
    auto rand_dist = std::uniform_int_distribution<int>{0, 32};

    auto set_bad = std::unordered_set<definitely_not_an_int, bad_int_hash>{};

    for (auto i = 0; auto _ : state) {
        set_bad.emplace(i);

        if (rand_dist(rand_engine) == 31) {
            set_bad.erase({i / 2});
        }

        ++i;
    }
}

BENCHMARK(sanity_check_good_hash);
BENCHMARK(sanity_check_bad_hash);

template<typename T0, typename T1>
static void check_pair_sequential(benchmark::State& state) {
    auto rand_engine = stf::random::xoshiro_256pp{rand_seed};
    auto roulette_dist = std::uniform_int_distribution<int>{0, 32};

    auto set = std::unordered_set<std::pair<T0, T1>>{};

    for (auto i = 0; auto _ : state) {
        set.emplace(i + 1, i + 2);

        if (roulette_dist(rand_engine) == 31) {
            set.erase(std::pair<T0, T1>{(i + 1) / 2, (i + 2) / 2});
        }

        i += 2;
    }
}

BENCHMARK(check_pair_sequential<int, int>);
BENCHMARK(check_pair_sequential<float, float>);

template<std::integral T0, std::integral T1>
static void check_pair_random(benchmark::State& state) {
    auto rand_engine = stf::random::xoshiro_256pp{rand_seed};
    auto roulette_dist = std::uniform_int_distribution<int>{0, 32};

    auto set_dist_0 = std::uniform_int_distribution<T0>{
      std::numeric_limits<T0>::min(),
      std::numeric_limits<T0>::max()
    };

    auto set_dist_1 = std::uniform_int_distribution<T1>{
      std::numeric_limits<T1>::min(),
      std::numeric_limits<T1>::max()
    };

    auto set = std::unordered_set<std::pair<T0, T1>>{};

    for (auto _ : state) {
        set.emplace(set_dist_0(rand_engine), set_dist_1(rand_engine));
    }
}

BENCHMARK(check_pair_random<int, int>);
