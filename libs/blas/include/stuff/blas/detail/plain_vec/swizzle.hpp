#pragma once

namespace stf::blas {

template<string_literal Lit, typename T, usize N>
constexpr auto swizzle(vector<T, N> const& vec) -> vector<T, Lit.size()>;

}

#include <stuff/blas/detail/plain_vec/swizzle.ipp>
