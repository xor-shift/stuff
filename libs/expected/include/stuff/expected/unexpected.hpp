#pragma once

#include <expected>
#include <utility>

namespace stf {

template<typename E>
    requires(!std::is_same_v<void, E>)
struct unexpected {
    constexpr unexpected(const unexpected&) = default;
    constexpr unexpected(unexpected&&) = default;

    template<class Err = E>
        requires(!std::is_same_v<std::remove_cvref_t<Err>, unexpected>) &&       //
                (!std::is_same_v<std::remove_cvref_t<Err>, std::in_place_t>) &&  //
                std::is_constructible_v<E, Err>
    explicit constexpr unexpected(Err&& e) noexcept(std::is_nothrow_constructible_v<E, Err>)
        : m_data(std::forward<Err>(e)) {}

    template<typename... Args>
        requires std::is_constructible_v<E, Args...>
    explicit constexpr unexpected(std::in_place_t, Args&&... ts)
        : m_data(std::forward<Args>(ts)...) {}

    template<typename U, typename... Args>
        requires std::is_constructible_v<E, std::initializer_list<U>&, Args...>
    explicit constexpr unexpected(
      std::in_place_t,
      std::initializer_list<U> il,
      Args&&... args
    ) noexcept(std::is_nothrow_constructible_v<E, std::initializer_list<U>&, Args...>)
        : m_data(il, std::forward<Args>(args)...) {}

    constexpr unexpected(std::unexpected<E>&& other)
        : m_data(std::move(other).error()) {
    }

    constexpr unexpected(std::unexpected<E> const& other)
        : m_data(other.error()) {
    }

    constexpr operator std::unexpected<E>() && {
        return std::unexpected<E>(std::move(m_data));
    }

    constexpr operator std::unexpected<E>() const& {
        return std::unexpected<E>(m_data);
    }

    constexpr auto operator=(unexpected const&) -> unexpected& = default;
    constexpr auto operator=(unexpected&&) -> unexpected& = default;

    [[nodiscard]] constexpr auto error() const& noexcept -> const E& { return m_data; };
    [[nodiscard]] constexpr auto error() & noexcept -> E& { return m_data; };
    [[nodiscard]] constexpr auto error() const&& noexcept -> const E&& { return std::move(m_data); };
    [[nodiscard]] constexpr auto error() && noexcept -> E&& { return std::move(m_data); };

    constexpr void swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>) {
        using std::swap;
        swap(m_data, other.m_data);
    }

    template<typename E2>
    friend constexpr bool operator==(unexpected const& x, unexpected<E2> const& y);

    template<typename E2>
    friend constexpr void swap(unexpected& x, unexpected<E2>& y) noexcept(noexcept(x.swap(y)));

private:
    E m_data;
};

template<typename E>
unexpected(E) -> unexpected<E>;

template<typename E1, typename E2>
constexpr bool operator==(unexpected<E1> const& x, unexpected<E2> const& y) {
    return x.error() == y.error();
}

template<typename E1, typename E2>
constexpr void swap(unexpected<E1>& x, unexpected<E2>& y) noexcept(noexcept(x.swap(y))) {
    return x.swap(y);
}

template<typename T>
struct is_unexpected : std::bool_constant<false> {};

template<typename T>
struct is_unexpected<unexpected<T>> : std::bool_constant<true> {};

template<typename T>
struct is_unexpected<std::unexpected<T>> : std::bool_constant<true> {};

template<typename T>
struct is_unexpected<const T> : is_unexpected<T> {};

template<typename T>
struct is_unexpected<volatile T> : is_unexpected<T> {};

template<typename T>
struct is_unexpected<const volatile T> : is_unexpected<T> {};

template<typename T>
static constexpr bool is_unexpected_v = is_unexpected<T>::value;

}  // namespace stf
