#pragma once

namespace stf::blas {

namespace detail {

template<concepts::vector T, concepts::vector U, typename Fn>
    requires(T::size == U::size)
constexpr auto elementwise_fn(T const& lhs, U const& rhs, Fn&& functor = {}) {
    // TODO: implement expression templates, maybe
    // real to-do: benchmark expression templates versus straightforward operations

    using ret_value_type = core::type_promotion_t<typename T::value_type, typename U::value_type>;
    concepts::vector_backend auto ret = typename T::template rebind<ret_value_type>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::invoke(functor, lhs[i], rhs[i]);
    }

    return ret;
}

template<concepts::vector T, typename U, typename Fn>
    requires std::is_arithmetic_v<U>
constexpr auto elementwise_fn(T const& lhs, U rhs, Fn&& functor = {}) {
    using ret_value_type = core::type_promotion_t<typename T::value_type, U>;
    concepts::vector_backend auto ret = typename T::template rebind<ret_value_type>{};

    for (usize i = 0; i < T::size; i++) {
        ret[i] = std::invoke(functor, lhs[i], rhs);
    }

    return ret;
}

template<typename T, concepts::vector U, typename Fn>
    requires std::is_arithmetic_v<T>
constexpr auto elementwise_fn(T lhs, U const& rhs, Fn&& functor = {}) {
    using ret_value_type = core::type_promotion_t<typename U::value_type, T>;
    concepts::vector_backend auto ret = typename U::template rebind<ret_value_type>{};

    for (usize i = 0; i < U::size; i++) {
        ret[i] = std::invoke(functor, lhs, rhs[i]);
    }

    return ret;
}

}  // namespace detail

#pragma push_macro("ELEMENTWISE_FACTORY")
#pragma pop_macro("ELEMENTWISE_FACTORY")

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto operator+(T const& lhs, U const& rhs) -> concepts::nd_vector<T::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::plus<>{});
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto operator-(T const& lhs, U const& rhs) -> concepts::nd_vector<T::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::minus<>{});
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto operator*(T const& lhs, U const& rhs) -> concepts::nd_vector<T::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::multiplies<>{});
}

template<concepts::vector T, concepts::vector U>
    requires(T::size == U::size)
constexpr auto operator/(T const& lhs, U const& rhs) -> concepts::nd_vector<T::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::divides<>{});
}

template<concepts::vector T, typename U>
    requires std::is_arithmetic_v<U>
constexpr auto operator/(T const& lhs, U rhs) -> concepts::nd_vector<T::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::divides<>{});
}

template<typename T, concepts::vector U>
    requires std::is_arithmetic_v<T>
constexpr auto operator/(T lhs, U const& rhs) -> concepts::nd_vector<U::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::divides<>{});
}

template<concepts::vector T, typename U>
    requires std::is_arithmetic_v<U>
constexpr auto operator*(T const& lhs, U rhs) -> concepts::nd_vector<T::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::multiplies<>{});
}

template<typename T, concepts::vector U>
    requires std::is_arithmetic_v<T>
constexpr auto operator*(T lhs, U const& rhs) -> concepts::nd_vector<U::size> auto{
    return detail::elementwise_fn(lhs, rhs, std::multiplies<>{});
}

}  // namespace stf::blas
