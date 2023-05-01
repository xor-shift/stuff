#pragma once

namespace stf::blas {

template<typename T, typename U>
constexpr auto cross(vector<T, 3> const& x, vector<U, 3> const& y) -> vector<std::common_type_t<T, U>, 3> {
    auto det_0 = x[1] * y[2] - x[2] * y[1];
    auto det_1 = x[0] * y[2] - x[2] * y[0];
    auto det_2 = x[0] * y[1] - x[1] * y[0];

    vector<std::common_type_t<T, U>, 3> ret{det_0, -det_1, det_2};

    return ret;
}

template<typename T, typename U>
constexpr auto cross(vector<T, 7> const& x, vector<U, 7> const& y) -> vector<std::common_type_t<T, U>, 7> {
#pragma push_macro("IDK_SOME_FACTORY")
// clang-format off
#define IDK_SOME_FACTORY(a, b, c, d, e, f, g, h, i, j, k, l) \
    x[a-1]*y[b-1]-x[c-1]*y[d-1]+x[e-1]*y[f-1]-x[g-1]*y[h-1]+x[i-1]*y[j-1]-x[k-1]*y[l-1]
    // clang-format on

    vector<std::common_type_t<T, U>, 7> ret{
      IDK_SOME_FACTORY(2, 4, 4, 2, 3, 7, 7, 3, 5, 6, 6, 5),  //
      IDK_SOME_FACTORY(3, 5, 5, 3, 4, 1, 1, 4, 6, 7, 7, 6),  //
      IDK_SOME_FACTORY(4, 6, 6, 4, 5, 2, 2, 5, 7, 1, 1, 7),  //
      IDK_SOME_FACTORY(5, 7, 7, 5, 6, 3, 3, 6, 1, 2, 2, 1),  //
      IDK_SOME_FACTORY(6, 1, 1, 6, 7, 4, 4, 7, 2, 3, 3, 2),  //
      IDK_SOME_FACTORY(7, 2, 2, 7, 1, 5, 5, 1, 3, 4, 4, 3),  //
      IDK_SOME_FACTORY(1, 3, 3, 1, 2, 6, 6, 2, 4, 5, 5, 4),  //
    };

#undef IDK_SOME_FACTORY
#pragma pop_macro("IDK_SOME_FACTORY")

    return ret;
}

template<typename T, typename U, usize N>
constexpr auto distance(vector<T, N> const& p_0, vector<U, N> const& p_1) -> std::common_type_t<T, U> {
    return length(p_0 - p_1);
}

template<typename T, typename U, usize N>
constexpr auto dot(vector<T, N> const& x, vector<U, N> const& y) -> std::common_type_t<T, U> {
    std::common_type_t<T, U> ret = 0;

    for (usize i = 0; i < N; i++) {
        ret += x[i] * y[i];
    }

    return ret;
}

template<typename T, typename U, typename V, usize N>
constexpr auto faceforward(vector<T, N> const& n, vector<U, N> const& i, vector<V, N> const& n_ref)
  -> vector<std::common_type_t<T, U, V>, N> {
    return dot(n_ref, i) < 0 ? n : -n;
}

template<typename T, usize N>
constexpr auto length(vector<T, N> const& x) -> T {
    return std::sqrt(dot(x, x));  // implicit conversion, FIXME
}

template<typename T, usize N>
constexpr auto normalize(vector<T, N> const& x) -> vector<T, N> {
    return x / length(x);
}

template<typename T, typename U, usize N>
constexpr auto reflect(vector<T, N> const& i, vector<U, N> const& n) -> vector<std::common_type_t<T, U>, N> {
    return 2 * dot(i, n) * n - i;
}

}  // namespace stf::blas
