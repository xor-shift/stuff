#pragma once

#include <stuff/expected/expected.hpp>
#include <stuff/expected/unexpected.hpp>

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
    requires std::is_void_v<T>
struct try_helper<expected<T, E>> {
    static constexpr auto has_value(expected<T, E> const& v) -> bool { return bool(v); }
    static constexpr void value(expected<T, E>&& v) {}
    static constexpr auto error(expected<T, E>&& v) -> unexpected<E> { return unexpected{std::move(v.error())}; }
};

template<typename T>
struct try_helper<const T> : try_helper<T> {};

template<typename T>
struct try_helper<volatile T> : try_helper<T> {};

template<typename T>
struct try_helper<const volatile T> : try_helper<T> {};

}  // namespace stf::detail

#define TRYX_NUMBERED(_no, ...)                                                               \
    ({                                                                                        \
        auto _res_##_no = (__VA_ARGS__);                                                      \
        using _helper_type_##_no = ::stf::detail::try_helper<decltype(_res_##_no)>; \
        if (!_helper_type_##_no::has_value(_res_##_no)) {                                     \
            return _helper_type_##_no::error(std::move(_res_##_no));                          \
        }                                                                                     \
        _helper_type_##_no::value(std::move(_res_##_no));                                     \
    })

#define TRYX(...)                                                                 \
    ({                                                                            \
        auto _res = (__VA_ARGS__);                                                \
        using _helper_type = ::stf::detail::try_helper<decltype(_res)>; \
        if (!_helper_type::has_value(_res)) {                                     \
            return _helper_type::error(std::move(_res));                          \
        }                                                                         \
        _helper_type::value(std::move(_res));                                     \
    })
