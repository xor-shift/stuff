#pragma once

#include "./config.hpp"

#include <stuff/intro/detail/aggregate/initializable.hpp>
#include <stuff/intro/detail/aggregate/search.hpp>

namespace stf::intro::detail {

template<typename T>
struct initializable_predicate {
    template<usize NumArgs>
    struct predicate : std::bool_constant<initializable_n<T, NumArgs>> {};
};

template<typename T>
inline static constexpr usize faux_arity_lower_bound = forward_search<initializable_predicate<T>::template predicate>::value;

enum class upper_bound_strategy {
    sizeof_bf,
    sizeof_no_bf,
    powers_of_two,
};

template<typename T, upper_bound_strategy>
struct faux_arity_upper_bound_helper;

template<typename T>
struct faux_arity_upper_bound_helper<T, upper_bound_strategy::sizeof_bf> : std::integral_constant<usize, sizeof(T) * 8> {};

template<typename T>
struct faux_arity_upper_bound_helper<T, upper_bound_strategy::sizeof_no_bf> : std::integral_constant<usize, sizeof(T)> {};

template<typename T>
struct faux_arity_upper_bound_helper<T, upper_bound_strategy::powers_of_two>
    : powers_of_two_search<invert_predicate<initializable_predicate<T>::template predicate>::template predicate, faux_arity_lower_bound<T>> {};

template<typename T, upper_bound_strategy Strategy = upper_bound_strategy::powers_of_two>
inline static constexpr usize faux_arity_upper_bound = faux_arity_upper_bound_helper<T, Strategy>::value;

template<typename T, upper_bound_strategy UBStrategy = upper_bound_strategy::powers_of_two>
inline static constexpr usize faux_arity = binary_search<initializable_predicate<T>::template predicate, faux_arity_lower_bound<T>, faux_arity_upper_bound<T, UBStrategy>>::value;

/*template<typename T>
inline static constexpr usize faux_arity = forward_search<invert_predicate<initializable_predicate<T>::template predicate>::template predicate>::value - 1;*/

}  // namespace stf::intro::detail

#if STF_INTRO_RUN_CT_TESTS

namespace stf::intro::detail::ct_tests {

static_assert(([]() {
    static_assert(faux_arity_lower_bound<test_struct_inner> == 8);
    static_assert(faux_arity_upper_bound<test_struct_inner, upper_bound_strategy::powers_of_two> == 16);
    static_assert(faux_arity<test_struct_inner, upper_bound_strategy::powers_of_two> == 13);

    static_assert(faux_arity<test_struct, upper_bound_strategy::powers_of_two> == 15);

    return true;
})());

}  // namespace stf::intro::detail::ct_tests

#endif
