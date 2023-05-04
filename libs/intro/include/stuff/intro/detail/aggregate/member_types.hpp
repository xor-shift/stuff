#pragma once

#include <stuff/core.hpp>

#include <stuff/intro/detail/aggregate/arity.hpp>

#include "./member_types_generated.ipp"

namespace stf::intro::detail::agg {

template<typename T>
inline auto member_types_helper_example(std::integral_constant<usize, 5>) {
    // TODO: i don't get it, is it because the return type must be deduced that this function is always executed in an
    // evaluated context? hint to future me: adding a return type does allow the static asserts to exist.

    // static_assert(std::__declval_protector<T>::__stop, "only call aggregate_member_helper in an unevaluated
    // context"); static_assert(asd<T>::value, "only call aggregate_member_helper in an unevaluated context");
    // static_assert(!std::is_same_v<T, T>, "only call aggregate_member_helper in an unevaluated context");

    std::unreachable();  // rust wouldn't like this (the function would return `!`, not stf::bunch_of_types<...>)

    auto&& [a, b, c, d, e] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e)>{};
}

namespace ct_tests {

static_assert(([] constexpr->bool {
    int v = 0;
    struct test_struct {
        int a;
        int& b;
        int const& c;
        int&& d;
        int const&& e;
    } asd{
      .a = v,
      .b = v,
      .c = v,
      .d = std::move(v),
      .e = std::move(v),
    };

    decltype(std::declval<int>()) asddfgb{std::move(v)};

    auto&& [a, b, c, d, e] = asd;

    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), int&>);
    static_assert(std::is_same_v<decltype(c), int const&>);
    static_assert(std::is_same_v<decltype(d), int&&>);
    static_assert(std::is_same_v<decltype(e), int const&&>);

    // using struct_types = decltype(aggregate_member_helper<test_struct>({}));
    using struct_types = decltype(type_helpers::f<test_struct>(std::integral_constant<usize, 5>{}));

    static_assert(std::is_same_v<struct_types::nth_type<0>, int>);
    static_assert(std::is_same_v<struct_types::nth_type<1>, int&>);
    static_assert(std::is_same_v<struct_types::nth_type<2>, int const&>);
    static_assert(std::is_same_v<struct_types::nth_type<3>, int&&>);
    static_assert(std::is_same_v<struct_types::nth_type<4>, int const&&>);

    return true;
})());

}

}  // namespace stf::intro::detail::agg

namespace stf::intro {

// clang-format off
/// Aliased to a stf::a_bunch_of_types<...> representing the types of the members of an aggregate.
template<typename T, usize N>
using member_type_t = typename decltype(detail::agg::type_helpers::f<std::remove_cvref_t<T>>(std::integral_constant<usize, arity_v<std::remove_cvref_t<T>>>{}))::template nth_type<N>;
// clang-format on

}  // namespace stf::intro

namespace stf::intro::detail::agg::ct_tests {

static_assert(([] constexpr->bool {
    struct foo {
        int& a;
        int& b;
        int c;
    };

    static_assert(initializable<foo, int&, int&, int>);
    static_assert(initializable<foo, convertible_to_anything<>, convertible_to_anything<>, convertible_to_anything<>>);
    static_assert(!initializable_n<foo, 1>);
    static_assert(initializable_n<foo, 2>);
    static_assert(initializable_n<foo, 3>);
    static_assert(!initializable_n<foo, 4>);
    static_assert(
      initializable_multipart<
        foo, std::tuple<>, std::tuple<convertible_to_anything<>>, std::tuple<convertible_to_anything<>>>::value
    );
    static_assert(
      initializable_multipart<
        foo, std::tuple<convertible_to_anything<>>, std::tuple<convertible_to_anything<>>, std::tuple<>>::value
    );
    static_assert(initializable_multipart_n<foo, 0, 1, 1>::value);
    static_assert(!initializable_multipart_n<foo, 0, 2, 0>::value);
    static_assert(initializable_multipart_n<foo, 1, 1, 1>::value);
    static_assert(!initializable_multipart_n<foo, 1, 2, 0>::value);
    static_assert(initializable_multipart_n<foo, 2, 1, 0>::value);
    static_assert(!initializable_multipart_n<foo, 2, 2, 0>::value);
    static_assert(detail::faux_arity_lower_bound<foo>() == 2);
    static_assert(detail::faux_arity_upper_bound<foo>() == 4);
    static_assert(faux_arity_v<foo> == 3);

    struct bar {
        int a;
        int& b;
        int const& c;
        int&& d;
        int const&& e;
    };

    static_assert(std::is_same_v<member_type_t<bar, 0>, int>);
    static_assert(std::is_same_v<member_type_t<bar, 1>, int&>);
    static_assert(std::is_same_v<member_type_t<bar, 2>, int const&>);
    static_assert(std::is_same_v<member_type_t<bar, 3>, int&&>);
    static_assert(std::is_same_v<member_type_t<bar, 4>, int const&&>);

    return true;
})());

}
