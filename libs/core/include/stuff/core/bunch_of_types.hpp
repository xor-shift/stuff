#pragma once

#include <stuff/core/integers.hpp>

namespace stf {

template<typename... Ts>
struct bunch_of_types;

// bless you, GCC
namespace detail {

template<usize I, typename... Ts>
struct nth_type_helper;

template<usize I, typename T, typename... Ts>
    requires(I < sizeof...(Ts) + 1 && I != 0)
struct nth_type_helper<I, T, Ts...> : nth_type_helper<I - 1, Ts...> {};

template<typename T, typename... Ts>
struct nth_type_helper<0, T, Ts...> {
    using type = T;
};

template<usize I, template<typename> typename Predicate, typename Types, bool = Predicate<typename Types::template nth_type<I>>::value>
struct find_type_helper;

template<usize I, template<typename> typename Predicate, bool DontCare>
struct find_type_helper<I, Predicate, bunch_of_types<>, DontCare> : std::integral_constant<usize, I> {};

template<usize I, template<typename> typename Predicate, typename T, typename... Ts>
struct find_type_helper<I, Predicate, bunch_of_types<T, Ts...>, true> : std::integral_constant<usize, I> {};

template<template<typename> typename Predicate, typename T, typename... Ts>
struct find_type_helper<sizeof...(Ts), Predicate, bunch_of_types<T, Ts...>, false> : std::integral_constant<usize, sizeof...(Ts) + 1> {};

template<template<typename> typename Predicate, typename T, typename... Ts>
struct find_type_helper<sizeof...(Ts), Predicate, bunch_of_types<T, Ts...>, true> : std::integral_constant<usize, sizeof...(Ts)> {};

template<usize I, template<typename> typename Predicate, typename T, typename... Ts>
struct find_type_helper<I, Predicate, bunch_of_types<T, Ts...>, false> : find_type_helper<I + 1, Predicate, bunch_of_types<T, Ts...>> {};

}  // namespace detail

template<typename... Ts>
    requires(sizeof...(Ts) == 0)
struct bunch_of_types<Ts...> {
    static constexpr usize size = 0;
};

template<typename T, typename... Ts>
struct bunch_of_types<T, Ts...> {
    static constexpr usize size = 1uz + sizeof...(Ts);

    template<usize I>
        requires(I < size)
    using nth_type = typename detail::nth_type_helper<I, T, Ts...>::type;

    template<template<typename> typename Predicate>
    using find_type = typename detail::find_type_helper<0, Predicate, bunch_of_types<T, Ts...>, Predicate<T>::value>::type;
};

template<typename T>
struct bunch_of_types<T> {
    static constexpr usize size = 1uz;

    template<usize I>
        requires(I == 0)
    using nth_type = T;

    template<template<typename> typename Predicate>
    using find_type = std::conditional_t<Predicate<T>::value, std::integral_constant<usize, 0>, std::integral_constant<usize, 1>>;
};

}  // namespace stf
