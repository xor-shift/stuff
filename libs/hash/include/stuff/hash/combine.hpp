#pragma once

#include <stuff/core.hpp>

namespace stf {

namespace detail {

template<typename T>
struct hash_magic;

template<>
struct hash_magic<u32> {
    // "round(2^64 / ((1 + sqrt(5)) / 2)) in hex" in wolfram
    static constexpr u32 phi = 0x9e3779b9ul;
    // closest prime to phi above
    static constexpr u32 phi_prime = 0x9e370001ul;
    // the same procedure as phi but with pi
    static constexpr u32 pi = 0x243f6a88;
    // the same procedure as phi but with e
    static constexpr u32 euler = 0xb7e15162ul;
};

template<>
struct hash_magic<u64> {
    static constexpr u64 phi = 0x9e3779b97f4a7c16ull;
    static constexpr u64 phi_prime = 0x9e37fffffffc0001ull;
    static constexpr u64 pi = 0x243f6a8885a308d3ull;
    static constexpr u64 euler = 0xb7e151628aed2a6bull;
};

}  // namespace detail

constexpr auto hash_combine(usize lhs, usize rhs) -> usize {
    using sized_integer_type = std::conditional_t<  //
      sizeof(usize) == sizeof(u32), u32,       //
      std::conditional_t<
        sizeof(usize) == sizeof(u64), u64,  //
        void>>;

    static constexpr auto magic_number = detail::hash_magic<sized_integer_type>::phi;

    return lhs ^ (rhs + magic_number + (lhs << 6) + (lhs >> 2));
}

}
