#pragma once

namespace stf::blas {

template<typename T, typename U, usize N>
constexpr auto equivalent(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N>;

template<typename T, typename U, usize N>
constexpr auto greater_than(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N>;

template<typename T, typename U, usize N>
constexpr auto greater_than_equal(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N>;

template<typename T, typename U, usize N>
constexpr auto inequivalent(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N>;

template<typename T, typename U, usize N>
constexpr auto less_than(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N>;

template<typename T, typename U, usize N>
constexpr auto less_than_equal(vector<T, N> const& x, vector<U, N> const& y) -> vector<bool, N>;

//

template<usize N>
constexpr auto any_of(vector<bool, N> const& x) -> bool;

template<usize N>
constexpr auto all_of(vector<bool, N> const& x) -> bool;

template<usize N>
constexpr auto operator!(vector<bool, N> const& x) -> vector<bool, N>;

//

template<usize N>
constexpr auto disjunction(vector<bool, N> const& x) -> bool {
    return any_of(x);
}

template<usize N>
constexpr auto conjunction(vector<bool, N> const& x) -> bool {
    return all_of(x);
}

}  // namespace stf::blas

#include <stuff/blas/detail/plain_vec/relational.ipp>
