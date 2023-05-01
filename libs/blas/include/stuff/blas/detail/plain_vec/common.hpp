#pragma once

namespace stf::blas {

template<typename T, usize N>
constexpr auto abs(vector<T, N> const& x) -> vector<T, N>;

template<typename T, usize N>
constexpr auto ceil(vector<T, N> const& x) -> vector<T, N>;

template<typename T, typename U, usize N>
constexpr auto clamp(vector<T, N> const& x, vector<U, N> const& min, vector<U, N> const& max) -> vector<std::common_type_t<T, U>, N>;

template<typename T, typename U, usize N>
constexpr auto clamp(vector<T, N> const& x, U min, U max) -> vector<std::common_type_t<T, U>, N>;

template<typename T, usize N>
constexpr auto floor(vector<T, N> const& x) -> vector<T, N>;

template<typename T, usize N>
constexpr auto fract(vector<T, N> const& x) -> vector<T, N>;

template<typename T, typename U, usize N>
constexpr auto max(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>;

template<typename T, typename U, usize N>
constexpr auto max(vector<T, N> const& x, U y) -> vector<std::common_type_t<T, U>, N>;

template<typename T, typename U, usize N>
constexpr auto min(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>;

template<typename T, typename U, usize N>
constexpr auto min(vector<T, N> const& x, U y) -> vector<std::common_type_t<T, U>, N>;

template<typename T, usize N>
constexpr auto mix(vector<T, N> const& x, vector<T, N> const& y, vector<T, N> const& a) -> vector<T, N>;

template<typename T, usize N, typename V>
constexpr auto mix(vector<T, N> const& x, vector<T, N> const& y, V a) -> vector<T, N>;

template<typename T, usize N>
constexpr auto mod(vector<T, N> const& x, vector<T, N> const& y) -> vector<T, N>;

template<typename T, usize N>
constexpr auto mod(vector<T, N> const& x, T y) -> vector<T, N>;

template<typename T, typename U, usize N>
constexpr auto pow(vector<T, N> const& x, vector<U, N> const& y) -> vector<decltype(std::pow(T{}, U{})), N>;

template<typename T, typename U, usize N>
constexpr auto pow(vector<T, N> const& x, U y) -> vector<decltype(std::pow(T{}, U{})), N>;

template<typename T, usize N>
constexpr auto round(vector<T, N> const& x) -> vector<T, N>;

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

#include <stuff/blas/detail/plain_vec/common.ipp>
