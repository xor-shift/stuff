#pragma once

#include <stuff/core/integers.hpp>
#include <stuff/core/type_traits.hpp>

#include <algorithm>
#include <iterator>

namespace stf {

// this place is currently a dumping ground for uncategorized functions and types

template<usize Size>
struct string_literal {
    char data[Size];

    constexpr string_literal(const char (&v)[Size]) noexcept { std::copy(std::begin(v), std::end(v), data); }

    constexpr static auto size() -> usize { return Size - 1; }

    constexpr auto begin() & -> char* { return &data[0]; }
    constexpr auto end() & -> char* { return &data[size()]; }
    constexpr auto begin() const& -> const char* { return &data[0]; }
    constexpr auto end() const& -> const char* { return &data[size()]; }

    constexpr auto c_str() const& -> const char* { return data; }
};

template<typename... Funcs>
struct multi_visitor : public Funcs... {
    using Funcs::operator()...;
};

template<typename... Funcs>
multi_visitor(Funcs&&...) -> multi_visitor<Funcs...>;

/// A macro to check if a function exists with certain arguments.
/// @example
/// <pre>static_assert(STF_FUNC_EXISTS(__builtin_fabsf, 0.f)); // this, most, likely passes</pre>
/// <pre>static_assert(STF_FUNC_EXISTS(__builtin_fabsf)); // this, most, likely fails</pre>
/// <pre>static_assert(!STF_FUNC_EXISTS(ksdvjbjksdfv, 0.f)); // this, most likely, fails</pre>
#define STF_FUNC_EXISTS(_name, ...)                                                                               \
    ([]<typename... Args>(Args&&... args) {                                                                       \
        multi_visitor visitor{                                                                                    \
          []<typename... Ts>(int, Ts&&... vs) -> decltype(_name(std::forward<Ts>(vs)...), true) { return true; }, \
          []<typename... Ts>(double, Ts&&...) -> bool { return false; },                                          \
        };                                                                                                        \
                                                                                                                  \
        return visitor(0, std::forward<Args>(args)...);                                                           \
    })(__VA_ARGS__)

namespace detail {

template<typename T, typename = void>
struct type_exists_helper {
    inline static constexpr bool value = false;
};

template<typename T>
struct type_exists_helper<T, std::void_t<T>> {
    inline static constexpr bool value = true;
};

}  // namespace detail

template<typename T>
struct type_exists : ::stf::detail::type_exists_helper<T, void> {};

template<typename T>
inline static constexpr bool type_exists_v = type_exists<T>::value;

template<typename, typename = void>
constexpr bool is_type_complete_v = false;

template<typename T>
constexpr bool is_type_complete_v<T, std::void_t<decltype(sizeof(T))>> = true;

};  // namespace stf
