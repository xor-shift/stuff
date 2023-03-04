#pragma once

#include <stuff/core/integers.hpp>

#include <array>
#include <bit>
#include <concepts>

namespace stf::bit {

namespace detail {

template<std::unsigned_integral T, usize nth_pattern>
constexpr auto bit_reversal_pattern() -> T {
    //                  inc shift val
    // 1010101010101010   2    2    1
    // 1100110011001100   4    4    3
    // 1111000011110000   8    8    15
    // 1111111100000000   16   16   255

    T ret{};

    usize bits = sizeof(T) * 8;

    usize shift_amount = 1 << (nth_pattern + 1);
    T or_val = (T{1} << (shift_amount / 2)) - 1;
    usize final_shift = shift_amount / 2;

    for (usize i = 0; i < bits; i += shift_amount) {
        ret <<= (shift_amount >= bits) ? bits - 1 : shift_amount;
        ret |= or_val;
    }

    return ret << final_shift;
}

template<std::unsigned_integral T>
struct reversal_steps;

template<>
struct reversal_steps<u8> : std::integral_constant<usize, 3> {};

template<>
struct reversal_steps<u16> : std::integral_constant<usize, 4> {};

template<>
struct reversal_steps<u32> : std::integral_constant<usize, 5> {};

template<>
struct reversal_steps<u64> : std::integral_constant<usize, 6> {};

template<std::unsigned_integral T>
inline static constexpr usize reversal_steps_v = reversal_steps<T>::value;

template<std::unsigned_integral T, size_t current_step = 0>
constexpr auto reverse_bits_patterns(T v) -> T {
    const size_t pattern = bit_reversal_pattern<T, current_step>();

    const auto shift = 1 << current_step;

    const auto left = v & pattern;
    const auto right = v & ~pattern;

    const auto temp = (left >> shift) | (right << shift);

    if constexpr (current_step + 1 < reversal_steps_v<T>)
        return reverse_bits_patterns<T, current_step + 1>(temp);
    return temp;
}

template<std::unsigned_integral T>
constexpr auto reverse_bits_naive(T v) -> T {
    T out{};

    for (size_t i = 0; i < sizeof(T) * 8; i++) {
        const T lsb = v & T{1};
        v >>= 1;
        out |= lsb << (sizeof(T) * 8 - i - 1);
    }

    return out;
}

}  // namespace detail

template<std::unsigned_integral T>
constexpr auto reverse_bits(T v) -> T {
    return detail::reverse_bits_patterns(v);
}

template<std::unsigned_integral T>
constexpr auto reverse_bytes(T v) -> T {
    if consteval {
        auto arr = std::bit_cast<std::array<char, sizeof(T)>>(v);
        for (usize i = 0; i < sizeof(T) / 2; i++) {
            char& a = arr[i];
            char& b = arr[sizeof(T) - i - 1];

            auto t = a;
            a = b;
            b = t;
        }
        return std::bit_cast<T>(arr);
    } else {
        auto* p = reinterpret_cast<char*>(&v);
        for (usize i = 0; i < sizeof(T) / 2; i++) {
            char& a = p[i];
            char& b = p[sizeof(T) - i - 1];

            auto t = a;
            a = b;
            b = t;
        }
        return v;
    }
}

}  // namespace stf::bit
