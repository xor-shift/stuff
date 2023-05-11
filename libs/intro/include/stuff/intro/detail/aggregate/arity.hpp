#pragma once

#include "./config.hpp"

#include <stuff/intro/detail/aggregate/member_length.hpp>

namespace stf::intro::detail {

template<aggregate T, usize Index = 0, usize Count = 0>
struct arity_helper;

template<aggregate T, usize Index, usize Count>
    requires(Index > faux_arity<T>)
struct arity_helper<T, Index, Count> {
    static_assert(!std::is_same_v<T, T>, "something went wrong");
};

template<aggregate T, usize Index, usize Count>
    requires(Index == faux_arity<T>)
struct arity_helper<T, Index, Count> : std::integral_constant<usize, Count> {};

template<aggregate T, usize Index, usize Count>
    requires(Index < faux_arity<T>)
struct arity_helper<T, Index, Count> : arity_helper<T, Index + member_length<T, Index>, Count + 1> {};

}  // namespace stf::intro::detail

namespace stf::intro {

template<typename T>
    requires(std::is_aggregate_v<std::remove_cvref_t<T>>)
inline static constexpr usize arity = detail::arity_helper<std::remove_cvref_t<T>>::value;

}

#if STF_INTRO_RUN_CT_TESTS

namespace stf::intro::detail::ct_tests {

static_assert(([]() {
    static_assert(arity<test_struct_simple> == 5);
    static_assert(arity<test_struct_inner> == 9);
    static_assert(arity<test_struct> == 3);

    return true;
})());

}

#endif
