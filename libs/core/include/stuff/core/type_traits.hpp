#pragma once

#include <stuff/core/detail/padding.hpp>

#include <concepts>

namespace stf::core {

template<typename...>
struct type_promotion;

template<typename T, typename U>
struct type_promotion<T, U> { // TODO: find a better name
    using type = std::decay_t<decltype(std::declval<T>() + std::declval<U>())>;
};

template<typename T, typename U, typename V, typename... Ts>
struct type_promotion<T, U, V, Ts...> { // TODO: find a better name
    using cur_type = std::decay_t<decltype(std::declval<T>() + std::declval<U>())>;
    using type = typename type_promotion<cur_type, V, Ts...>::type;
};

template<typename... Ts>
using type_promotion_t = typename type_promotion<Ts...>::type;

}  // namespace stf
