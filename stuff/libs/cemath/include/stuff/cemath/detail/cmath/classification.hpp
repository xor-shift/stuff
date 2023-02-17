#pragma once

#include <stuff/cemath/detail/soft_float.hpp>

namespace stf::cemath::detail {

template<std::floating_point T>
constexpr auto fp_classify(T v) -> classification {
    return float_parts<T>::from_float(v).classify();
}

template<std::floating_point T>
constexpr auto sign_bit(T v) -> bool {
    return float_parts<T>::from_float(v).sign_bit();
}

template<std::floating_point T>
constexpr auto ordering(T lhs, T rhs) -> std::partial_ordering {
    return float_parts<T>::from_float(lhs) <=> float_parts<T>::from_float(rhs);
}

}  // namespace stf::cemath::detail
