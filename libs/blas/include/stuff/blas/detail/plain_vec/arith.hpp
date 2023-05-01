#pragma once

namespace stf::blas {

//template<typename T, typename U, usize N>
//constexpr auto operator+(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>;

template<typename T, usize N>
constexpr auto operator-(vector<T, N> const& x) -> vector<T, N>;

template<typename T, typename U, usize N>
constexpr auto operator-(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>;

template<typename T, typename U, usize N>
constexpr auto operator*(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>;

template<typename T, typename U, usize N>
constexpr auto operator*(vector<T, N> const& x, U y) -> vector<std::common_type_t<T, U>, N>
    requires requires { x[0] * y; };

template<typename T, typename U, usize N>
constexpr auto operator*(T x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>
    requires requires { x * y[0]; };

template<typename T, typename U, usize N>
constexpr auto operator/(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>;

template<typename T, typename U, usize N>
constexpr auto operator/(vector<T, N> const& x, U y) -> vector<std::common_type_t<T, U>, N>
    requires requires { x[0] / y; };

template<typename T, typename U, usize N>
constexpr auto operator/(T x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>
    requires requires { x / y[0]; };

}  // namespace stf::blas

#include <stuff/blas/detail/plain_vec/arith.ipp>
