#pragma once

namespace stf::blas {

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto equivalent(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> bool { return x == y; });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto greater_than(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> bool { return x > y; });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto greater_than_equal(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> bool { return x >= y; });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto inequivalent(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> bool { return x != y; });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto less_than(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> bool { return x < y; });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto less_than_equal(X const& x, Y const& y) -> concepts::generic_vector<bool, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> bool { return x <= y; });
}

}  // namespace stf::blas
