#pragma once

#include <stuff/core.hpp>
#include <stuff/intro/concepts.hpp>

#include <span>

namespace stf::intro::detail {

template<typename T, usize N = std::dynamic_extent>
struct span_introspector {
    using intro_type = std::span<T, N>;
    using value_type = T;

    template<usize I>
        requires(I < N)
    using nth_type = value_type;

    static constexpr auto size(intro_type const& v) -> usize { return v.size(); }

    template<typename Span>
        requires(std::is_same_v<std::decay_t<Span>, intro_type>)
    static constexpr auto begin(Span&& v) -> decltype(auto) {
        return v.begin();
    }

    template<typename Span>
        requires(std::is_same_v<std::decay_t<Span>, intro_type>)
    static constexpr auto end(Span&& v) -> decltype(auto) {
        return v.end();
    }

    template<typename Span>
        requires(std::is_same_v<std::decay_t<Span>, intro_type>)
    static constexpr auto index(Span&& v, usize i) -> decltype(auto) {
        if constexpr (std::is_lvalue_reference_v<Span>) {
            return v[i];
        } else {
            return std::move(v[i]);
        }
    }

    template<usize I, typename Span>
        requires(std::is_same_v<std::decay_t<Span>, intro_type>)
    static constexpr auto get(Span&& v) -> decltype(auto) {
        return index(std::forward<Span>(v), I);
    }
};

static_assert(concepts::span_introspector<span_introspector<int, 3>>);

}  // namespace stf::intro::detail

namespace std {

template<typename T, usize N = std::dynamic_extent>
constexpr auto _stf_adl_introspector(std::span<T, N> const&) {
    return ::stf::intro::detail::span_introspector<T, N>{};
}

}  // namespace std
