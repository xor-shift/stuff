#pragma once

#include <stuff/intro/detail/aggregate/arity.hpp>

#include <type_traits>
#include <utility>

namespace stf::intro::detail::agg {

template<typename Like, typename Thing>
struct make_reference_like;

template<typename Like, typename Thing>
    requires(std::is_reference_v<Like>)
struct make_reference_like<Like, Thing> {
    inline static constexpr bool is_const =
      std::is_const_v<std::remove_reference_t<Like>> || std::is_const_v<std::remove_reference_t<Thing>>;

    inline static constexpr bool is_rvalue = std::is_rvalue_reference_v<Like> || std::is_rvalue_reference_v<Thing>;

    using rvalue_reference =
      std::conditional_t<is_const, const std::remove_reference_t<Thing>, std::remove_reference_t<Thing>>&&;
    using lvalue_reference =
      std::conditional_t<is_const, const std::remove_reference_t<Thing>, std::remove_reference_t<Thing>>&;

    using type = std::conditional_t<
      std::is_rvalue_reference_v<Like> || std::is_rvalue_reference_v<Thing>,
      rvalue_reference,
      lvalue_reference>;
};

template<typename Like, typename Thing>
    requires(!std::is_reference_v<Like>)
struct make_reference_like<Like, Thing> : make_reference_like<Like&&, Thing> {};

namespace ce_tests {

static_assert(([] constexpr->bool {
    static_assert(std::is_same_v<make_reference_like<int, int>::type, int&&>);
    static_assert(std::is_same_v<make_reference_like<int, int&>::type, int&&>);
    static_assert(std::is_same_v<make_reference_like<int, int&&>::type, int&&>);
    static_assert(std::is_same_v<make_reference_like<int, const int>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int, int const&>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int, int const&&>::type, int const&&>);

    static_assert(std::is_same_v<make_reference_like<const int, int>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<const int, int&>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<const int, int&&>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<const int, const int>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<const int, int const&>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<const int, int const&&>::type, int const&&>);

    static_assert(std::is_same_v<make_reference_like<int&, int>::type, int&>);
    static_assert(std::is_same_v<make_reference_like<int&, int&>::type, int&>);
    static_assert(std::is_same_v<make_reference_like<int&, int&&>::type, int&&>);
    static_assert(std::is_same_v<make_reference_like<int&, const int>::type, int const&>);
    static_assert(std::is_same_v<make_reference_like<int&, int const&>::type, int const&>);
    static_assert(std::is_same_v<make_reference_like<int&, int const&&>::type, int const&&>);

    static_assert(std::is_same_v<make_reference_like<int&&, int>::type, int&&>);
    static_assert(std::is_same_v<make_reference_like<int&&, int&>::type, int&&>);
    static_assert(std::is_same_v<make_reference_like<int&&, int&&>::type, int&&>);
    static_assert(std::is_same_v<make_reference_like<int&&, const int>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int&&, int const&>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int&&, int const&&>::type, int const&&>);

    static_assert(std::is_same_v<make_reference_like<int const&, int>::type, int const&>);
    static_assert(std::is_same_v<make_reference_like<int const&, int&>::type, int const&>);
    static_assert(std::is_same_v<make_reference_like<int const&, int&&>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int const&, const int>::type, int const&>);
    static_assert(std::is_same_v<make_reference_like<int const&, int const&>::type, int const&>);
    static_assert(std::is_same_v<make_reference_like<int const&, int const&&>::type, int const&&>);

    static_assert(std::is_same_v<make_reference_like<int const&&, int>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int const&&, int&>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int const&&, int&&>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int const&&, const int>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int const&&, int const&>::type, int const&&>);
    static_assert(std::is_same_v<make_reference_like<int const&&, int const&&>::type, int const&&>);

    return true;
})());

}

}  // namespace stf::intro::detail::agg

#pragma push_macro("FWD")
#undef FWD
#define FWD(_letter) static_cast<typename make_reference_like<T, decltype(_letter)>::type>(_letter)

#include "./tie_generated.ipp"

#undef FWD
#pragma pop_macro("FWD")

namespace stf::intro {

template<typename T>
constexpr auto tie_aggregate(T&& v) {
    return detail::agg::tie_helpers::f(std::forward<T>(v), std::integral_constant<usize, arity<T>>{});
}

namespace detail::agg::ct_tests {

/*static_assert(([] constexpr->bool {
    struct test_struct {
        int a;
        int& b;
        int const& c;
        int&& d;
        int const&& e;
    };
    int a = 0;
    test_struct foo{a, a, a, std::move(a), std::move(a)};
    const test_struct bar{a, a, a, std::move(a), std::move(a)};

    auto tied_foo = tie_aggregate(foo);
    static_assert(std::is_same_v<decltype(tied_foo), std::tuple<int&, int&, int const&, int&&, int const&&>>);

    auto tied_foo_rv = tie_aggregate(std::move(foo));
    static_assert(std::is_same_v<decltype(tied_foo_rv), std::tuple<int&&, int&&, int const&&, int&&, int const&&>>);

    auto tied_bar = tie_aggregate(bar);
    static_assert(std::is_same_v<
                  decltype(tied_bar), std::tuple<int const&, int const&, int const&, int const&&, int const&&>>);

    return true;
})());*/

}

}  // namespace stf::intro
