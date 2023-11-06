#pragma once

#include <stuff/core.hpp>

#include <limits>

namespace stf::detail {

template<usize Bits>
struct decide_medium_int_repr;

template<usize Bits>
    requires(Bits < std::numeric_limits<unsigned int>::digits)
struct decide_medium_int_repr<Bits> : std::type_identity<unsigned int> {};

template<usize Bits>
    requires(Bits > std::numeric_limits<unsigned int>::digits && Bits <= std::numeric_limits<u8>::digits)
struct decide_medium_int_repr<Bits> : std::type_identity<std::uint_fast8_t> {};

template<usize Bits>
    requires(Bits > std::numeric_limits<unsigned int>::digits && Bits <= std::numeric_limits<u16>::digits)
struct decide_medium_int_repr<Bits> : std::type_identity<std::uint_fast16_t> {};

template<usize Bits>
    requires(Bits > std::numeric_limits<unsigned int>::digits && Bits <= std::numeric_limits<u32>::digits)
struct decide_medium_int_repr<Bits> : std::type_identity<std::uint_fast32_t> {};

template<usize Bits>
    requires(Bits > std::numeric_limits<unsigned int>::digits && Bits <= std::numeric_limits<u64>::digits)
struct decide_medium_int_repr<Bits> : std::type_identity<std::uint_fast64_t> {};

}  // namespace detail
