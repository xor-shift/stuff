#pragma once

#include "./config.hpp"

#include <stuff/core.hpp>

#include <stuff/intro/detail/aggregate/any.hpp>

namespace stf::intro::detail {

template<typename T>
concept aggregate = std::is_aggregate_v<T>;

template<typename T, typename... Ts>
concept initializable_with = aggregate<T> && requires(Ts&&... args) { T{std::forward<Ts>(args)...}; };

template<aggregate T, typename Types>
struct initializable_helper;

template<aggregate T, typename... Ts>
struct initializable_helper<T, stf::bunch_of_types<Ts...>>  //
    : std::bool_constant<initializable_with<T, Ts...>> {};

template<aggregate T, typename Indices>
struct initializable_indices_helper;

template<aggregate T, usize... Is>
struct initializable_indices_helper<T, std::integer_sequence<usize, Is...>>  //
    : initializable_helper<T, stf::bunch_of_types<convertible_to_anything<Is>...>> {};

template<aggregate T, usize N>
inline static constexpr bool initializable_n = initializable_indices_helper<T, std::make_integer_sequence<usize, N>>::value;

template<aggregate T, usize I = 0>
struct initialization_signature_helper;

template<aggregate T, usize I>
struct initialization_signature_helper  //
    : std::integral_constant<u64, static_cast<u64>(initializable_n<T, I>) | (initialization_signature_helper<T, I + 1>::value << 1)> {};

template<aggregate T>
struct initialization_signature_helper<T, 63>  //
    : std::integral_constant<u64, static_cast<u64>(initializable_n<T, 63>)> {};

template<aggregate T>
static constexpr u64 initialization_signature = initialization_signature_helper<T>::value;

}  // namespace stf::intro::detail

#if STF_INTRO_RUN_CT_TESTS
#    include <tuple>

namespace stf::intro::detail::ct_tests {

template<typename T>
static constexpr void assert_false() {
    static_assert(!std::is_same_v<T, T>);
}

template<std::integral T, T v>
static constexpr void assert_false() {
    static_assert(!std::is_same_v<std::integral_constant<T, v>, std::integral_constant<T, v>>);
}

struct test_struct_simple {
    int& a;
    int b;
    int& c;
    int d;
    int e[2];
};

static_assert(([]() {
    static_assert(initializable_with<test_struct_simple, int&, int, int&, int, int, int>);

    static_assert(!initializable_n<test_struct_simple, 0>);
    static_assert(!initializable_n<test_struct_simple, 1>);
    static_assert(!initializable_n<test_struct_simple, 2>);
    static_assert(initializable_n<test_struct_simple, 3>);
    static_assert(initializable_n<test_struct_simple, 4>);
    static_assert(initializable_n<test_struct_simple, 5>);
    static_assert(initializable_n<test_struct_simple, 6>);

    static_assert(initialization_signature<test_struct_simple> == 0b1111000);

    return true;
})());

struct test_struct_inner {
    int a;
    int& b;
    int const& c;
    int&& d;
    int const&& e;
    int f[2];
    int (&g)[3];
    int* h;
    int i[4];
};

static_assert(([]() {
    static_assert(!initializable_n<test_struct_inner, 0>);  // default init
    static_assert(!initializable_n<test_struct_inner, 1>);  // a
    static_assert(!initializable_n<test_struct_inner, 2>);  // b
    static_assert(!initializable_n<test_struct_inner, 3>);  // c
    static_assert(!initializable_n<test_struct_inner, 4>);  // d
    static_assert(!initializable_n<test_struct_inner, 5>);  // e
    static_assert(!initializable_n<test_struct_inner, 6>);  // f[0]
    static_assert(!initializable_n<test_struct_inner, 7>);  // f[1]
    static_assert(initializable_n<test_struct_inner, 8>);   // g
    static_assert(initializable_n<test_struct_inner, 9>);   // h
    static_assert(initializable_n<test_struct_inner, 10>);  // i[0]
    static_assert(initializable_n<test_struct_inner, 11>);  // i[1]
    static_assert(initializable_n<test_struct_inner, 12>);  // i[2]
    static_assert(initializable_n<test_struct_inner, 13>);  // i[3]
    static_assert(!initializable_n<test_struct_inner, 14>);

    static_assert(initialization_signature<test_struct_inner> == 0b11111100000000);

    return true;
})());

struct test_struct {
    test_struct_inner a;
    int b;
    std::tuple<test_struct_inner, int> c;
};

static_assert(([]() {
    using U = convertible_to_anything<>;

    static_assert(!initializable_n<test_struct, 0>);   // default init
    static_assert(!initializable_n<test_struct, 1>);   // a.a
    static_assert(!initializable_n<test_struct, 2>);   // a.b
    static_assert(!initializable_n<test_struct, 3>);   // a.c
    static_assert(!initializable_n<test_struct, 4>);   // a.d
    static_assert(!initializable_n<test_struct, 5>);   // a.e
    static_assert(!initializable_n<test_struct, 6>);   // a.f[0]
    static_assert(!initializable_n<test_struct, 7>);   // a.f[1]
    static_assert(!initializable_n<test_struct, 8>);   // a.g
    static_assert(!initializable_n<test_struct, 9>);   // a.h
    static_assert(!initializable_n<test_struct, 10>);  // a.i[0]
    static_assert(!initializable_n<test_struct, 11>);  // a.i[1]
    static_assert(!initializable_n<test_struct, 12>);  // a.i[2]
    static_assert(!initializable_n<test_struct, 13>);  // a.i[3]
    static_assert(!initializable_n<test_struct, 14>);  // b
    static_assert(initializable_n<test_struct, 15>);   // c

    static_assert(initialization_signature<test_struct> == 0b1000000000000000);

    return true;
})());

}  // namespace stf::intro::detail::ct_tests

#endif
