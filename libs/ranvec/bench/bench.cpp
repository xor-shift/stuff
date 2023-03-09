#include <benchmark/benchmark.h>

#include <stuff/random.hpp>
#include <stuff/ranvec.hpp>

static std::random_device rd{};
static stf::random::xoroshiro_128p xoro_128p{rd()};
static stf::random::xoroshiro_128pp xoro_128pp{rd()};
static stf::random::xoroshiro_128ss xoro_128ss{rd()};
static stf::random::xoshiro_256p xo_256p{rd()};
static stf::random::xoshiro_256pp xo_256pp{rd()};
static stf::random::xoshiro_256ss xo_256ss{rd()};

// scalar distributions

static stf::random::erand48_distribution<double> erand_dist;
static stf::random::normal_distribution_marsaglia<double> norm_dist_marsaglia;
static stf::random::normal_distribution_box_muller<double> norm_dist_box_muller;

template<typename Gen>
static void generator(benchmark::State& state, Gen& gen) {
    std::array<typename Gen::result_type, 16> samples;

    for (auto _ : state) {
        std::generate(samples.begin(), samples.end(), gen);
        for (auto& v : samples)
            benchmark::DoNotOptimize(v);
    }

    state.counters["Count"] = {static_cast<double>(state.iterations() * samples.size()), benchmark::Counter::kIsRate};
}

BENCHMARK_CAPTURE(generator, xoro_128p, xoro_128p);
BENCHMARK_CAPTURE(generator, xoro_128pp, xoro_128pp);
BENCHMARK_CAPTURE(generator, xoro_128ss, xoro_128ss);
BENCHMARK_CAPTURE(generator, xo_256p, xo_256p);
BENCHMARK_CAPTURE(generator, xo_256pp, xo_256pp);
BENCHMARK_CAPTURE(generator, xo_256ss, xo_256ss);

template<typename Dist, typename Gen>
static void distribution(benchmark::State& state, Dist& dist, Gen& gen) {
    std::array<typename Dist::result_type, 16> samples;

    for (auto _ : state) {
        std::generate(samples.begin(), samples.end(), gen);
        for (auto& v : samples)
            benchmark::DoNotOptimize(v);
    }

    state.counters["Count"] = {static_cast<double>(state.iterations() * samples.size()), benchmark::Counter::kIsRate};
}

BENCHMARK_CAPTURE(distribution, erand, erand_dist, xo_256p);
BENCHMARK_CAPTURE(distribution, norm_marsaglia, norm_dist_marsaglia, xo_256p);
BENCHMARK_CAPTURE(distribution, norm_box_muller, norm_dist_box_muller, xo_256p);

template<typename Fn, typename Gen>
static void vector(benchmark::State& state, Fn&& fn, Gen& gen) {
    using T = std::decay_t<decltype(std::invoke(fn, gen))>;
    std::array<T, 16> samples;

    for (auto _ : state) {
        std::generate(samples.begin(), samples.end(), [&]() { return std::invoke(fn, gen); });
        for (auto& v : samples)
            benchmark::DoNotOptimize(v);
    }

    state.counters["Count"] = {static_cast<double>(state.iterations() * samples.size()), benchmark::Counter::kIsRate};
}

// clang-format off

BENCHMARK_CAPTURE(vector, s1_polar, stf::random::sphere_sampler<1>::polar<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, s1_rejection, stf::random::sphere_sampler<1>::rejection<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, s1_normal, stf::random::sphere_sampler<1>::normal<double, decltype(xo_256p)>, xo_256p);

BENCHMARK_CAPTURE(vector, s2_polar_trig, stf::random::sphere_sampler<2>::polar_trig<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, s2_polar, stf::random::sphere_sampler<2>::polar<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, s2_normal, stf::random::sphere_sampler<2>::normal<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, s2_rejection_marsaglia, stf::random::sphere_sampler<2>::rejection_marsaglia<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, s2_rejection_cook, stf::random::sphere_sampler<2>::rejection_cook<double, decltype(xo_256p)>, xo_256p);

BENCHMARK_CAPTURE(vector, b2_rejection, stf::random::ball_sampler<2>::rejection<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, b2_radius, stf::random::ball_sampler<2>::radius<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, b2_radial, stf::random::ball_sampler<2>::radial<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, b2_concentric, stf::random::ball_sampler<2>::concentric<double, decltype(xo_256p)>, xo_256p);

BENCHMARK_CAPTURE(vector, b3_rejection, stf::random::ball_sampler<3>::rejection<double, decltype(xo_256p)>, xo_256p);
BENCHMARK_CAPTURE(vector, b3_radius, stf::random::ball_sampler<3>::radius<double, decltype(xo_256p)>, xo_256p);

// clang-format on
