#pragma once

#include <functional>
#include <limits>

namespace stf::blas {

// return type is based on promotion rules
template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto dot(T const& lhs, U const& rhs) -> type_promotion_t<typename T::value_type, typename U::value_type> {
    auto ret = lhs[0] * rhs[0];

    for (usize i = 1; i < T::size; i++) {
        ret += lhs[i] * rhs[i];
    }

    return ret;
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size && T::size == 3)
constexpr auto cross(T const& lhs, U const& rhs) -> concepts::nd_vector<3> auto{
    auto det_0 = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    auto det_1 = lhs[0] * rhs[2] - lhs[2] * rhs[0];
    auto det_2 = lhs[0] * rhs[1] - lhs[1] * rhs[0];

    using ret_value_type = type_promotion_t<typename T::value_type, typename U::value_type>;

    concepts::vector_backend auto ret = typename T::template rebind<ret_value_type>{};
    ret[0] = det_0;
    ret[1] = -det_1;
    ret[2] = det_2;

    return ret;
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size && T::size == 7)
constexpr auto cross(T const& x, U const& y) -> concepts::nd_vector<7> auto{
    using ret_value_type = type_promotion_t<typename T::value_type, typename U::value_type>;

    // clang-format off
#define IDK_SOME_FACTORY(a, b, c, d, e, f, g, h, i, j, k, l) \
    x[a-1]*y[b-1]-x[c-1]*y[d-1]+x[e-1]*y[f-1]-x[g-1]*y[h-1]+x[i-1]*y[j-1]-x[k-1]*y[l-1]
    // clang-format on

    concepts::vector_backend auto ret = typename T::template rebind<ret_value_type>{};
    ret[0] = IDK_SOME_FACTORY(2, 4, 4, 2, 3, 7, 7, 3, 5, 6, 6, 5);
    ret[1] = IDK_SOME_FACTORY(3, 5, 5, 3, 4, 1, 1, 4, 6, 7, 7, 6);
    ret[2] = IDK_SOME_FACTORY(4, 6, 6, 4, 5, 2, 2, 5, 7, 1, 1, 7);
    ret[3] = IDK_SOME_FACTORY(5, 7, 7, 5, 6, 3, 3, 6, 1, 2, 2, 1);
    ret[4] = IDK_SOME_FACTORY(6, 1, 1, 6, 7, 4, 4, 7, 2, 3, 3, 2);
    ret[5] = IDK_SOME_FACTORY(7, 2, 2, 7, 1, 5, 5, 1, 3, 4, 4, 3);
    ret[6] = IDK_SOME_FACTORY(1, 3, 3, 1, 2, 6, 6, 2, 4, 5, 5, 4);

#undef IDK_SOME_FACTORY

    return ret;
}

namespace detail {

template<concepts::vector T, concepts::vector U, typename Fn>
    requires(T::size == U::size)
constexpr auto elementwise_fn(T const& lhs, U const& rhs, Fn&& functor = {}) {
    // TODO: implement expression templates, maybe
    // real to-do: benchmark expression templates versus straightforward operations

    using ret_value_type = type_promotion_t<typename T::value_type, typename U::value_type>;
    concepts::vector_backend auto ret = typename T::template rebind<ret_value_type>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::invoke(functor, lhs[i], rhs[i]);
    }

    return ret;
}

}  // namespace detail

template<concepts::vector T, concepts::vector U>
constexpr auto operator+(T const& lhs, U const& rhs) -> concepts::nd_vector<T::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::plus<>{});
}

template<concepts::vector T>
constexpr auto operator-(T const& v) -> concepts::nd_vector_of_t<typename T::value_type, T::size> auto{
    using ret_type = typename T::template rebind<typename T::value_type>;
    concepts::nd_vector_of_t<typename T::value_type, T::size> auto ret = ret_type{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = -v[i];
    }

    return ret;
}

template<concepts::vector T, concepts::vector U>
constexpr auto operator-(T const& lhs, U const& rhs) -> concepts::nd_vector<T::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::minus<>{});
}

template<concepts::vector T, concepts::vector U>
constexpr auto operator<=>(T const& lhs, U const& rhs) -> std::partial_ordering {
    if constexpr (T::size != U::size) {
        return std::partial_ordering::unordered;
    }

    std::strong_ordering ret = lhs[0] <=> rhs[0];

    for (usize i = 1; i < T::size; i++) {
        std::strong_ordering t = lhs[i] <=> rhs[i];
        if (t != ret) {
            return std::partial_ordering::unordered;
        }
    }

    return ret;
}

namespace detail {

constexpr auto swizzle_index(char c) -> usize {
    switch (c) {
        case 's': [[fallthrough]];
        case 'r': [[fallthrough]];
        case 'x': return 0;

        case 't': [[fallthrough]];
        case 'g': [[fallthrough]];
        case 'y': return 1;

        case 'p': [[fallthrough]];
        case 'b': [[fallthrough]];
        case 'z': return 2;

        case 'q': [[fallthrough]];
        case 'a': [[fallthrough]];
        case 'w': return 3;

        default: return std::numeric_limits<usize>::max();
    }
}

template<StringLiteral Lit>
constexpr auto swizzle_needed_dimensions() -> usize {
    usize dims = 0;
    for (char c : Lit) {
        dims = std::max(dims, swizzle_index(c) + 1);
    }
    return dims;
}

template<StringLiteral Lit, typename T>
concept can_swizzle =                                                                  //
  concepts::vector<T> &&                                                               //
  std::all_of(Lit.begin(), Lit.end(), [](char c) { return 'z' >= c && c >= 'w'; }) &&  //
  (T::size >= swizzle_needed_dimensions<Lit>());

}  // namespace detail

template<StringLiteral Lit, concepts::vector T>
constexpr auto swizzle(T const& vec) -> concepts::nd_vector_of_t<typename T::value_type, Lit.size()> auto{
    static_assert(detail::can_swizzle<Lit, T>);

    concepts::nd_vector_of_t<typename T::value_type, Lit.size()> auto ret =
      typename T::template rebind_n<typename T::value_type, Lit.size()>{};

    for (usize i = 0; char c : Lit) {
        usize index = detail::swizzle_index(c);
        ret[i++] = vec[index];
    }

    return ret;
}

}  // namespace stf::blas
