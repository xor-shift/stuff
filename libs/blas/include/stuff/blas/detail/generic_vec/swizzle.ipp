#pragma once

#include <stuff/blas/detail/swizzle.ipp>

namespace stf::blas {

template<string_literal Lit, concepts::vector X, typename T, usize N>
constexpr auto swizzle(X const& vec) -> concepts::generic_vector<T, Lit.size()> auto{
    static_assert(detail::can_swizzle<Lit, X>);

    typename X::template rebind<T, Lit.size()> ret{};

    for (usize i = 0; char c : Lit) {
        usize index = detail::swizzle_index(c);
        ret[i++] = vec[index];
    }

    return ret;
}

}  // namespace stf::blas
