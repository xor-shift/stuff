#pragma once

#include <stuff/core/integers.hpp>

#include <array>
#include <bit>

namespace stf::core {

namespace detail {

template<typename T>
concept no_padding =  //
  requires() {
      std::integral_constant<usize, std::bit_cast<std::array<signed char, sizeof(T)>>(T {}).size()>::value;
  };

};

template<typename T>
struct has_padding : std::bool_constant<!detail::no_padding<T>> {};

template<typename T>
inline static constexpr bool has_padding_v = has_padding<T>::value;

}
