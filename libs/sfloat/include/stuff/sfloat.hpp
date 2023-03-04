#pragma once

#include <stuff/core/integers.hpp>
#include <stuff/sfloat/ieee.hpp>
#include <stuff/sfloat/soft_float.hpp>

#include <concepts>

namespace stf::sfloat {

template<std::floating_point T>
struct native_float;

template<std::floating_point T>
using native_float_t = typename native_float<T>::type;

template<std::floating_point T>
constexpr auto make_soft(T v) -> concepts::soft_float auto {
    using sf_type = native_float_t<T>;

    auto temp = std::bit_cast<std::array<unsigned char, sizeof(T)>>(v);

    if constexpr (std::endian::native == std::endian::big) {
        std::fill_n(temp.begin(), sf_type::padding_bytes, 0);
    } else {
        std::fill_n(temp.rbegin(), sf_type::padding_bytes, 0);
    }

    return sf_type(std::bit_cast<typename sf_type::repr_type>(temp));
}

}  // namespace stf::sfloat
