#pragma once

#include <stuff/paeno/detail/simplify.hpp>

namespace stf::paeno::detail {

template<typename T>
struct negate;
template<typename T>
using negate_t = typename negate<T>::type;

template<typename T, typename U>
struct add;

template<typename T, typename U>
using add_t = simplify_t<typename add<T, U>::type>;

template<typename T, typename U>
using sub_t = add_t<T, negate_t<U>>;

template<int I>
struct from_value;

template<int I>
using from_value_t = typename from_value<I>::type;

}  // namespace stf::paeno::detail

#include <stuff/paeno/detail/arith.ipp>
