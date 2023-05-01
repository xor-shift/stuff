#pragma once

namespace stf::blas {

template<typename T, typename U, usize N>
constexpr auto equivalent(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N> {
    return detail::vecop_binary(x, y, [](T x, U y) { return x == y; });
}

template<typename T, typename U, usize N>
constexpr auto greater_than(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N> {
    return detail::vecop_binary(x, y, [](T x, U y) { return x > y; });
}

template<typename T, typename U, usize N>
constexpr auto greater_than_equal(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N> {
    return detail::vecop_binary(x, y, [](T x, U y) { return x >= y; });
}

template<typename T, typename U, usize N>
constexpr auto inequivalent(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N> {
    return detail::vecop_binary(x, y, [](T x, U y) { return x != y; });
}

template<typename T, typename U, usize N>
constexpr auto less_than(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N> {
    return detail::vecop_binary(x, y, [](T x, U y) { return x < y; });
}

template<typename T, typename U, usize N>
constexpr auto less_than_equal(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N> {
    return detail::vecop_binary(x, y, [](T x, U y) { return x <= y; });
}

//

template<usize N>
constexpr auto any_of(vector<bool, N> const& x) -> bool {
    return std::any_of(x.data(), x.data() + x.size(), std::identity{});
}

template<usize N>
constexpr auto all_of(vector<bool, N> const& x) -> bool {
    return std::all_of(x.data(), x.data() + x.size(), std::identity{});
}

template<usize N>
constexpr auto operator!(vector<bool, N> const& x) -> vector<bool, N> {
    return detail::vecop_unary(x, std::logical_not{});
}

}  // namespace stf::blas
