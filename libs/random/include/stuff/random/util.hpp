#pragma once

#include <stuff/core.hpp>
#include <stuff/random/concepts.hpp>

#include <bit>
#include <functional>
#include <span>

namespace stf::random {

template<concepts::uniform_random_bit_generator G>
constexpr auto generator_bits_per_call() -> int {
    using U = typename G::result_type;

    constexpr U range = G::max() - G::min();

    constexpr int bits =
      range == std::numeric_limits<U>::max() ? std::numeric_limits<U>::digits : (std::bit_width(range + 1) - 1);

    static_assert(bits > 0);

    return bits;
}

template<std::unsigned_integral T, concepts::uniform_random_bit_generator G>
constexpr auto get_entropy(G& generator, int bits) -> T {
    using U = typename G::result_type;

    constexpr int bits_per_call = std::min(generator_bits_per_call<G>(), std::numeric_limits<T>::digits);
    constexpr U mask = bits_per_call == std::numeric_limits<T>::digits ? static_cast<U>(static_cast<T>(-1))
                                                                       : ((static_cast<U>(1) << bits_per_call) - 1);


    assume(bits <= std::numeric_limits<T>::digits);
    assume(bits_per_call <= std::numeric_limits<T>::digits);
    assume(bits_per_call <= std::numeric_limits<U>::digits);

    if (bits_per_call >= std::numeric_limits<T>::digits) { // supress a warning
        U gen = generator() >> (bits_per_call - bits);
        return static_cast<T>(gen);
    }

    int num_calls = bits / bits_per_call;
    bool have_excess = (bits % bits_per_call) != 0;

    T ret = 0;

    for (int i = 0; i < num_calls; i++) {
        ret <<= bits_per_call;
        U gen = generator();
        ret |= static_cast<T>(gen & mask);
    }

    if (have_excess) {
        int excess_bits = bits - num_calls * bits_per_call;
        ret <<= excess_bits;
        U gen = generator();
        ret |= static_cast<T>(gen & mask) >> (bits_per_call - excess_bits);
    }

    return ret;
}

template<std::unsigned_integral T, size_t N, typename Fn>
constexpr void jump_f2_linear(std::span<T, N> state, std::span<const T, N> polynomial, Fn&& permuter) {
    // TODO: study:
    // https://github.com/peteroupc/peteroupc.github.io/blob/master/jump.md
    // https://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf

    std::array<T, N> s{0};

    for (T v : polynomial) {
        for (int b = 0; b < std::numeric_limits<T>::digits; b++) {
            if (v & (T(1) << b)) {
                for (usize i = 0; i < N; i++) {
                    s[i] ^= state[i];
                }
            }

            std::invoke(std::forward<Fn>(permuter));
        }
    }

    std::copy_n(s.begin(), N, state.begin());
}

}  // namespace stf::random
