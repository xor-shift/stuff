#pragma once

#include <stuff/cemath/bit.hpp>
#include <stuff/core/integers.hpp>

#include <concepts>
#include <limits>
#include <type_traits>

namespace stf::cemath {

enum class classification {
    infinite,
    nan,
    normal,
    subnormal,
    zero,
};

}

namespace stf::cemath::concepts {

template<typename T>
concept soft_float =  //
  requires(T const& v, T& mut) {
      typename T::repr_type;

      T::from_repr(std::declval<typename T::repr_type>());
      { v.get_repr() } -> std::same_as<typename T::repr_type>;

      { v.classify() } -> std::same_as<classification>;

      { v.sign_bit() } -> std::same_as<bool>;

      { mut.abs() } -> std::same_as<T&>;
      { mut.ceil() } -> std::same_as<T&>;
      { mut.floor() } -> std::same_as<T&>;
      { mut.trunc() } -> std::same_as<T&>;
      { mut.round() } -> std::same_as<T&>;
  };

}

namespace stf::cemath {

template<std::floating_point T>
struct native_float;

template<std::floating_point T>
using native_float_t = typename native_float<T>::type;

template<>
struct native_float<float>;

template<>
struct native_float<double>;

template<>
struct native_float<long double>;

template<std::floating_point T>
constexpr auto native_to_soft_float(T v) -> concepts::soft_float auto{
    using soft_float_type = native_float_t<T>;

    typename soft_float_type::repr_type repr;

    if constexpr (std::is_same_v<T, long double> && core::has_padding_v<T>) {
        auto temp = std::bit_cast<std::array<unsigned char, sizeof(T)>>(v);

        if (std::endian::native == std::endian::little) {
            std::fill(temp.data() + 10, temp.data() + 16, 0);
        } else {
            std::fill(temp.data() + 0, temp.data() + 6, 0);
        }

        repr = std::bit_cast<decltype(repr)>(temp);
    } else {
        repr = std::bit_cast<decltype(repr)>(v);
    }

    return soft_float_type::from_repr(repr);
}

}  // namespace stf::cemath

#include <stuff/cemath/detail/soft_float/ieee.ipp>
#include <stuff/cemath/detail/soft_float/intel.ipp>
