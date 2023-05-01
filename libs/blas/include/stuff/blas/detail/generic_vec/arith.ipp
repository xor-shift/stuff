#pragma once

namespace stf::blas {

namespace detail {

template<typename T, typename U, usize N, typename Fn>
constexpr auto
vecop_binary(concepts::generic_vector<T, N> auto const& x, concepts::generic_vector<U, N> auto const& y, Fn&& fn)
  -> concepts::generic_vector<std::invoke_result_t<Fn, T, U>, N> auto
    requires requires { std::invoke(fn, x[0], y[0]); }
{
    using V = std::invoke_result_t<Fn, T, U>;
    typename std::decay_t<decltype(x)>::template rebind<V, N> ret{};

    for (usize i = 0; i < N; i++) {
        ret[i] = std::invoke(fn, x[i], y[i]);
    }

    return ret;
}

template<typename T, typename U, usize N, typename Fn>
constexpr auto vecop_binary(concepts::generic_vector<T, N> auto const& x, U y, Fn&& fn)
  -> concepts::generic_vector<std::invoke_result_t<Fn, T, U>, N> auto
    requires requires { std::invoke(fn, x[0], y); }
{
    using V = std::invoke_result_t<Fn, T, U>;
    typename std::decay_t<decltype(x)>::template rebind<V, N> ret{};

    for (usize i = 0; i < N; i++) {
        ret[i] = std::invoke(fn, x[i], y);
    }

    return ret;
}

template<typename T, typename U, usize N, typename Fn>
constexpr auto vecop_binary(T x, concepts::generic_vector<U, N> auto const& y, Fn&& fn)
  -> concepts::generic_vector<std::invoke_result_t<Fn, T, U>, N> auto
    requires requires { std::invoke(fn, x, y[0]); }
{
    using V = std::invoke_result_t<Fn, T, U>;
    typename std::decay_t<decltype(y)>::template rebind<V, N> ret{};

    for (usize i = 0; i < N; i++) {
        ret[i] = std::invoke(fn, x, y[i]);
    }

    return ret;
}

template<typename T, usize N, typename Fn>
constexpr auto vecop_unary(concepts::generic_vector<T, N> auto const& x, Fn&& fn)
  -> concepts::generic_vector<std::invoke_result_t<Fn, T>, N> auto
    requires requires { std::invoke(fn, x[0]); }
{
    using V = std::invoke_result_t<Fn, T>;
    typename std::decay_t<decltype(x)>::template rebind<V, N> ret{};

    for (usize i = 0; i < N; i++) {
        ret[i] = std::invoke(fn, x[i]);
    }

    return ret;
}

}  // namespace detail

#pragma push_macro("BINARY_ARITH_FACTORY_VV")
#pragma push_macro("BINARY_ARITH_FACTORY_VS_SV")

// clang-format off
#define BINARY_ARITH_FACTORY_VV(_name, _functor)                                                             \
template<concepts::vector X, concepts::vector Y, typename T, typename U, usize N>                            \
constexpr auto _name(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto { \
    return detail::vecop_binary<T, U, N>(x, y, _functor);                                                    \
}

#define BINARY_ARITH_FACTORY_VS_SV(_name, _sym, _functor)                                                                                    \
template<concepts::vector X, typename U, typename T, usize N>                                                                                \
    requires(!concepts::vector<U>)                                                                                                           \
constexpr auto _name(X const& x, U y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto requires requires { x[0] _sym y; } {     \
    return detail::vecop_binary<T, U, N>(x, y, _functor);                                                                                    \
}                                                                                                                                            \
                                                                                                                                             \
template<typename T, concepts::vector Y, typename U, usize N>                                                                                \
    requires(!concepts::vector<T>)                                                                                                           \
constexpr auto _name(T x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto requires requires { x _sym y[0]; } {     \
    return detail::vecop_binary<T, U, N>(x, y, _functor);                                                                                    \
}

// clang-format on

BINARY_ARITH_FACTORY_VV(operator+, std::plus{});
BINARY_ARITH_FACTORY_VV(operator-, std::minus{});
BINARY_ARITH_FACTORY_VV(operator*, std::multiplies{});
BINARY_ARITH_FACTORY_VV(operator/, std::divides{});

BINARY_ARITH_FACTORY_VS_SV(operator*, *, std::multiplies{});
BINARY_ARITH_FACTORY_VS_SV(operator/, /, std::divides{});

#undef BINARY_ARITH_FACTORY_VV
#pragma pop_macro("BINARY_ARITH_FACTORY_VV")
#undef BINARY_ARITH_FACTORY_VS_SV
#pragma pop_macro("BINARY_ARITH_FACTORY_VS_SV")

template<concepts::vector X, typename T, usize N>
constexpr auto operator-(X const& x) -> concepts::generic_vector<T, N> auto{
    return detail::vecop_unary<T, N>(x, std::negate{});
}

}  // namespace stf::blas
