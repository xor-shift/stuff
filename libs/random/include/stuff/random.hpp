#pragma once

#include <stuff/random/concepts.hpp>
#include <stuff/random/util.hpp>

#include <stuff/random/dist/erand48.hpp>
#include <stuff/random/dist/normal.hpp>
#include <stuff/random/engine/splitmix.hpp>
#include <stuff/random/engine/xoroshiro128.hpp>
#include <stuff/random/engine/xoshiro256.hpp>

#include <random>

namespace stf::random {

// https://en.wikipedia.org/wiki/Linear_congruential_generator#Parameters_in_common_use

// the same LCG POSIX erand48 family of functions use. generates 48 bits of entropy per call.
using erand_double_generator = std::linear_congruential_engine<u64, 0x5DEECE66Dull, 11ull, 1ull << 48>;

// this is not related to erand48 but can be used to generate floats in a similar vain.
using erand_float_generator = std::minstd_rand;

using xoshiro_float_generator = std::conditional_t<std::is_same_v<ufast32, u64>, xoshiro_256p, xoroshiro_128p>;
using xoshiro_double_generator = xoshiro_256p;

template<concepts::random_number_engine E = xoshiro_float_generator>
constexpr auto get_float(E& e, bool include_zero = true) -> float {
    return erand48_distribution<float>{{include_zero}}(e);
}

template<concepts::random_number_engine E = xoshiro_float_generator>
constexpr auto get_double(E& e, bool include_zero = true) -> float {
    return erand48_distribution<double>{{include_zero}}(e);
}

}  // namespace stf::random
