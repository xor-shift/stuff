#pragma once

#include <stuff/blas/detail/swizzle.ipp>

namespace stf::blas {

template<string_literal Lit, typename T, usize N>
constexpr auto swizzle(vector<T, N> const& vec) -> vector<T, Lit.size()> {
    static_assert(detail::can_swizzle<Lit, vector<T, N>>);

    vector<T, Lit.size()> ret {};

    for (usize i = 0; char c : Lit) {
        usize index = detail::swizzle_index(c);
        ret[i++] = vec[index];
    }

    return ret;
}

}  // namespace stf::blas
