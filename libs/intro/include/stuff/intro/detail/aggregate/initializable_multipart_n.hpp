#pragma once

#include <stuff/core/tuple.hpp>

#include <stuff/intro/detail/aggregate/any.hpp>
#include <stuff/intro/detail/aggregate/faux_arity.hpp>
#include <stuff/intro/detail/aggregate/initializable_multipart.hpp>

namespace stf::intro::detail::agg {

namespace detail {

/// Checks if <code>T</code> is initializable with values of <code>{any\<I0s>..., {any\<I1s>...}}</code>
/// \tparam Seq0 <code>std::integer_sequence\<usize, I0s...></code>
/// \tparam Seq1 <code>std::integer_sequence\<usize, I1s...></code>
template<typename T, typename Seq0, typename Seq1, typename Seq2>
struct initializable_multipart_indices;

// clang-format off
/// Checks if <code>T</code> is initializable with values of <code>{any\<I0s>..., {any\<I1s>...}}</code>
template<typename T, usize... I0s, usize... I1s, usize... I2s>
struct initializable_multipart_indices<T, std::integer_sequence<usize, I0s...>, std::integer_sequence<usize, I1s...>, std::integer_sequence<usize, I2s...>>
    : initializable_multipart<T, std::tuple<convertible_to_anything<I0s>...>, std::tuple<convertible_to_anything<I1s>...>, std::tuple<convertible_to_anything<I2s>...>> {};
// clang-format on

template<typename T, typename Seq0, typename Seq1, typename Seq2>
inline constexpr bool initializable_multipart_indices_v = initializable_multipart_indices<T, Seq0, Seq1, Seq2>::value;

}  // namespace detail

// clang-format off
/// Checks if <code>T</code> is initializable with values of <code>{any\<I0s>..., {any\<I1s>...}, ...}</code> where
/// <code>I0s</code> is a pack of <code>size_t</code> from <code>0</code> upto <code>N</code>, and <code>I1s</code> is a
/// pack of <code>size_t</code> from <code>0</code> to <code>M</code>
template<typename T, usize Index, usize Length, usize Slack>
struct initializable_multipart_n
    : detail::initializable_multipart_indices<T, std::make_integer_sequence<usize, Index>, std::make_integer_sequence<usize, Length>, std::make_integer_sequence<usize, Slack>> {};
// clang-format on

template<typename T, usize Index, usize Length, usize Slack>
inline constexpr bool initializable_multipart_n_v = initializable_multipart_n<T, Index, Length, Slack>::value;

namespace ct_tests {

static_assert(([] constexpr->bool {
    struct Anon {
        int& a;
        int b[2];
        int c;
        int d;
    };

    /*int v;
    Anon{v, {1, 2}};*/

    using nt_0 = n_tuple_t<convertible_to_anything<>, 0>;
    static_assert(std::is_same_v<nt_0, std::tuple<>>);
    using nt_1 = n_tuple_t<convertible_to_anything<>, 1>;
    static_assert(std::is_same_v<nt_1, std::tuple<convertible_to_anything<>>>);
    using nt_2 = n_tuple_t<convertible_to_anything<>, 2>;
    static_assert(std::is_same_v<nt_2, std::tuple<convertible_to_anything<>, convertible_to_anything<>>>);
    using nt_3 = n_tuple_t<convertible_to_anything<>, 3>;
    using nt_4 = n_tuple_t<convertible_to_anything<>, 4>;
    using nt_5 = n_tuple_t<convertible_to_anything<>, 5>;

    static_assert(initializable_multipart_n_v<Anon, 0, 1, 4>);
    static_assert(initializable_multipart<Anon, nt_0, nt_1, nt_4>::value);

    static_assert(!initializable_multipart_n_v<Anon, 0, 2, 3>);
    static_assert(!initializable_multipart<Anon, nt_0, nt_2, nt_3>::value);

    static_assert(initializable_multipart_n_v<Anon, 1, 1, 3>);
    static_assert(initializable_multipart<Anon, nt_1, nt_1, nt_3>::value);

    static_assert(initializable_multipart_n_v<Anon, 1, 2, 2>);
    static_assert(initializable_multipart<Anon, nt_1, nt_2, nt_2>::value);

    static_assert(!initializable_multipart_n_v<Anon, 1, 3, 1>);
    static_assert(!initializable_multipart<Anon, nt_1, nt_3, nt_1>::value);

    static_assert(initializable_multipart_n_v<Anon, 3, 1, 1>);
    static_assert(initializable_multipart<Anon, nt_3, nt_1, nt_1>::value);

    static_assert(!initializable_multipart_n_v<Anon, 3, 2, 0>);
    static_assert(!initializable_multipart<Anon, nt_3, nt_2, nt_0>::value);

    static_assert(initializable_multipart_n_v<Anon, 4, 1, 0>);
    static_assert(initializable_multipart<Anon, nt_4, nt_1, nt_0>::value);

    // clang-format off
    static_assert(initializable_multipart<Anon, std::tuple<convertible_to_anything<>>, std::tuple<>, std::tuple<convertible_to_anything<>>>::value);
    static_assert(initializable_multipart<Anon, std::tuple<convertible_to_anything<>>, std::tuple<convertible_to_anything<>>, std::tuple<convertible_to_anything<>>>::value);
    static_assert(initializable_multipart<Anon, std::tuple<convertible_to_anything<>>, std::tuple<convertible_to_anything<>, convertible_to_anything<>>, std::tuple<convertible_to_anything<>>>::value);
    static_assert(!initializable_multipart<Anon, std::tuple<convertible_to_anything<>>, std::tuple<convertible_to_anything<>, convertible_to_anything<>, convertible_to_anything<>>, std::tuple<>>::value);

    static_assert(detail::initializable_multipart_indices_v<Anon, std::make_integer_sequence<usize, 1>, std::make_integer_sequence<usize, 2>, std::make_integer_sequence<usize, 1>>);
    static_assert(!detail::initializable_multipart_indices_v<Anon, std::make_integer_sequence<usize, 1>, std::make_integer_sequence<usize, 3>, std::make_integer_sequence<usize, 0>>);
    // clang-format on

    return true;
})());

}

}  // namespace stf::intro::detail::agg
