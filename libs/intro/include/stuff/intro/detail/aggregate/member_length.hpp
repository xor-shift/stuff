#pragma once

#include <stuff/intro/detail/aggregate/initializable_multipart_n.hpp>

namespace stf::intro::detail::agg {

namespace detail {

template<typename T, usize Index>
struct member_length_predicate {
    template<usize Length>
    struct predicate
        : std::bool_constant<initializable_multipart_n_v<
            T,
            Index,
            Length,
            // (>= is needed because > makes the parser think that we're concluding the template parameters)
            Index + Length + 1 >= faux_arity_v<T> ? 0 : faux_arity_v<T> - Index - Length>> {};
};

}  // namespace detail

/// \tparam N
/// The index to check (should be the start of a member)
template<typename T, usize Index>
inline constexpr usize member_length_v =
  binary_search<detail::member_length_predicate<T, Index>::template predicate, 1, faux_arity_v<T>>::type::value;

namespace ct_tests {

static_assert(([] constexpr->bool {
    struct Anon {
        int& a;
        int b[2];
        int c;
        int d;
    };

    // illustration for the linear search for the member lengths

    static_assert(detail::member_length_predicate<Anon, 0>::predicate<1>::value);
    // member at idx 0 is 1 long (2 -> false)
    static_assert(!detail::member_length_predicate<Anon, 0>::predicate<2>::value);
    static_assert(detail::member_length_predicate<Anon, 1>::predicate<1>::value);
    static_assert(detail::member_length_predicate<Anon, 1>::predicate<2>::value);
    // member at idx 1 is 2 long (3 -> false)
    static_assert(!detail::member_length_predicate<Anon, 1>::predicate<3>::value);
    static_assert(detail::member_length_predicate<Anon, 3>::predicate<1>::value);
    // member at idx 3 is 1 long (2 -> false)
    static_assert(!detail::member_length_predicate<Anon, 3>::predicate<2>::value);
    static_assert(detail::member_length_predicate<Anon, 4>::predicate<1>::value);
    // member at idx 4 is 1 long (2 -> false)
    static_assert(!detail::member_length_predicate<Anon, 4>::predicate<2>::value);
    // index 5 is >= faux_arity, terminate

    static_assert(initializable_multipart_n_v<Anon, 0, 1, 4>);

    static_assert(member_length_v<Anon, 0> == 1);
    static_assert(member_length_v<Anon, 1> == 2);
    static_assert(member_length_v<Anon, 2> == 1);
    static_assert(member_length_v<Anon, 3> == 1);

    /*static_assert(member_length_v<Anon, 0> == 0);
    static_assert(member_length_v<Anon, 1> == 2);
    static_assert(member_length_v<Anon, 2> == 1);
    static_assert(member_length_v<Anon, 3> == 1);*/

    return true;
})());

}

}  // namespace stf::intro::detail::agg
