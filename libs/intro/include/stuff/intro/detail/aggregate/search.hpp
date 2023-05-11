#pragma once

#include <stuff/core/integers.hpp>

#include <type_traits>

namespace stf::intro::detail {

template<template<usize> typename Predicate, usize Left, usize Right, bool = Right - Left <= 1uz, bool = Predicate<(Left + Right) / 2>::value>
struct binary_search;

template<template<usize> typename Predicate, usize Left, usize Right, bool DontCare>
struct binary_search<Predicate, Left, Right, true, DontCare> : std::integral_constant<usize, Left> {};

template<template<usize> typename Predicate, usize Left, usize Right>
struct binary_search<Predicate, Left, Right, false, false> : binary_search<Predicate, Left, (Left + Right) / 2> {};

template<template<usize> typename Predicate, usize Left, usize Right>
struct binary_search<Predicate, Left, Right, false, true> : binary_search<Predicate, (Left + Right) / 2, Right> {};

///

template<template<usize> typename Predicate, usize StartFrom = 0, usize UpTo = (usize)-1, bool = Predicate<StartFrom>::value>
struct forward_search;

template<template<usize> typename Predicate, usize I, usize UpTo, bool DontCare>
    requires(I >= UpTo)
struct forward_search<Predicate, I, UpTo, DontCare> : std::integral_constant<usize, UpTo> {};

template<template<usize> typename Predicate, usize I, usize UpTo>
    requires(I < UpTo)
struct forward_search<Predicate, I, UpTo, true> : std::integral_constant<usize, I> {};

template<template<usize> typename Predicate, usize I, usize UpTo>
    requires(I < UpTo)
struct forward_search<Predicate, I, UpTo, false> : forward_search<Predicate, I + 1, UpTo> {};

///

template<template<usize> typename Predicate, usize Start = 0, usize I = 0, bool = Predicate<Start + I>::value>
struct powers_of_two_search;

template<template<usize> typename Predicate, usize Start, usize I>
struct powers_of_two_search<Predicate, Start, I, true> : std::integral_constant<usize, Start + I> {};

template<template<usize> typename Predicate, usize Start, usize I>
struct powers_of_two_search<Predicate, Start, I, false> : powers_of_two_search<Predicate, Start, I ? I * 2 : 1> {};

///

template<template<usize> typename Predicate>
struct invert_predicate {
    template<usize I>
    struct predicate : std::bool_constant<!Predicate<I>::value> {};
};

}  // namespace stf::intro::detail
