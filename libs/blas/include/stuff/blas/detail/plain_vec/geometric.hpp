#pragma once

namespace stf::blas {

template<typename T, typename U>
constexpr auto cross(vector<T, 3> const& x, vector<U, 3> const& y) -> vector<std::common_type_t<T, U>, 3>;

template<typename T, typename U>
constexpr auto cross(vector<T, 7> const& x, vector<U, 7> const& y) -> vector<std::common_type_t<T, U>, 7>;

template<typename T, typename U, usize N>
constexpr auto distance(vector<T, N> const& p_0, vector<U, N> const& p_1) -> std::common_type_t<T, U>;

template<typename T, typename U, usize N>
constexpr auto dot(vector<T, N> const& x, vector<U, N> const& y) -> std::common_type_t<T, U>;

template<typename T, typename U, typename V, usize N>
constexpr auto faceforward(vector<T, N> const& n, vector<U, N> const& i, vector<V, N> const& n_ref)
  -> vector<std::common_type_t<T, U, V>, N>;

template<typename T, usize N>
constexpr auto length(vector<T, N> const& x) -> T;

template<typename T, usize N>
constexpr auto normalize(vector<T, N> const& x) -> vector<T, N>;

template<typename T, typename U, usize N>
constexpr auto reflect(vector<T, N> const& i, vector<U, N> const& n) -> vector<std::common_type_t<T, U>, N>;

template<typename T, typename U, typename V, usize N>
constexpr auto refract(vector<T, N> const& i, vector<U, N> const& n, V eta) -> vector<T, N>;

}  // namespace stf::blas

#include <stuff/blas/detail/plain_vec/geometric.ipp>
