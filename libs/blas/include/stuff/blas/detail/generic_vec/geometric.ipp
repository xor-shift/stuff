#pragma once

namespace stf::blas {

template<concepts::vector X, concepts::vector Y, typename T, typename U>
    requires(X::size == 3, Y::size == 3)
constexpr auto cross(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, 3> auto{
    blas::vector<T, 3> c_x(x);  // cache the vectors
    blas::vector<T, 3> c_y(y);  // expression vectors will calculate the values on every call to operator[](usize)

    auto det_0 = c_x[1] * c_y[2] - c_x[2] * c_y[1];
    auto det_1 = c_x[0] * c_y[2] - c_x[2] * c_y[0];
    auto det_2 = c_x[0] * c_y[1] - c_x[1] * c_y[0];

    using R = std::common_type_t<T, U>;
    typename X::template rebind<R, 3> ret{det_0, -det_1, det_2};

    return ret;
}

template<concepts::vector X, concepts::vector Y, typename T, typename U>
    requires(X::size == 7, Y::size == 7)
constexpr auto cross(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, 7> auto{
    blas::vector<T, 7> c_x(x);
    blas::vector<T, 7> c_y(y);

    // clang-format off
#define IDK_SOME_FACTORY(a, b, c, d, e, f, g, h, i, j, k, l) \
    c_x[a-1]*c_y[b-1]-c_x[c-1]*c_y[d-1]+c_x[e-1]*c_y[f-1]-c_x[g-1]*c_y[h-1]+c_x[i-1]*c_y[j-1]-c_x[k-1]*c_y[l-1]
    // clang-format on

    using R = std::common_type_t<T, U>;
    typename X::template rebind<R, 3> ret{};
    ret[0] = IDK_SOME_FACTORY(2, 4, 4, 2, 3, 7, 7, 3, 5, 6, 6, 5);
    ret[1] = IDK_SOME_FACTORY(3, 5, 5, 3, 4, 1, 1, 4, 6, 7, 7, 6);
    ret[2] = IDK_SOME_FACTORY(4, 6, 6, 4, 5, 2, 2, 5, 7, 1, 1, 7);
    ret[3] = IDK_SOME_FACTORY(5, 7, 7, 5, 6, 3, 3, 6, 1, 2, 2, 1);
    ret[4] = IDK_SOME_FACTORY(6, 1, 1, 6, 7, 4, 4, 7, 2, 3, 3, 2);
    ret[5] = IDK_SOME_FACTORY(7, 2, 2, 7, 1, 5, 5, 1, 3, 4, 4, 3);
    ret[6] = IDK_SOME_FACTORY(1, 3, 3, 1, 2, 6, 6, 2, 4, 5, 5, 4);

    return ret;
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto distance(X const& p_0, Y const& p_1) -> decltype(std::sqrt(std::common_type_t<T, U>{})) {
    std::common_type_t<T, U> sum = 0;

    for (usize i = 0; i < N; i++) {
        std::common_type_t<T, U> term = p_0[i] - p_1[i];

        sum += term * term;
    }

    return std::sqrt(sum);
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto dot(X const& x, Y const& y) -> std::common_type_t<T, U> {
    T ret = 0;

    for (usize i = 0; i < N; i++) {
        ret += x[i] * y[i];
    }

    return ret;
}

template<concepts::vector X, typename T, usize N>
constexpr auto length(X const& x) -> decltype(std::sqrt(T{})) {
    T sum = 0;

    // we can't use dot() for the same reason we cache the vectors' elements in cross() for

    for (usize i = 0; i < N; i++) {
        T x_i = x[i];

        sum += x_i * x_i;
    }

    return std::sqrt(sum);
}

template<concepts::vector X, typename T, usize N>
constexpr auto normalize(X const& x) -> concepts::generic_vector<T, N> auto{
    blas::vector<T, N> cached_x(x);
    auto len = length(cached_x);

    for (T& v : cached_x) {
        v /= len;
    }

    return typename X::template rebind<T, N>(cached_x);
}

}  // namespace stf::blas
