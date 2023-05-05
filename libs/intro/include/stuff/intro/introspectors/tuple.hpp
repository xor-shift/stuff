#pragma once

#include <stuff/core.hpp>

#include <stuff/intro/concepts.hpp>

#include <tuple>
#include <utility>

namespace stf::intro {

template<typename Intro, typename Fn, typename Tuple, usize I = 0>
static constexpr void tuple_iterate(Tuple&& v, Fn&& fn) {
    constexpr bool is_lvalue = std::is_lvalue_reference_v<Tuple>;
    using elem_type = typename Intro::template nth_type<I>;
    using arg_type = std::conditional_t<is_lvalue, elem_type&, elem_type&&>;

    constexpr bool constexpr_iterno_functor =
      requires { std::invoke(std::forward<Fn>(fn), std::integral_constant<usize, I>{}, std::declval<arg_type>()); };

    constexpr bool runtime_iterno_functor =
      !constexpr_iterno_functor && requires { std::invoke(std::forward<Fn>(fn), I, std::declval<arg_type>()); };

    // could be || too
    static_assert(constexpr_iterno_functor != runtime_iterno_functor, "Bad functor for iterate()");

    if constexpr (constexpr_iterno_functor) {
        if constexpr (is_lvalue) {
            std::invoke(std::forward<Fn>(fn), std::integral_constant<usize, I>{}, Intro::template get<I>(v));
        } else {
            std::invoke(
              std::forward<Fn>(fn), std::integral_constant<usize, I>{},
              std::move(Intro::template get<I>(std::forward<Tuple>(v)))
            );
        }
    } else if constexpr (runtime_iterno_functor) {
        if constexpr (std::is_lvalue_reference_v<Tuple>) {
            std::invoke(std::forward<Fn>(fn), I, Intro::template get<I>(v));
        } else {
            std::invoke(std::forward<Fn>(fn), I, std::move(Intro::template get<I>(std::forward<Tuple>(v))));
        }
    }

    if constexpr (I + 1 != Intro::size()) {
        return tuple_iterate<Intro, Fn, Tuple, I + 1>(std::forward<Tuple>(v), std::forward<Fn>(fn));
    }
}

template<typename... Ts>
struct tuple_introspector {
private:
    // FIXME: bad

    struct pair_placeholder {
        pair_placeholder() = delete;
        pair_placeholder(pair_placeholder const&) = delete;
        pair_placeholder(pair_placeholder&&) = delete;
    };

    template<typename... Us>
    struct pair_type_helper {
        using pair_type = pair_placeholder;
    };

    template<typename First, typename Second>
    struct pair_type_helper<First, Second> {
        using pair_type = std::pair<First, Second>;
    };

    using pair_type = typename pair_type_helper<Ts...>::pair_type;

public:
    using intro_type = std::tuple<Ts...>;

    template<usize I>
        requires(I < sizeof...(Ts))
    using nth_type = std::tuple_element_t<I, std::tuple<Ts...>>;

    static constexpr auto size() -> usize { return sizeof...(Ts); }
    static constexpr auto size(std::tuple<Ts...> const& v) -> usize { return sizeof...(Ts); }
    static constexpr auto size(pair_type const& v) -> usize requires(sizeof...(Ts) == 2) { return 2uz; }

    template<usize I, typename Tuple>
        requires(std::is_same_v<std::remove_cvref_t<Tuple>, std::tuple<Ts...>> ||
                 std::is_same_v<std::remove_cvref_t<Tuple>, pair_type>)
    static constexpr auto get(Tuple&& v) -> decltype(auto) {
        return std::get<I>(std::forward<Tuple>(v));
    }

    template<typename Fn, typename Tuple>
        requires(std::is_same_v<std::remove_cvref_t<Tuple>, std::tuple<Ts...>> || std::is_same_v<std::remove_cvref_t<Tuple>, pair_type>)
    static constexpr void iterate(Tuple&& v, Fn&& fn) {
        return tuple_iterate<tuple_introspector<Ts...>, Fn, Tuple, 0>(std::forward<Tuple>(v), std::forward<Fn>(fn));
    }
};

static_assert(concepts::tuple_introspector<tuple_introspector<int, float, double>>);

}  // namespace stf::intro::detail

namespace std {

template<typename... Ts>
constexpr auto _stf_adl_introspector(tuple<Ts...> const&) {
    return ::stf::intro::tuple_introspector<Ts...>{};
}

template<typename... Ts>
constexpr auto _stf_adl_introspector(pair<Ts...> const&) {
    return ::stf::intro::tuple_introspector<Ts...>{};
}

}  // namespace std
