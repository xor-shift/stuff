
#pragma once

#include <stuff/core/integers.hpp>

#include <bit>
#include <concepts>

namespace stf::bit {

template<std::unsigned_integral T>
constexpr auto reverse_bits(T v) -> T;

template<std::unsigned_integral T>
constexpr auto reverse_bytes(T v) -> T;

template<std::unsigned_integral T>
constexpr auto convert_endian(T v, std::endian from, std::endian to) -> T {
    return from == to ? v : reverse_bytes(v);
}

template<std::unsigned_integral T>
constexpr auto set_bit(T v, T idx, bool set) -> T {
    T mask = 1;
    mask <<= idx;

    return set ? v | mask : v & ~mask;
}

template<std::unsigned_integral T>
constexpr auto get_bit(T v, T idx) -> bool {
    return ((v >> idx) & 1) != 0;
}

}  // namespace stf::bit

#include <stuff/bit/detail/bit.ipp>
