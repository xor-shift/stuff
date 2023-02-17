#pragma once

#include <stuff/paeno/types.hpp>

#include <stuff/paeno/detail/arith.hpp>
#include <stuff/paeno/detail/simplify.hpp>

namespace stf::paeno {

using detail::add_t;
using detail::from_value_t;
using detail::negate_t;
using detail::simplify_t;
using detail::sub_t;

constexpr auto value_of(null) -> int {
    return 0;
}

template<typename T>
constexpr auto value_of(succ<T>) -> int {
    return value_of(T{}) + 1;
}

template<typename T>
constexpr auto value_of(pred<T>) -> int {
    return value_of(T{}) - 1;
}

}  // namespace stf::paeno
