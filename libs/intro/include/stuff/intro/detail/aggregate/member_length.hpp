#pragma once

#include "./config.hpp"

#include <stuff/intro/detail/aggregate/initializable_multipart.hpp>

namespace stf::intro::detail {

template<aggregate T, usize Index>
inline static constexpr bool member_is_initializer_list = initializable_n_multipart<T, Index, 2, faux_arity<T> - 1 - Index>;

#if STF_INTRO_RUN_CT_TESTS
namespace ct_tests {

static_assert(([]() {
    static_assert(!member_is_initializer_list<has_il, 0>);
    static_assert(!member_is_initializer_list<has_il, 1>);
    static_assert(member_is_initializer_list<has_il, 2>);
    static_assert(!member_is_initializer_list<has_il, 3>);

    return true;
})());

}  // namespace ct_tests
#endif

template<aggregate T, usize Index>
struct member_length_predicate {
    template<usize Len>
    inline static constexpr usize rest = Index + Len >= faux_arity<T> ? 0 : faux_arity<T> - Index - Len;

    template<usize Len>
    struct predicate : std::bool_constant<initializable_n_multipart<T, Index, Len, rest<Len>>> {};
};

template<aggregate T, usize Index>
struct member_length_helper;

/*template<aggregate T, usize Index>
    requires member_is_initializer_list<T, Index>
struct member_length_helper<T, Index> : std::integral_constant<usize, 1> {};*/

template<aggregate T, usize Index>
    requires(!member_is_initializer_list<T, Index>)
struct member_length_helper<T, Index> {
    inline static constexpr usize upto = faux_arity<T> - Index + 1;
    inline static constexpr usize raw = forward_search<member_length_predicate<T, Index>::template predicate, 2, upto>::value;
    inline static constexpr usize value = raw == upto ? 1 : raw;
};

template<aggregate T, usize Index>
static constexpr usize member_length = member_length_helper<T, Index>::value;

#if STF_INTRO_RUN_CT_TESTS
namespace ct_tests {

static_assert(([]() {
    static_assert(member_length_predicate<test_struct_simple, 4>::predicate<2>::value);
    static_assert(member_length_helper<test_struct_simple, 4>::upto == 3);
    static_assert(member_length_helper<test_struct_simple, 4>::raw == 2);
    static_assert(member_length<test_struct_simple, 0> == 1);
    static_assert(member_length<test_struct_simple, 1> == 1);
    static_assert(member_length<test_struct_simple, 2> == 1);
    static_assert(member_length<test_struct_simple, 3> == 1);
    static_assert(member_length<test_struct_simple, 4> == 2);

    static_assert(faux_arity<test_struct_inner> == 13);
    static_assert(!member_length_predicate<test_struct_inner, 9>::predicate<2>::value);
    static_assert(!member_length_predicate<test_struct_inner, 9>::predicate<3>::value);
    static_assert(member_length_predicate<test_struct_inner, 9>::predicate<4>::value);
    static_assert(!initializable_n_multipart<test_struct_inner, 9, 2, 2>);
    static_assert(!initializable_n_multipart<test_struct_inner, 9, 3, 1>);
    static_assert(initializable_n_multipart<test_struct_inner, 9, 4, 0>);
    static_assert(member_length<test_struct_inner, 0> == 1);
    static_assert(member_length<test_struct_inner, 1> == 1);
    static_assert(member_length<test_struct_inner, 2> == 1);
    static_assert(member_length<test_struct_inner, 3> == 1);
    static_assert(member_length<test_struct_inner, 4> == 1);
    static_assert(member_length<test_struct_inner, 5> == 2);
    static_assert(member_length<test_struct_inner, 7> == 1);
    static_assert(member_length<test_struct_inner, 8> == 1);
    static_assert(member_length<test_struct_inner, 9> == 4);

    return true;
})());

}  // namespace ct_tests
#endif

}  // namespace stf::intro::detail
