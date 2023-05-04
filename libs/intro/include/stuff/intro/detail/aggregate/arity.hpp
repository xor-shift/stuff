#pragma once

#include <stuff/intro/detail/aggregate/member_length.hpp>

namespace stf::intro::detail::agg {

namespace detail {

template<typename T, usize Count = 0, usize AtIndex = 0>
struct member_counter;

template<typename T, usize Count, usize AtIndex>
    requires(AtIndex != faux_arity_v<T>)
struct member_counter<T, Count, AtIndex> : member_counter<T, Count + 1, AtIndex + member_length_v<T, AtIndex>> {};

template<typename T, usize Count, usize AtIndex>
    requires(AtIndex == faux_arity_v<T>)
struct member_counter<T, Count, AtIndex> : std::integral_constant<usize, Count> {};

}  // namespace detail

}

namespace stf::intro {

/// Gets the arity of an aggregate.
template<typename T>
inline constexpr usize arity_v = detail::agg::detail::member_counter<std::remove_cvref_t<T>, 0, 0>::value;

}

namespace stf::intro::detail::agg::ct_tests {

static_assert(([] constexpr->bool {
    struct Anon {
        int& a;
        int b[2];
        int c;
        int d;
    };

    static_assert(detail::member_counter<Anon, 0, 0>::value == 4);
    static_assert(arity_v<Anon> == 4);

    return true;
})());

}
