#pragma once

namespace stf::blas {

/// @return
/// String comparison between the binary parameters.
template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
constexpr auto operator<=>(X const& lhs, Y const& rhs) -> decltype(std::declval<T>() <=> std::declval<U>()) {
    using order_type = decltype(std::declval<T>() <=> std::declval<U>());

    for (usize i = 0; i < N; i++) {
        order_type cur = lhs[i] <=> rhs[i];
        if (cur != order_type::equivalent) {
            return cur;
        }
    }

    return order_type::equivalent;
}

/// Checks the equality of two vectors; <code>lhs</code> and <code>rhs</code>.
/// Not recommended for vectors of floating-point value_type.
template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
constexpr auto operator==(X const& lhs, Y const& rhs) -> bool {
    return (lhs <=> rhs) == std::partial_ordering::equivalent;
}

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto equivalent(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto greater_than(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto greater_than_equal(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto inequivalent(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto less_than(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto;

template<
  concepts::vector X,
  concepts::vector Y,
  typename T = typename X::value_type,
  typename U = typename Y::value_type,
  usize N = X::size>
    requires(X::size == Y::size)
constexpr auto less_than_equal(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto;

//

template<usize N>
constexpr auto any_of(concepts::generic_vector<bool, N> auto const& x) -> bool;

template<usize N>
constexpr auto all_of(concepts::generic_vector<bool, N> auto const& x) -> bool;

template<usize N>
constexpr auto operator!(concepts::generic_vector<bool, N> auto const& x) -> concepts::generic_vector<bool, N> auto;

//

template<usize N>
constexpr auto disjunction(concepts::generic_vector<bool, N> auto const& x) -> bool {
    return any_of(x);
}

template<usize N>
constexpr auto conjunction(concepts::generic_vector<bool, N> auto const& x) -> bool {
    return all_of(x);
}

}  // namespace stf::blas

#include <stuff/blas/detail/generic_vec/relational.ipp>
