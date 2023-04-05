#pragma once

#include <cmath>
#include <functional>
#include <limits>

namespace stf::blas {

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto dot(T const& lhs, U const& rhs)
  -> core::type_promotion_t<typename T::value_type, typename U::value_type> {
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

    using ret_value_type = core::type_promotion_t<typename T::value_type, typename U::value_type>;

    concepts::vector_backend auto ret = typename T::template rebind<ret_value_type>{};
    ret[0] = det_0;
    ret[1] = -det_1;
    ret[2] = det_2;

    return ret;
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size && T::size == 7)
constexpr auto cross(T const& x, U const& y) -> concepts::nd_vector<7> auto{
    using ret_value_type = core::type_promotion_t<typename T::value_type, typename U::value_type>;

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

template<concepts::vector T>
constexpr auto operator-(T const& v) -> concepts::vector_like<T> auto{
    using ret_type = typename T::template rebind<>;
    concepts::vector_like<T> auto ret = ret_type{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = -v[i];
    }

    return ret;
}

/*template<concepts::vector T>
constexpr auto abs(T const& v) -> concepts::vector_like<T> auto {
    using ret_type = typename T::template rebind<>;
    concepts::vector_like<T> auto ret = ret_type{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = v[i] < 0 ? -v[i] : v[i];
    }

    return ret;
}*/

template<concepts::vector T>
constexpr auto abs(T const& v) -> typename T::value_type {
    using U = typename T::value_type;
    U sum = 0;

    for (usize i = 0; i < T::size; i++) {
        U t = v[i];
        sum += t * t;
    }

    return std::sqrt(sum);
}

template<concepts::vector T>
constexpr auto elem_abs(T const& v) -> concepts::vector_like<T> auto {
    concepts::vector_backend auto ret = typename T::template rebind<>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::abs(v[i]);
    }

    return ret;
}

template<concepts::vector T>
constexpr auto normalize(T const& v) -> concepts::vector_like<T> auto{
    return v / abs(v);
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto less_than(T const& lhs, U const& rhs) -> concepts::nd_vector_of_t<bool, T::size> auto{
    concepts::vector_backend auto ret = typename T::template rebind<bool>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = lhs[i] < rhs[i];
    }

    return ret;
}

template<concepts::vector T, concepts::vector U, concepts::vector V>
    requires(T::size == U::size && U::size == V::size)
constexpr auto mix(T const& lhs, U const& rhs, V const& param) -> concepts::vector_like<T> auto{
    using ret_value_type =
      core::type_promotion_t<typename T::value_type, typename U::value_type, typename V::value_type>;
    concepts::vector_backend auto ret = typename T::template rebind<ret_value_type>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = lhs[i] * (1 - param[i]) + rhs[i] * param[i];
    }

    return ret;
}

template<concepts::vector T, concepts::vector U, typename V>
    requires(T::size == U::size && std::is_arithmetic_v<V>)
constexpr auto mix(T const& lhs, U const& rhs, V param) -> concepts::vector_like<T> auto{
    using ret_value_type = core::type_promotion_t<typename T::value_type, typename U::value_type, V>;
    concepts::vector_backend auto ret = typename T::template rebind<ret_value_type>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = lhs[i] * (1 - param) + rhs[i] * param;
    }

    return ret;
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto min(T const& lhs, U const& rhs) -> concepts::vector_like<T> auto {
    concepts::vector_backend auto ret = typename T::template rebind<>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::min<typename T::value_type>(lhs[i], rhs[i]);
    }

    return ret;
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto max(T const& lhs, U const& rhs) -> concepts::vector_like<T> auto {
    concepts::vector_backend auto ret = typename T::template rebind<>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::max<typename T::value_type>(lhs[i], rhs[i]);
    }

    return ret;
}

template<concepts::vector T, typename U>
constexpr auto clamp(T const& lhs, U min, U max) -> concepts::vector_like<T> auto {
    concepts::vector_backend auto ret = typename T::template rebind<>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::clamp<typename T::value_type>(lhs[i], min, max);
    }

    return ret;
}

template<concepts::vector T>
constexpr auto round(T const& lhs) -> concepts::vector_like<T> auto {
    concepts::vector_backend auto ret = typename T::template rebind<>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::round(lhs[i]);
    }

    return ret;
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto pow(T const& lhs, U const& rhs) -> concepts::vector_like<T> auto{
    concepts::vector_backend auto ret = typename T::template rebind<>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::pow<typename T::value_type>(lhs[i], rhs[i]);
    }

    return ret;
}

template<concepts::vector T, typename U>
    requires(std::is_arithmetic_v<U>)
constexpr auto pow(T const& lhs, U rhs) -> concepts::vector_like<T> auto{
    concepts::vector_backend auto ret = typename T::template rebind<>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::pow<typename T::value_type>(lhs[i], rhs);
    }

    return ret;
}

template<concepts::vector T, concepts::vector U>
constexpr auto operator<=>(T const& lhs, U const& rhs) -> std::partial_ordering {
    if constexpr (T::size != U::size) {
        return std::partial_ordering::unordered;
    }

    std::partial_ordering ret = lhs[0] <=> rhs[0];

    for (usize i = 1; i < T::size; i++) {
        std::partial_ordering t = lhs[i] <=> rhs[i];
        if (t != ret) {
            return std::partial_ordering::unordered;
        }
    }

    return ret;
}

}  // namespace stf::blas
