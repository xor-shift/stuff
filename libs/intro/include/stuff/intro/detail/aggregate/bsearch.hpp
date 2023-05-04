#pragma once

#include <stuff/core/integers.hpp>

#include <type_traits>

namespace stf::intro::detail::agg {

template<template<usize> typename Predicate, usize Left, usize Right>
struct binary_search;

template<template<usize> typename Predicate, usize Left, usize Right>
struct binary_search_iterator {
    using left_search_type = binary_search<Predicate, Left, (Left + Right) / 2>;
    using right_search_type = binary_search<Predicate, (Left + Right) / 2, Right>;

    using search_type = std::conditional_t<Predicate<(Left + Right) / 2>::value, right_search_type, left_search_type>;
};

template<template<usize> typename Predicate, usize Left, usize Right>
struct binary_search
    : std::conditional_t<
        Right - Left <= 1uz,
        std::type_identity<std::integral_constant<usize, Left>>,
        typename binary_search_iterator<Predicate, Left, Right>::search_type> {};

template<template<usize> typename Predicate, usize Left, usize Right>
inline constexpr usize binary_search_v = binary_search<Predicate, Left, Right>::value;

}
