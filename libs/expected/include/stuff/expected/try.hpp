#pragma once

#include <stuff/expected/expected.hpp>
#include <stuff/expected/unexpected.hpp>

#include <expected>
#include <optional>

namespace stf::detail {

template<typename T>
struct try_helper;

template<typename T>
struct try_helper<std::optional<T>> {
    static constexpr auto has_value(std::optional<T> const& v) -> bool { return bool(v); }
    static constexpr auto value(std::optional<T>&& v) -> T&& { return std::move(*v); }
    static constexpr auto error(std::optional<T>&& v) -> std::nullopt_t { return std::nullopt; }
};

template<typename T, typename E>
    requires(!std::is_void_v<T>)
struct try_helper<expected<T, E>> {
    static constexpr auto has_value(expected<T, E> const& v) -> bool { return bool(v); }
    static constexpr auto value(expected<T, E>&& v) -> T&& { return std::move(v.value()); }
    static constexpr auto error(expected<T, E>&& v) -> unexpected<E> { return unexpected{std::move(v.error())}; }
};

template<typename T, typename E>
    requires(!std::is_void_v<T>)
struct try_helper<std::expected<T, E>> {
    static constexpr auto has_value(std::expected<T, E> const& v) -> bool { return bool(v); }
    static constexpr auto value(std::expected<T, E>&& v) -> T&& { return std::move(v.value()); }
    static constexpr auto error(std::expected<T, E>&& v) -> std::unexpected<E> { return std::unexpected{std::move(v.error())}; }
};

template<typename T, typename E>
    requires std::is_void_v<T>
struct try_helper<expected<T, E>> {
    static constexpr auto has_value(expected<T, E> const& v) -> bool { return bool(v); }
    static constexpr void value(expected<T, E>&& v) {}
    static constexpr auto error(expected<T, E>&& v) -> unexpected<E> { return unexpected{std::move(v.error())}; }
};

template<typename T, typename E>
    requires std::is_void_v<T>
struct try_helper<std::expected<T, E>> {
    static constexpr auto has_value(std::expected<T, E> const& v) -> bool { return bool(v); }
    static constexpr void value(std::expected<T, E>&& v) {}
    static constexpr auto error(std::expected<T, E>&& v) -> std::unexpected<E> { return std::unexpected{std::move(v.error())}; }
};

template<typename T>
struct try_helper<const T> : try_helper<T> {};

template<typename T>
struct try_helper<volatile T> : try_helper<T> {};

template<typename T>
struct try_helper<const volatile T> : try_helper<T> {};

}  // namespace stf::detail

#define TRYX_BASE(_return_keyword, _res_name, _helper_name, ...)             \
    ({                                                                       \
        auto _res_name = (__VA_ARGS__);                                      \
        using _helper_name = ::stf::detail::try_helper<decltype(_res_name)>; \
        if (!_helper_name::has_value(_res_name)) {                           \
            _return_keyword _helper_name::error(std::move(_res_name));       \
        }                                                                    \
        _helper_name::value(std::move(_res_name));                           \
    })

#define TRYX_CAT_1(_a, _b) _a##_##_b
#define TRYX_CAT_0(_a, _b) TRYX_CAT_1(_a, _b)

#define TRYX(...) TRYX_BASE(return, TRYX_CAT_0(_res, __COUNTER__), TRYX_CAT_0(_helper_type, __COUNTER__), __VA_ARGS__)
#define TRYX_CORO(...) TRYX_BASE(co_return, TRYX_CAT_0(_res, __COUNTER__), TRYX_CAT_0(_helper_type, __COUNTER__), __VA_ARGS__)

#define UNWRAP(...)                                                     \
    ({                                                                  \
        auto _res = (__VA_ARGS__);                                      \
        using _helper_type = ::stf::detail::try_helper<decltype(_res)>; \
        if (!_helper_type::has_value(_res)) {                           \
            std::abort();                                               \
        }                                                               \
        _helper_type::value(std::move(_res));                           \
    })

#define TRY_OR_BREAK(...)                                               \
    ({                                                                  \
        auto _res = (__VA_ARGS__);                                      \
        using _helper_type = ::stf::detail::try_helper<decltype(_res)>; \
        if (!_helper_type::has_value(_res)) {                           \
            break;                                                      \
        }                                                               \
        _helper_type::value(std::move(_res));                           \
    })

#define TRY_OR_RETURN(_try_expr, ...)                                   \
    ({                                                                  \
        auto _res = (_try_expr);                                        \
        using _helper_type = ::stf::detail::try_helper<decltype(_res)>; \
        if (!_helper_type::has_value(_res)) {                           \
            return (__VA_ARGS__);                                       \
        }                                                               \
        _helper_type::value(std::move(_res));                           \
    })
