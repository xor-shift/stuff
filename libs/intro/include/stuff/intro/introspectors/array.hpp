#pragma once

#include <stuff/core.hpp>
#include <stuff/intro/concepts.hpp>

#include <array>

namespace stf::intro {

template<typename T, usize N>
struct array_introspector {
    using intro_type = std::array<T, N>;
    using value_type = T;

    template<usize I>
        requires(I < N)
    using nth_type = value_type;

    static constexpr auto size(const T (&v)[N]) -> usize { return N; }
    static constexpr auto size(std::array<T, N> const& v) -> usize { return N; }
    static constexpr auto size() -> usize { return N; }

    static constexpr auto begin(const T (&v)[N]) -> const T* { return v; }
    static constexpr auto begin(const T (&&v)[N]) -> const T* { return v; }
    static constexpr auto begin(T (&v)[N]) -> T* { return v; }
    static constexpr auto begin(T (&&v)[N]) -> T* { return v; }

    static constexpr auto end(const T (&v)[N]) -> const T* { return v + N; }
    static constexpr auto end(const T (&&v)[N]) -> const T* { return v + N; }
    static constexpr auto end(T (&v)[N]) -> T* { return v + N; }
    static constexpr auto end(T (&&v)[N]) -> T* { return v + N; }

    static constexpr auto begin(std::array<T, N> const& v) -> const T* { return v.begin(); }
    static constexpr auto begin(std::array<T, N> const&& v) -> const T* { return v.begin(); }
    static constexpr auto begin(std::array<T, N>& v) -> T* { return v.begin(); }
    static constexpr auto begin(std::array<T, N>&& v) -> T* { return v.begin(); }

    static constexpr auto end(std::array<T, N> const& v) -> const T* { return v.end(); }
    static constexpr auto end(std::array<T, N> const&& v) -> const T* { return v.end(); }
    static constexpr auto end(std::array<T, N>& v) -> T* { return v.end(); }
    static constexpr auto end(std::array<T, N>&& v) -> T* { return v.end(); }

    template<usize I>
    static constexpr auto get(const T (&v)[N]) -> T const& {
        return v[I];
    }

    template<usize I>
    static constexpr auto get(std::array<T, N> const& v) -> T const& {
        return v[I];
    }

    template<usize I>
    static constexpr auto get(T (&v)[N]) -> T& {
        return v[I];
    }

    template<usize I>
    static constexpr auto get(std::array<T, N>& v) -> T& {
        return v[I];
    }

    template<usize I>
    static constexpr auto get(const T (&&v)[N]) -> T const&& {
        return std::move(v[I]);
    }

    template<usize I>
    static constexpr auto get(std::array<T, N> const&& v) -> T const&& {
        return std::move(v[I]);
    }
    template<usize I>
    static constexpr auto get(T (&&v)[N]) -> T&& {
        return std::move(v[I]);
    }

    template<usize I>
    static constexpr auto get(std::array<T, N>&& v) -> T&& {
        return std::move(v[I]);
    }
};

static_assert(concepts::array_introspector<array_introspector<int, 3>>);

}  // namespace stf::intro::detail

template<typename T, usize N>
constexpr auto _stf_adl_introspector(const T (&)[N]) {
    return ::stf::intro::array_introspector<T, N>{};
}

namespace std {

template<typename T, usize N>
constexpr auto _stf_adl_introspector(std::array<T, N> const&) {
    return ::stf::intro::array_introspector<T, N>{};
}

}  // namespace std
