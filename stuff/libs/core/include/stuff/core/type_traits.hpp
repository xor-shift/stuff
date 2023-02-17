#pragma once

#include <concepts>

namespace stf {

template<typename T, typename U>
requires (std::is_arithmetic_v<T>) && (std::is_arithmetic_v<U>)
struct type_promotion { // TODO: find a better name
    using type = std::decay_t<decltype(std::declval<T>() + std::declval<U>())>;
};

template<typename T, typename U>
using type_promotion_t = typename type_promotion<T, U>::type;

}  // namespace stf
