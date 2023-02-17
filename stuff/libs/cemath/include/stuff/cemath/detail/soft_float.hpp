#pragma once

#include <stuff/cemath/bit.hpp>
#include <stuff/core/integers.hpp>

#include <concepts>
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
  requires(T const& v) {
      typename T::repr_type;

      T::from_repr(std::declval<typename T::repr_type>());
      { v.get_repr() } -> std::same_as<typename T::repr_type>;

      { v.classify() } -> std::same_as<classification>;

      { v.sign_bit() } -> std::same_as<bool>;

      v.abs();
      v.ceil();
      v.floor();
      v.trunc();
      v.round();
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
    return soft_float_type::from_repr(std::bit_cast<typename soft_float_type::repr_type>(v));
}

}  // namespace stf::cemath

#include <stuff/cemath/detail/soft_float/ieee.ipp>
#include <stuff/cemath/detail/soft_float/intel.ipp>
