#include <benchmark/benchmark.h>

#include <stuff/qoi.hpp>

#include "../test/test_images.hpp"

static void decode(benchmark::State& state, std::span<const uint8_t> qoi) {
    stf::qoi::image image{};

    uint64_t pixels_processed = 0;
    uint64_t images_processed = 0;

    for (auto _ : state) {
        auto res = image.from_memory(qoi);
        benchmark::DoNotOptimize(res);
        benchmark::DoNotOptimize(image);

        pixels_processed += image.size();
        ++images_processed;

        // benchmark::ClobberMemory();
    }

    state.counters["Pixels"] = {static_cast<double>(pixels_processed), benchmark::Counter::kIsRate};
    state.counters["Frames"] = {static_cast<double>(images_processed), benchmark::Counter::kIsRate};
}

static void encode(benchmark::State& state, std::span<const uint8_t> original_qoi, double tolerance = 0.) {
    stf::qoi::image image{};
    if ( auto res = image.from_memory(original_qoi);!res) {
        return;
    }

    std::vector<uint8_t> out {};

    uint64_t pixels_processed = 0;
    uint64_t images_processed = 0;

    for (auto _ : state) {
        auto res = image.to_memory(std::back_inserter(out), tolerance);

        benchmark::DoNotOptimize(res);
        benchmark::DoNotOptimize(out);
        benchmark::DoNotOptimize(image);

        pixels_processed += image.size();
        ++images_processed;

        // benchmark::ClobberMemory();
    }

    state.counters["Pixels"] = {static_cast<double>(pixels_processed), benchmark::Counter::kIsRate};
    state.counters["Frames"] = {static_cast<double>(images_processed), benchmark::Counter::kIsRate};
}

BENCHMARK_CAPTURE(decode, kodim10, stf::qoi::test_images::kodim10_qoi);
BENCHMARK_CAPTURE(decode, kodim23, stf::qoi::test_images::kodim23_qoi);
BENCHMARK_CAPTURE(encode, kodim10, stf::qoi::test_images::kodim10_qoi);
BENCHMARK_CAPTURE(encode, kodim23, stf::qoi::test_images::kodim23_qoi);
BENCHMARK_CAPTURE(encode, kodim10_3, stf::qoi::test_images::kodim10_qoi, 3.);
BENCHMARK_CAPTURE(encode, kodim23_3, stf::qoi::test_images::kodim23_qoi, 3.);

BENCHMARK_MAIN();
