#pragma once

#include <stuff/intro/detail/aggregate/member_length.hpp>

namespace stf::intro::detail::agg {

namespace detail {

template<typename T, usize Index>
struct member_length_sum_helper;

template<typename T>
struct member_length_sum_helper<T, 0> : std::integral_constant<usize, member_length_v<T, 0>> {};

template<typename T, usize Index>
struct member_length_sum_helper {
    inline static constexpr usize prev = member_length_sum_helper<T, Index - 1>::value;
    inline static constexpr usize value = prev + member_length_v<T, prev>;
};

}  // namespace detail

namespace ct_tests {

static_assert(([] constexpr->bool {
    struct Anon {
        int& a;
        int b[2];
        int c;
        int d;
    };

    static_assert(detail::member_length_sum_helper<Anon, 0>::value == 1);
    static_assert(detail::member_length_sum_helper<Anon, 1>::value == 3);
    static_assert(detail::member_length_sum_helper<Anon, 2>::value == 4);
    static_assert(detail::member_length_sum_helper<Anon, 3>::value == 5);

    return true;
})());

}

}  // namespace stf::intro::detail::agg
