#pragma once

#include <stuff/intro/detail/aggregate/any.hpp>
#include <stuff/intro/detail/aggregate/initializable.hpp>

namespace stf::intro::detail::agg {

namespace detail {

template<typename T, usize... Is>
concept initializable_indices =  //
  requires { T{convertible_to_anything<Is>{}...}; };

template<typename T, typename Sequence>
struct initializable_sequence_helper;

template<typename T, usize... Is>
struct initializable_sequence_helper<T, std::integer_sequence<usize, Is...>>
    : std::bool_constant<initializable_indices<T, Is...>> {};

}  // namespace detail

/// Determines whether an aggregate <code>T</code> can be aggregate-initialized with <code>N</code> arguments.\n
/// Keep brace elision in mind, <code>struct { int a[2]; }</code> can be initialized with <code>N = 2</code>.\n
/// <code>N = 0</code> determines default-construct-ability.
template<typename T, usize N>
concept initializable_n = detail::initializable_sequence_helper<T, std::make_integer_sequence<usize, N>>::value;

namespace ct_tests {

static_assert(([] constexpr->bool {
    struct Anon {
        int& a;
        int b[2];
        int c;
        int d;
    };

    struct Anon2 {
        int b[2];
        int c;
        int d;
    };

    /*int v;
    Anon{v, {}, 3, 4};
    Anon{v, {1}, 3, 4};
    Anon{v, {1, 2}, 3, 4};
    Anon{v, 1, 2, 3, 4};*/
    // Anon{v, {1, 2, -1}, 3, 4};

    // clang-format off
    static_assert(!initializable<Anon>);
    static_assert(initializable<Anon2>); // default init-able
    static_assert(initializable<Anon, convertible_to_anything<>>);
    static_assert(initializable<Anon, convertible_to_anything<>, convertible_to_anything<>>);
    static_assert(initializable<Anon, convertible_to_anything<>, convertible_to_anything<>,convertible_to_anything<>>);
    static_assert(initializable<Anon, convertible_to_anything<>, convertible_to_anything<>,convertible_to_anything<>, convertible_to_anything<>>);
    static_assert(initializable<Anon, convertible_to_anything<>, convertible_to_anything<>,convertible_to_anything<>, convertible_to_anything<>,convertible_to_anything<>>);
    static_assert(!initializable<Anon, convertible_to_anything<>, convertible_to_anything<>,convertible_to_anything<>, convertible_to_anything<>,convertible_to_anything<>, convertible_to_anything<>>);
    // clang-format on

    static_assert(!detail::initializable_indices<Anon>);
    static_assert(detail::initializable_indices<Anon2>);
    static_assert(detail::initializable_indices<Anon, 0>);
    static_assert(detail::initializable_indices<Anon, 0, 1>);
    static_assert(detail::initializable_indices<Anon, 0, 1, 2>);
    static_assert(detail::initializable_indices<Anon, 0, 1, 2, 3>);
    static_assert(detail::initializable_indices<Anon, 0, 1, 2, 3, 4>);
    static_assert(!detail::initializable_indices<Anon, 0, 1, 2, 3, 4, 5>);

    static_assert(!initializable_n<Anon, 0>);
    static_assert(initializable_n<Anon2, 0>);
    static_assert(initializable_n<Anon, 1>);
    static_assert(initializable_n<Anon, 2>);
    static_assert(initializable_n<Anon, 3>);
    static_assert(initializable_n<Anon, 4>);
    static_assert(initializable_n<Anon, 5>);
    static_assert(!initializable_n<Anon, 6>);

    return true;
})());

}

}  // namespace stf::intro::detail::agg
