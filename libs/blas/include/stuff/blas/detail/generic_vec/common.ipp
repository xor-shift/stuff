#pragma once

#include <stuff/sfloat.hpp>

namespace stf::blas {

template<concepts::vector X, typename T, usize N>
constexpr auto abs(X const& x) -> concepts::generic_vector<T, N> auto{
    return detail::vecop_unary<T, N>(x, [](T v) -> T { return v < 0 ? -v : v; });
}

template<concepts::vector X, typename T, usize N>
constexpr auto ceil(X const& x) -> concepts::generic_vector<T, N> auto{
    return detail::vecop_unary<T, N>(x, [](T v) -> T { return static_cast<T>(sfloat::ceil(v)); });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto clamp(X const& x, Y const& min, Y const& max)
  -> concepts::generic_vector<std::common_type_t<T, U>, N> auto{
    using V = std::common_type_t<T, U>;
    typename T::template rebind<V, N> ret{};

    for (usize i = 0; i < N; i++) {
        T x_i = x[i];
        U min_i = min[i];
        U max_i = max[i];
        ret[i] = x_i < min_i ? min_i : max_i < x_i ? max_i : x_i;
    }

    return ret;
}

template<concepts::vector X, typename U, typename T, usize N>
constexpr auto clamp(X const& x, U min, U max) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto{
    using V = std::common_type_t<T, U>;
    typename X::template rebind<V, N> ret{};

    for (usize i = 0; i < N; i++) {
        T x_i = x[i];
        ret[i] = x_i < min ? min : max < x_i ? max : x_i;
    }

    return ret;
}

template<concepts::vector X, typename T, usize N>
constexpr auto floor(X const& x) -> concepts::generic_vector<T, N> auto{
    return detail::vecop_unary<T, N>(x, [](T v) -> T { return static_cast<T>(sfloat::floor(v)); });
}

template<concepts::vector X, typename T, usize N>
constexpr auto fract(X const& x) -> concepts::generic_vector<T, N> auto{
    return detail::vecop_unary<T, N>(x, [](T v) -> T { return v - static_cast<T>(sfloat::ceil(v)); });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto max(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> std::common_type_t<T, U> { return x < y ? y : x; });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
constexpr auto max(X const& x, U y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> std::common_type_t<T, U> { return x < y ? y : x; });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto min(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> std::common_type_t<T, U> { return x < y ? x : y; });
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
constexpr auto min(X const& x, U y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) -> std::common_type_t<T, U> { return x < y ? x : y; });
}

template<concepts::vector X, concepts::vector Y, concepts::vector A, typename T, typename U, typename V, usize N>
    requires((X::size == Y::size) && (X::size == A::size))
constexpr auto mix(X const& x, Y const& y, A const& a) -> concepts::generic_vector<std::common_type_t<T, U, V>, N> auto{
    using R = std::common_type_t<T, U, V>;
    typename X::template rebind<R, N> ret{};

    for (usize i = 0; i < N; i++) {
        T x_i = x[i];
        U y_i = y[i];
        V a_i = a[i];
        ret[i] = x_i * (1 - a_i) + y_i * a_i;
    }

    return ret;
}

template<concepts::vector X, concepts::vector Y, typename A, typename T, typename U, usize N>
    requires(X::size == Y::Size)
constexpr auto mix(X const& x, Y const& y, A a) -> concepts::generic_vector<std::common_type_t<T, U, A>, N> auto{
    using R = std::common_type_t<T, U, A>;
    typename X::template rebind<R, N> ret{};

    for (usize i = 0; i < N; i++) {
        T x_i = x[i];
        U y_i = y[i];
        ret[i] = x_i * (1 - a) + y_i * a;
    }

    return ret;
}
template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto mod(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto{
    std::unreachable();
    return typename X::template rebind<T, N>{};
}

template<concepts::vector X, typename U, typename T, usize N>
constexpr auto mod(X const& x, U y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto{
    std::unreachable();
    return typename X::template rebind<T, N>{};
}

template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>
    requires(X::size == Y::size)
constexpr auto pow(X const& x, Y const& y) -> concepts::generic_vector<decltype(std::pow(T{}, U{})), N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) { return std::pow(x, y); });
}

template<concepts::vector X, typename U, typename T, usize N>
    requires(!concepts::vector<U>)
constexpr auto pow(X const& x, U y) -> concepts::generic_vector<decltype(std::pow(T{}, U{})), N> auto{
    return detail::vecop_binary<T, U, N>(x, y, [](T x, U y) { return std::pow(x, y); });
}

template<concepts::vector X, typename T, usize N>
constexpr auto round(X const& x) -> concepts::generic_vector<T, N> auto{
    return detail::vecop_unary<T, N>(x, [](T v) -> T { return static_cast<T>(sfloat::round(v)); });
}

template<concepts::vector X, typename T, usize N>
constexpr auto sign(X const& x) -> concepts::generic_vector<T, N> auto{
    std::unreachable();
    return typename X::template rebind<T, N>{};
}

template<concepts::vector E, concepts::vector X, typename T, typename U, usize N>
    requires(E::size == X::size)
constexpr auto step(E const& edge, X const& x) -> concepts::generic_vector<U, N> auto{
    std::unreachable();
    return typename X::template rebind<T, N>{};
}

template<typename T, concepts::vector X, typename U, usize N>
constexpr auto step(T edge, X const& x) -> concepts::generic_vector<U, N> auto{
    std::unreachable();
    return typename X::template rebind<T, N>{};
}

template<concepts::vector E, concepts::vector X, typename T, typename U, usize N>
    requires(E::size == X::size)
constexpr auto smooth_step(E const& edge_0, E const& edge_1, X const& x) -> concepts::generic_vector<U, N> auto{
    std::unreachable();
    return typename X::template rebind<T, N>{};
}

template<typename T, concepts::vector X, typename U, usize N>
constexpr auto smooth_step(T edge_0, T edge_1, X const& x) -> concepts::generic_vector<U, N> auto{
    std::unreachable();
    return typename X::template rebind<T, N>{};
}

}  // namespace stf::blas
