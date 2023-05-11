#pragma once

#include "./config.hpp"

#include <stuff/intro/detail/aggregate/faux_arity.hpp>

namespace stf::intro::detail {

template<aggregate T, typename Ts0, typename Ts1, typename Ts2>
struct initializable_with_multipart_helper;

template<aggregate T, typename... Ts0, typename... Ts1, typename... Ts2>
    requires(sizeof...(Ts1) != 0)
struct initializable_with_multipart_helper<T, stf::bunch_of_types<Ts0...>, stf::bunch_of_types<Ts1...>, stf::bunch_of_types<Ts2...>> {
    inline static constexpr bool value =  //
      requires(Ts0 && ... ts_0, Ts1&&... ts_1, Ts2&&... ts_2) {
          T{std::forward<Ts0>(ts_0)..., {std::forward<Ts1>(ts_1)...}, std::forward<Ts2>(ts_2)...};
      };
};

template<aggregate T, typename... Ts0, typename T1, typename... Ts2>
struct initializable_with_multipart_helper<T, stf::bunch_of_types<Ts0...>, stf::bunch_of_types<T1>, stf::bunch_of_types<Ts2...>> {
    inline static constexpr bool value =  //
      requires(Ts0 && ... ts_0, T1&& t_1, Ts2&&... ts_2) {
          T{std::forward<Ts0>(ts_0)..., std::forward<T1>(t_1), std::forward<Ts2>(ts_2)...};
      };
};

template<aggregate T, typename Ts0, typename Ts1, typename Ts2>
inline static constexpr bool initializable_with_multipart = initializable_with_multipart_helper<T, Ts0, Ts1, Ts2>::value;

template<aggregate T, typename Is0, typename Is1, typename Is2>
struct initializable_indices_multipart_helper;

template<aggregate T, usize... Is0, usize... Is1, usize... Is2>
struct initializable_indices_multipart_helper<  //
  T,
  std::integer_sequence<usize, Is0...>,
  std::integer_sequence<usize, Is1...>,
  std::integer_sequence<usize, Is2...>>
    : initializable_with_multipart_helper<
        T,
        stf::bunch_of_types<convertible_to_anything<Is0>...>,
        stf::bunch_of_types<convertible_to_anything<Is1>...>,
        stf::bunch_of_types<convertible_to_anything<Is2>...>> {};

template<aggregate T, typename Is0, typename Is1, typename Is2>
inline static constexpr bool initializable_indices_multipart = initializable_indices_multipart_helper<T, Is0, Is1, Is2>::value;

template<aggregate T, usize Prev, usize Cur, usize Rest>
inline static constexpr bool initializable_n_multipart = initializable_indices_multipart<  //
  T,
  std::make_integer_sequence<usize, Prev>,
  std::make_integer_sequence<usize, Cur>,
  std::make_integer_sequence<usize, Rest>>;

#if STF_INTRO_RUN_CT_TESTS
namespace ct_tests {

struct has_il {
    int a;
    int& b;
    std::initializer_list<int> c;
    int d;
};

static_assert(([]() {
    static_assert(initializable_n_multipart<test_struct_simple, 0, 1, 5>);
    static_assert(!initializable_n_multipart<test_struct_simple, 0, 2, 4>);
    static_assert(initializable_n_multipart<test_struct_simple, 1, 1, 4>);
    static_assert(!initializable_n_multipart<test_struct_simple, 1, 2, 3>);
    static_assert(initializable_n_multipart<test_struct_simple, 2, 1, 3>);
    static_assert(!initializable_n_multipart<test_struct_simple, 2, 2, 2>);
    static_assert(initializable_n_multipart<test_struct_simple, 3, 1, 2>);
    static_assert(!initializable_n_multipart<test_struct_simple, 3, 2, 1>);
    static_assert(initializable_n_multipart<test_struct_simple, 4, 1, 1>);
    static_assert(initializable_n_multipart<test_struct_simple, 4, 2, 0>);
    static_assert(!initializable_n_multipart<test_struct_simple, 4, 3, 0>);

    static_assert(initialization_signature<has_il> == 0b11100);
    static_assert(faux_arity<has_il> == 4);

    static_assert(!initializable_n_multipart<has_il, 0, 2, 3>);  // il-check
    static_assert(initializable_n_multipart<has_il, 0, 1, 3>);   // check 1
    static_assert(!initializable_n_multipart<has_il, 0, 2, 2>);  // check 2
    static_assert(!initializable_n_multipart<has_il, 1, 2, 2>);  // il-check
    static_assert(initializable_n_multipart<has_il, 1, 1, 2>);   // check 1
    static_assert(!initializable_n_multipart<has_il, 1, 2, 1>);  // check 2
    static_assert(initializable_n_multipart<has_il, 2, 2, 1>);   // il-check
    static_assert(!initializable_n_multipart<has_il, 3, 2, 0>);  // il-check
    static_assert(initializable_n_multipart<has_il, 3, 1, 0>);   // check 1
    static_assert(!initializable_n_multipart<has_il, 3, 2, 0>);  // check 2

    return true;
})());

}  // namespace ct_tests
#endif

}  // namespace stf::intro::detail
