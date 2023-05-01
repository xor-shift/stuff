#pragma once

namespace stf::blas {

template<concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto abs(X const& x) -> concepts::generic_vector<T, N> auto;

template<concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto ceil(X const& x) -> concepts::generic_vector<T, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto clamp(X const& x, Y const& min, Y const& max)
  -> concepts::generic_vector<std::common_type_t<T, U>, N> auto;

template<concepts::vector X, typename U, typename T = typename X::value_type, usize N = X::size>
constexpr auto clamp(X const& x, U min, U max) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto;

template<concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto floor(X const& x) -> concepts::generic_vector<T, N> auto;

template<concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto fract(X const& x) -> concepts::generic_vector<T, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto max(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
constexpr auto max(X const& x, U y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto min(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
constexpr auto min(X const& x, U y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  concepts::vector A,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  typename V = typename A::value_type,
  usize N = X::size>
    requires((X::size == Y::size) && (X::size == A::size))
constexpr auto mix(X const& x, Y const& y, A const& a) -> concepts::generic_vector<std::common_type_t<T, U, V>, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename A,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::Size)
constexpr auto mix(X const& x, Y const& y, A a) -> concepts::generic_vector<std::common_type_t<T, U, A>, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto mod(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto;

template<concepts::vector X, typename U, typename T = typename X::value_type, usize N = X::size>
constexpr auto mod(X const& x, U y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto pow(X const& x, Y const& y) -> concepts::generic_vector<decltype(std::pow(T{}, U{})), N> auto;

template<concepts::vector X, typename U, typename T = typename X::value_type, usize N = X::size>
    requires(!concepts::vector<U>)
constexpr auto pow(X const& x, U y) -> concepts::generic_vector<decltype(std::pow(T{}, U{})), N> auto;

template<concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto round(X const& x) -> concepts::generic_vector<T, N> auto;

template<concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto sign(X const& x) -> concepts::generic_vector<T, N> auto;

template<
  concepts::vector E,
  concepts::vector X,
  typename T = typename E::value_type,
  typename U = typename X::value_type,
  usize N = X::size>
    requires(E::size == X::size)
constexpr auto step(E const& edge, X const& x) -> concepts::generic_vector<U, N> auto;

template<typename T, concepts::vector X, typename U = typename X::value_type, usize N = X::size>
constexpr auto step(T edge, X const& x) -> concepts::generic_vector<U, N> auto;

template<
  concepts::vector E,
  concepts::vector X,
  typename T = typename E::value_type,
  typename U = typename X::value_type,
  usize N = E::size>
    requires(E::size == X::size)
constexpr auto smooth_step(E const& edge_0, E const& edge_1, X const& x) -> concepts::generic_vector<U, N> auto;

template<typename T, concepts::vector X, typename U = typename X::value_type, usize N = X::size>
constexpr auto smooth_step(T edge_0, T edge_1, X const& x) -> concepts::generic_vector<U, N> auto;

}  // namespace stf::blas

#include <stuff/blas/detail/generic_vec/common.ipp>
