#pragma once

namespace stf::blas {

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type>
    requires(X::size == 3, Y::size == 3)
constexpr auto cross(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, 3> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type>
    requires(X::size == 7, Y::size == 7)
constexpr auto cross(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, 7> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto distance(X const& p_0, Y const& p_1) -> decltype(std::sqrt(std::common_type_t<T, U>{}));

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto dot(X const& x, Y const& y) -> std::common_type_t<T, U>;

template<concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto length(X const& x) -> decltype(std::sqrt(T{}));

template<concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto normalize(X const& x) -> concepts::generic_vector<T, N> auto;

template<typename T, usize N>
constexpr auto reflect(concepts::generic_vector<T, N> auto const& i, concepts::generic_vector<T, N> auto const& n)
  -> concepts::generic_vector<T, N> auto;

template<typename T, usize N>
constexpr auto
refract(concepts::generic_vector<T, N> auto const& i, concepts::generic_vector<T, N> auto const& n, T eta)
  -> concepts::generic_vector<T, N> auto;

}  // namespace stf::blas

#include <stuff/blas/detail/generic_vec/geometric.ipp>
