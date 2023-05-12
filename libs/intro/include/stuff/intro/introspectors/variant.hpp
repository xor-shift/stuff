#pragma once

#ifdef STF_INTRO_HAS_BEEN_INCLUDED
# error you should include introspector related headers before <stuff/intro.hpp>
#endif

#include <stuff/core.hpp>
#include <stuff/intro/concepts.hpp>

#include <tuple>
#include <variant>

namespace stf::intro {

template<typename... Ts>
struct variant_introspector {
    using intro_type = std::variant<Ts...>;

    static constexpr auto size() -> usize { return sizeof...(Ts); }

    template<typename Variant>
    static constexpr auto size(Variant&&) -> usize {
        return size();
    }

    template<usize I>
        requires(I < size())
    using nth_type = std::tuple_element_t<I, std::tuple<Ts...>>;

    template<usize I, typename Variant>
    static constexpr auto get(Variant&& v) -> decltype(auto) {
        return std::get<I>(std::forward<Variant>(v));
    }

    template<typename Variant>
    static constexpr auto held_index(Variant&& v) -> usize {
        return v.index();
    }

    template<typename T, typename Variant>
    static constexpr auto holds_alternative(Variant&& v) -> usize {
        using std::holds_alternative;
        return holds_alternative<T>(std::forward<Variant>(v));
    }

    template<typename Fn, typename Variant>
    static constexpr auto visit(Fn&& fn, Variant&& v) {
        using std::visit;
        return visit(std::forward<Fn>(fn), std::forward<Variant>(v));
    }
};

static_assert(concepts::variant_introspector<variant_introspector<int, float, double>>);

}  // namespace stf::intro

namespace std {

template<typename... Ts>
constexpr auto _stf_adl_introspector(variant<Ts...> const&) {
    return ::stf::intro::variant_introspector<Ts...>{};
}

}  // namespace std
