#pragma once

#include <stuff/core/integers.hpp>

#include <algorithm>
#include <iterator>
#include <utility>

namespace stf {

// this place is currently a dumping ground for uncategorized functions and types

/// An analogue of Go's <code>struct {}</code> type\n
/// Intended to be used with stf::channel. (a channel of T=void returns <code>empty</code> values (this is akin to
/// <code>chan struct{}</code> from Go))
struct empty {};

template<usize Size>
struct string_literal {
    char data[Size];

    constexpr string_literal() = default;
    constexpr string_literal(const char (&v)[Size]) noexcept { std::copy(std::begin(v), std::end(v), data); }

    constexpr static auto size() -> usize { return Size - 1; }

    constexpr auto begin() & -> char* { return &data[0]; }
    constexpr auto end() & -> char* { return &data[size()]; }
    constexpr auto begin() const& -> const char* { return &data[0]; }
    constexpr auto end() const& -> const char* { return &data[size()]; }

    constexpr auto c_str() const& -> const char* { return data; }

    template<usize OtherSize>
    friend constexpr auto operator==(string_literal const& self, string_literal<OtherSize> const& other) -> bool {
        if constexpr (OtherSize != Size) {
            return false;
        }

        for (usize i = 0; i < Size; i++) {
            if (other.data[i] != self.data[i]) {
                return false;
            }
        }

        return true;
    }

    template<usize Start, usize Len>
        requires (Start < size())
    constexpr auto substr() const -> string_literal<Len + 1> {
        string_literal<Len + 1> ret {};
        std::copy_n(data, Len, ret.data);
        ret.data[Len] = '\0';

        return ret;
    }
};

template<typename... Ts>
struct bunch_of_types;

template<typename... Ts>
    requires(sizeof...(Ts) == 0)
struct bunch_of_types<Ts...> {
    static constexpr usize size = 0;
};

template<typename T, typename... Ts>
struct bunch_of_types<T, Ts...> {
    inline static constexpr usize size = 1uz + sizeof...(Ts);

private:
    template<usize I>
    struct nth_type_helper;

    template<usize I>
        requires(I < size)
    struct nth_type_helper<I> {
        using type = typename bunch_of_types<Ts...>::template nth_type<I - 1>;
    };

    template<>
    struct nth_type_helper<0> {
        using type = T;
    };

public:
    template<usize I>
        requires(I < size)
    using nth_type = typename nth_type_helper<I>::type;

private:
    template<usize I, template<typename> typename Predicate, bool = Predicate<typename nth_type_helper<I>::type>::value>
    struct find_type_helper;

    template<usize I, template<typename> typename Predicate>
    struct find_type_helper<I, Predicate, true> : std::integral_constant<usize, I> {};

    template<template<typename> typename Predicate>
    struct find_type_helper<size - 1, Predicate, false> : std::integral_constant<usize, size> {
        // static_assert(!std::is_same_v<Predicate<T>, Predicate<T>>, "predicate not satisfied for any of the types");
    };

    template<usize I, template<typename> typename Predicate>
    struct find_type_helper<I, Predicate, false>
        : find_type_helper<I + 1, Predicate, Predicate<nth_type<I + 1>>::value> {};

public:
    template<template<typename> typename Predicate>
    using find_type = typename find_type_helper<0, Predicate>::type;
};

template<typename T>
struct bunch_of_types<T> {
    static constexpr usize size = 1uz;

    template<usize I>
        requires(I == 0)
    using nth_type = T;

    template<template<typename> typename Predicate>
    using find_type =
      std::conditional_t<Predicate<T>::value, std::integral_constant<usize, 0>, std::integral_constant<usize, 1>>;
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
/// <pre>static_assert(STF_FUNC_EXISTS(__builtin_fabsf)); // this, most likely, fails</pre>
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

inline void assume(bool pred) {
    if (!pred) {
        std::unreachable();
    }
}

template<string_literal Lit>
inline void unreachable_with_message() {
    std::unreachable();
}

};  // namespace stf

using namespace std::literals;
using namespace std::string_view_literals;
using namespace std::string_literals;
