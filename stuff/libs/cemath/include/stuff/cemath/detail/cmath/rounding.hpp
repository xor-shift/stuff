#pragma once

namespace stf::cemath::detail {

template<std::floating_point T>
constexpr T ceil(T v) {
    auto res = native_to_soft_float(v).ceil();
    return std::bit_cast<T>(res.get_repr());
}

template<std::floating_point T>
constexpr T floor(T v) {
    auto res = native_to_soft_float(v).ceil();
    return std::bit_cast<T>(res.get_repr());
}

template<std::floating_point T>
constexpr T trunc(T v) {
    auto res = native_to_soft_float(v).ceil();
    return std::bit_cast<T>(res.get_repr());
}

}  // namespace stf::cemath::detail
