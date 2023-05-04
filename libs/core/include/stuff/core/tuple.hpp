#pragma once

#include <stuff/core/integers.hpp>

#include <functional>
#include <tuple>

namespace stf {

namespace detail {

template<typename T, typename U>
struct tuple_combiner;
template<typename T, usize N>
struct tuple_generator;

}  // namespace detail

template<typename T, typename U>
using tuple_combine_t = typename detail::tuple_combiner<T, U>::type;

template<typename T, size_t N>
using n_tuple_t = typename detail::tuple_generator<T, N>::type;

namespace detail {

template<typename Fn, typename Tuple, usize... Is>
constexpr auto tuple_call_helper(Fn&& fn, Tuple&& tuple, std::integer_sequence<usize, Is...>) noexcept(
  noexcept(std::is_invocable_v<Fn, decltype(std::get<Is>(std::forward<Tuple>(tuple)))...>)
)
    requires requires { std::invoke(std::forward<Fn>(fn), std::get<Is>(std::forward<Tuple>(tuple))...); }
{
    return std::invoke(std::forward<Fn>(fn), std::get<Is>(std::forward<Tuple>(tuple))...);
}

}  // namespace detail

/// Because std::apply participates in overload resolution even if the invoke expression is invalid
template<typename Fn, typename Tuple>
constexpr auto tuple_call(Fn&& fn, Tuple&& tuple)
    requires requires {
                 detail::tuple_call_helper(
                   std::forward<Fn>(fn), std::forward<Tuple>(tuple),
                   std::make_integer_sequence<usize, std::tuple_size_v<Tuple>>{}
                 );
             }
{
    return detail::tuple_call_helper(
      std::forward<Fn>(fn), std::forward<Tuple>(tuple), std::make_integer_sequence<usize, std::tuple_size_v<Tuple>>{}
    );
}

static_assert(tuple_call([](int a, int b) -> int { return a + b; }, std::make_tuple(1, 2)) == 3);
// static_assert(!requires { tuple_call([](int a, int b) -> int { return a + b; }, std::make_tuple(1, 2, 3)); });
static_assert(tuple_call([]<typename T>(T&& v) -> bool { return !std::is_reference_v<T>; }, std::make_tuple(1)));

namespace detail {

template<typename T>
struct tuple_maker {
    template<typename... Args>
    constexpr T operator()(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
        requires requires { T(std::forward<Args>(args)...); }
    {
        return T(std::forward<Args>(args)...);
    }
};

template<typename T, typename Tuple, usize... Is>
constexpr auto tuple_make_helper(Tuple&& tuple, std::integer_sequence<usize, Is...>)  //
  noexcept(std::is_nothrow_constructible_v<T, decltype(std::get<Is>(std::forward<Tuple>(tuple)))...>)
    requires std::is_constructible_v<T, decltype(std::get<Is>(std::forward<Tuple>(tuple)))...>
{
    return T(std::get<Is>(std::forward<Tuple>(tuple))...);
}

}  // namespace detail

/// Because std::make_from_tuple participates in overload resolution even if the invoke expression is invalid
template<typename T, typename Tuple>
constexpr auto tuple_make(Tuple&& tuple)
    requires requires(std::make_integer_sequence<usize, std::tuple_size_v<Tuple>> sequence) {
                 detail::tuple_make_helper<T>(std::forward<Tuple>(tuple), sequence);
             }
{
    return detail::tuple_make_helper<T>(
      std::forward<Tuple>(tuple), std::make_integer_sequence<usize, std::tuple_size_v<Tuple>>{}
    );
}

static_assert(tuple_make<decltype([] constexpr { return true; })>(std::make_tuple())());

}  // namespace stf

#include <stuff/core/detail/tuple.ipp>
