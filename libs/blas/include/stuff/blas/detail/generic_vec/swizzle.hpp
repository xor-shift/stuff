#pragma once

namespace stf::blas {

/// GLSL-like vector swizzling.\n
/// x, r, and s correspond to index 0\n
/// y, g, and t correspond to index 1\n
/// z, b, and p correspond to index 2\n
/// w, a, and q correspond to index 3\n
/// Unlike GLSL, assignment to swizzles are not allowed\n
/// @return
/// The swizzled vector.
/// @example
/// <pre>static_assert(swizzle\<"wztsrgzw">(vector\<int, 4>{0,1,2,3}) == vector\<int, 8>{3,2,1,0,0,1,2,3})</pre>
template<string_literal Lit, concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto swizzle(X const& vec) -> concepts::generic_vector<T, Lit.size()> auto;

}  // namespace stf::blas

#include <stuff/blas/detail/generic_vec/swizzle.ipp>
