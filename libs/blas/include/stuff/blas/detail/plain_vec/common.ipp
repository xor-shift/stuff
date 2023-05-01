#pragma once

#include <stuff/sfloat.hpp>

namespace stf::blas {

template<typename T, usize N>
constexpr auto abs(vector<T, N> const& x) -> vector<T, N> {
    return detail::vecop_unary(x, sfloat::abs);
}

static_assert(abs(vector<int, 3>{-1, 2, -3}) == vector<int, 3>{1, 2, 3});

template<typename T, usize N>
constexpr auto ceil(vector<T, N> const& x) -> vector<T, N> {
    return detail::vecop_unary(x, sfloat::ceil);
}

static_assert(ceil(vector<float, 3>{-1.2, 3.4, -5.6}) == vector<float, 3>{-1, 4, -5});

template<typename T, typename U, usize N>
constexpr auto clamp(vector<T, N> const& x, vector<U, N> const& min_val, vector<U, N> const& max_val) -> vector<std::common_type_t<T, U>, N> {
    return min(max(x, min_val), max_val);
}

template<typename T, typename U, usize N>
constexpr auto clamp(vector<T, N> const& x, U min_val, U max_val) -> vector<std::common_type_t<T, U>, N> {
    return min(max(x, min_val), max_val);
}

template<typename T, usize N>
constexpr auto floor(vector<T, N> const& x) -> vector<T, N> {
    return detail::vecop_unary(x, sfloat::floor);
}

template<typename T, usize N>
constexpr auto fract(vector<T, N> const& x) -> vector<T, N> {
    return x - floor(x);
}

template<typename T, typename U, usize N>
constexpr auto max(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N> {
    using V = std::common_type_t<T, U>;
    return detail::vecop_binary(x, y, [](V a, V b) -> V { return a < b ? b : a; });
}

template<typename T, typename U, usize N>
constexpr auto max(vector<T, N> const& x, U y) -> vector<std::common_type_t<T, U>, N> {
    using V = std::common_type_t<T, U>;
    return detail::vecop_binary(x, y, [](V a, V b) -> V { return a < b ? b : a; });
}

template<typename T, typename U, usize N>
constexpr auto min(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N> {
    using V = std::common_type_t<T, U>;
    return detail::vecop_binary(x, y, [](V a, V b) -> V { return a < b ? a : b; });
}

template<typename T, typename U, usize N>
constexpr auto min(vector<T, N> const& x, U y) -> vector<std::common_type_t<T, U>, N> {
    using V = std::common_type_t<T, U>;
    return detail::vecop_binary(x, y, [](V a, V b) -> V { return a < b ? a : b; });
}

template<typename T, usize N>
constexpr auto mix(vector<T, N> const& x, vector<T, N> const& y, vector<T, N> const& a) -> vector<T, N> {
    vector<T, N> t_0 = detail::vecop_binary(x, a, [](T v, T a) { return v * (1 - a); });
    vector<T, N> t_1 = detail::vecop_binary(y, a, [](T v, T a) { return v * a; });
    return t_0 + t_1;
}

template<typename T, usize N, typename V>
constexpr auto mix(vector<T, N> const& x, vector<T, N> const& y, V a) -> vector<T, N> {
    vector<T, N> t_0 = detail::vecop_binary(x, a, [](T v, T a) { return v * (1 - a); });
    vector<T, N> t_1 = detail::vecop_binary(y, a, [](T v, T a) { return v * a; });
    return t_0 + t_1;
}

template<typename T, usize N>
constexpr auto mod(vector<T, N> const& x, vector<T, N> const& y) -> vector<T, N>;

template<typename T, usize N>
constexpr auto mod(vector<T, N> const& x, T y) -> vector<T, N>;

template<typename T, typename U, usize N>
constexpr auto pow(vector<T, N> const& x, vector<U, N> const& y) -> vector<decltype(std::pow(T{}, U{})), N> {
    return detail::vecop_binary(x, y, [](T x, U y) { return std::pow(x, y); });
}

template<typename T, typename U, usize N>
constexpr auto pow(vector<T, N> const& x, U y) -> vector<decltype(std::pow(T{}, U{})), N> {
    return detail::vecop_binary(x, y, [](T x, U y) { return std::pow(x, y); });
}

template<typename T, usize N>
constexpr auto round(vector<T, N> const& x) -> vector<T, N> {
    return detail::vecop_unary(x, [](T v) -> T { return static_cast<T>(std::round(v)); });
}

template<typename T, usize N>
constexpr auto sign(vector<T, N> const& x) -> vector<T, N>;

template<typename T, usize N>
constexpr auto step(vector<T, N> const& edge, vector<T, N> const& x) -> vector<T, N>;

template<typename T, usize N>
constexpr auto step(T edge, vector<T, N> const& x) -> vector<T, N>;

template<typename T, usize N>
constexpr auto step(vector<T, N> const& edge_0, vector<T, N> const& edge_1, vector<T, N> const& x) -> vector<T, N>;

template<typename T, usize N>
constexpr auto step(T edge_0, T edge_1, vector<T, N> const& x) -> vector<T, N>;

}  // namespace stf::blas
