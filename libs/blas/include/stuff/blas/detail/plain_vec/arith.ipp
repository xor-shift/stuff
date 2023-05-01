#pragma once

namespace stf::blas {

namespace detail {

template<typename T, typename U, usize N, typename Fn>
constexpr auto vecop_binary(vector<T, N> const& x, vector<U, N> const& y, Fn&& fn)
  -> vector<std::invoke_result_t<Fn, T, U>, N>
    requires requires { std::invoke(fn, x[0], y[0]); }
{
    vector<std::invoke_result_t<Fn, T, U>, N> ret;
    for (usize i = 0; i < N; i++) {
        ret[i] = std::invoke(fn, x[i], y[i]);
    }
    return ret;
}

template<typename T, typename U, usize N, typename Fn>
constexpr auto vecop_binary(vector<T, N> const& x, U y, Fn&& fn) -> vector<std::invoke_result_t<Fn, T, U>, N>
    requires requires { std::invoke(fn, x[0], y); }
{
    vector<std::invoke_result_t<Fn, T, U>, N> ret;
    for (usize i = 0; i < N; i++) {
        ret[i] = std::invoke(fn, x[i], y);
    }
    return ret;
}

template<typename T, typename U, usize N, typename Fn>
constexpr auto vecop_binary(T x, vector<U, N> const& y, Fn&& fn) -> vector<std::invoke_result_t<Fn, T, U>, N>
    requires requires { std::invoke(fn, x, y[0]); }
{
    vector<std::invoke_result_t<Fn, T, U>, N> ret;
    for (usize i = 0; i < N; i++) {
        ret[i] = std::invoke(fn, x, y[i]);
    }
    return ret;
}

template<typename T, usize N, typename Fn>
constexpr auto vecop_unary(vector<T, N> const& x, Fn&& fn) -> vector<std::invoke_result_t<Fn, T>, N>
    requires requires { std::invoke(fn, x[0]); }
{
    vector<std::invoke_result_t<Fn, T>, N> ret;
    for (usize i = 0; i < N; i++) {
        ret[i] = std::invoke(fn, x[i]);
    }
    return ret;
}

}  // namespace detail

/*template<typename T, typename U, usize N>
constexpr auto operator+(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N> {
    return detail::vecop_binary(x, y, std::plus{});
}*/

//static_assert((vector<int, 3>{-1, 2, -3} + vector<int, 3>{-1, 2, -3}) == vector<int, 3>{-2, 4, -6});

template<typename T, usize N>
constexpr auto operator-(vector<T, N> const& x) -> vector<T, N> {
    return detail::vecop_unary(x, std::negate{});
}

static_assert(-vector<int, 3>{-1, 2, -3} == vector<int, 3>{1, -2, 3});

template<typename T, typename U, usize N>
constexpr auto operator-(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N> {
    return detail::vecop_binary(x, y, std::minus{});
}

static_assert((vector<int, 3>{-1, 2, -3} - vector<int, 3>{-1, 2, -3}) == vector<int, 3>(0));

template<typename T, typename U, usize N>
constexpr auto operator*(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N> {
    return detail::vecop_binary(x, y, std::multiplies{});
}

template<typename T, typename U, usize N>
constexpr auto operator*(vector<T, N> const& x, U y) -> vector<std::common_type_t<T, U>, N>
    requires requires { x[0] * y; }
{
    return detail::vecop_binary(x, y, std::multiplies{});
}

template<typename T, typename U, usize N>
constexpr auto operator*(T x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>
    requires requires { x * y[0]; }
{
    return detail::vecop_binary(x, y, std::multiplies{});
}

template<typename T, typename U, usize N>
constexpr auto operator/(vector<T, N> const& x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N> {
    return detail::vecop_binary(x, y, std::divides{});
}

template<typename T, typename U, usize N>
constexpr auto operator/(vector<T, N> const& x, U y) -> vector<std::common_type_t<T, U>, N>
    requires requires { x[0] / y; }
{
    return detail::vecop_binary(x, y, std::divides{});
}

template<typename T, typename U, usize N>
constexpr auto operator/(T x, vector<U, N> const& y) -> vector<std::common_type_t<T, U>, N>
    requires requires { x / y[0]; }
{
    return detail::vecop_binary(x, y, std::divides{});
}

}  // namespace stf::blas
