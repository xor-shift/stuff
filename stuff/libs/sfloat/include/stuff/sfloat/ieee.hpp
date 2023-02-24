#pragma once

#include <stuff/core/integers.hpp>

#include <concepts>

namespace stf::sfloat::concepts {

template<typename T>
concept ieee_float_description =  //
  requires() {
      typename T::repr_type;

      // exceptions for intel EFP
      // not supported currently
      { T::has_integer_bit } -> std::convertible_to<bool>;
      { T::padding_bytes } -> std::convertible_to<size_t>;

      { T::exponent_bits } -> std::convertible_to<typename T::repr_type>;
      { T::fraction_bits } -> std::convertible_to<typename T::repr_type>;
  };

}

namespace stf::sfloat::ieee {

struct single_precision {
    using repr_type = u32;

    inline static constexpr bool has_integer_bit = false;
    inline static constexpr usize padding_bytes = 0;

    inline static constexpr repr_type exponent_bits = 8;
    inline static constexpr repr_type fraction_bits = 23;
};

struct double_precision {
    using repr_type = u64;

    inline static constexpr bool has_integer_bit = false;
    inline static constexpr usize padding_bytes = 0;

    inline static constexpr repr_type exponent_bits = 11;
    inline static constexpr repr_type fraction_bits = 52;
};

struct intel_extended_precision {
    using repr_type = u128;

    inline static constexpr bool has_integer_bit = true;
    inline static constexpr usize padding_bytes = 6;

    inline static constexpr repr_type exponent_bits = 15;
    inline static constexpr repr_type fraction_bits = 63;
};

template<concepts::ieee_float_description Desc>
struct soft_float;

}  // namespace stf::sfloat::ieee

namespace stf::sfloat {

template<std::floating_point T>
struct native_float;

template<>
struct native_float<float> : std::type_identity<ieee::soft_float<ieee::single_precision>> {};

template<>
struct native_float<double> : std::type_identity<ieee::soft_float<ieee::double_precision>> {};

// template<>
// struct native_float<long double> : std::type_identity<ieee_float<ieee::intel_extended_precision>> {};

}  // namespace stf::sfloat

#include <stuff/sfloat/detail/ieee.ipp>
