#pragma once

#include <stuff/expected/detail/exception.hpp>
#include <stuff/expected/unexpected.hpp>
#include <stuff/scope/scope_guard.hpp>

#include <exception>
#include <expected>
#include <memory>
#include <variant>

namespace stf {

struct unexpect_t {};

template<typename T, typename E>
struct expected;

template<typename T, typename E>
    requires(!std::is_void_v<E>) && (!std::is_void_v<T>)
struct expected<T, E> {
    // FIXME: clang's overload resolution is fucky, we can't make use of implicitly deleted default functions.
    // as a result, this class is not standards compliant and can never be a trivial type. fixing this issue should be
    // fairly easy after clang 16.

    // TODO: add requirements against triviality of storage_value_type and storage_unexpect_type after the issue above
    // is solved.

private:
    template<typename OtherT, typename OtherE>
    static constexpr bool different_expected_helper =                             //
      std::is_constructible_v<T, expected<OtherT, OtherE>&> ||                    //
      std::is_constructible_v<T, expected<OtherT, OtherE>> ||                     //
      std::is_constructible_v<T, expected<OtherT, OtherE> const&> ||              //
      std::is_constructible_v<T, const expected<OtherT, OtherE>> ||               //
      std::is_constructible_v<expected<OtherT, OtherE>, T> ||                     //
      std::is_constructible_v<expected<OtherT, OtherE>&, T> ||                    //
      std::is_constructible_v<expected<OtherT, OtherE> const&, T> ||              //
      std::is_constructible_v<const expected<OtherT, OtherE>, T> ||               //
      std::is_constructible_v<unexpected<E>, expected<OtherT, OtherE>&> ||        //
      std::is_constructible_v<unexpected<E>, expected<OtherT, OtherE>> ||         //
      std::is_constructible_v<unexpected<E>, expected<OtherT, OtherE> const&> ||  //
      std::is_constructible_v<unexpected<E>, const expected<OtherT, OtherE>>;

    template<typename OtherT, typename OtherE>
    static constexpr bool different_expected_explicit_helper =  //
      !std::is_convertible_v<OtherT, T> ||                      //
      !std::is_convertible_v<OtherE, E>;

public:
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;

    template<typename U>
    using rebind = expected<U, E>;

    constexpr expected(std::expected<T, E>&& other)
        : m_has_value((bool)other) {
        if (m_has_value) {
            std::construct_at(std::addressof(m_value), std::move(other).value());
        } else {
            std::construct_at(std::addressof(m_unexpected), std::move(other).error());
        }
    }

    constexpr expected(std::expected<T, E> const& other)
        : m_has_value((bool)other) {
        if (m_has_value) {
            std::construct_at(std::addressof(m_value), other.value());
        } else {
            std::construct_at(std::addressof(m_unexpected), other.error());
        }
    }

    constexpr expected(std::unexpected<E>&& unexpect)
        : expected(stf::unexpected(std::move(unexpect).error())) {
    }

    constexpr expected(std::unexpected<E> const& unexpect)
        : expected(stf::unexpected(unexpect.error())) {
    }

    constexpr operator std::expected<T, E>() {
        if (has_value()) {
            return std::move(m_value);
        }

        return std::unexpected<E>(std::move(m_unexpected));
    }

    // clang-format off

    constexpr expected()
        noexcept(std::is_nothrow_default_constructible_v<T>)
        requires std::is_default_constructible_v<T>
    : m_value()
    , m_has_value(true) {}

    constexpr expected(expected const&) = default;

    constexpr expected(expected const& other)
        noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>)
        requires std::is_copy_constructible_v<T> && std::is_copy_constructible_v<E> &&
                 (!std::is_trivially_copy_constructible_v<T> || !std::is_trivially_copy_constructible_v<E>)
    : m_has_value(other.m_has_value) {
        if (m_has_value) {
            std::construct_at(std::addressof(m_value), other.m_value);
        } else {
            std::construct_at(std::addressof(m_unexpected), other.m_unexpected);
        }
    }

    constexpr expected(expected&& other) = default;

    constexpr expected(expected&& other)
        noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_constructible_v<E>)
        requires std::is_move_constructible_v<T> && std::is_move_constructible_v<E> &&
                 (!std::is_trivially_move_constructible_v<T> || !std::is_trivially_move_constructible_v<E>)
    : m_has_value(other.m_has_value) {
        if (m_has_value) {
            std::construct_at(std::addressof(m_value), std::move(other).m_value);
        } else {
            std::construct_at(std::addressof(m_unexpected), std::move(other).m_unexpected);
        }
    }

    template<typename OtherT, typename OtherE>
        requires std::is_constructible_v<T, OtherT const&> &&
                 std::is_constructible_v<E, OtherE const&> &&
                 (!different_expected_helper<OtherT, OtherE>)
        explicit(different_expected_explicit_helper<OtherT const&, OtherE const&>)
    constexpr expected(expected<OtherT, OtherE> const& other)
        noexcept(std::is_nothrow_constructible_v<T, OtherT const&> && std::is_nothrow_constructible_v<T, OtherE const&>)
    : m_has_value(other.m_has_value) {
        if (m_has_value) {
            std::construct_at(std::addressof(m_value), other.m_value);
        } else {
            std::construct_at(std::addressof(m_unexpected), other.m_unexpected);
        }
    }

    template<typename OtherT, typename OtherE>
        requires std::is_constructible_v<T, OtherT> &&
                 std::is_constructible_v<E, OtherE> &&
                 (!different_expected_helper<OtherT, OtherE>)
        explicit(different_expected_explicit_helper<OtherT, OtherE>)
    constexpr expected(expected<OtherT, OtherE>&& other)
        noexcept(std::is_nothrow_constructible_v<T, OtherT> && std::is_nothrow_constructible_v<T, OtherE>)
    : m_has_value(other.m_has_value) {
        if (m_has_value) {
            std::construct_at(std::addressof(m_value), std::move(other).m_value);
        } else {
            std::construct_at(std::addressof(m_unexpected), std::move(other).m_unexpected);
        }
    }

    template<typename OtherT = T>
        requires (!std::is_same_v<std::remove_cvref_t<OtherT>, expected>) &&
                 (!std::is_same_v<std::remove_cvref_t<OtherT>, std::in_place_t>) &&
                 (!is_unexpected_v<OtherT>) &&
                 std::is_constructible_v<T, OtherT>
        explicit(!std::is_convertible_v<OtherT, T>)
    constexpr expected(OtherT&& v)
        noexcept(std::is_nothrow_constructible_v<T, OtherT>)
    : m_value(std::forward<OtherT>(v))
    , m_has_value(true) {}

    template<typename OtherE>
        requires std::is_constructible_v<E, OtherE const&>
        explicit(!std::is_convertible_v<OtherE const&, E>)
    constexpr expected(unexpected<OtherE> const& v)
        noexcept(std::is_nothrow_constructible_v<E, OtherE const&>)
    : m_unexpected(v.error())
    , m_has_value(false) {}

    template<typename OtherE>
        requires std::is_constructible_v<E, OtherE>
        explicit(!std::is_convertible_v<OtherE, E>)
    constexpr expected(unexpected<OtherE>&& v)
        noexcept(std::is_nothrow_constructible_v<E, OtherE>)
    : m_unexpected(std::move(v).error())
    , m_has_value(false) {}

    template<typename... Args>
        requires std::is_constructible_v<T, Args...>
        explicit
    constexpr expected(std::in_place_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args...>)
    : m_value(std::forward<Args>(args)...)
    , m_has_value(true) {}

    template<typename U, typename... Args>
        requires std::is_constructible_v<T, std::initializer_list<U>&, Args...>
        explicit
    constexpr expected(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>)
    : m_value(il, std::forward<Args>(args)...)
    , m_has_value(true) {}

    template<typename... Args>
        requires std::is_constructible_v<E, Args...>
        explicit
    constexpr expected(unexpect_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args...>)
    : m_unexpected(std::forward<Args>(args)...)
    , m_has_value(false) {}

    template<typename U, typename... Args>
        requires std::is_constructible_v<E, std::initializer_list<U>&, Args...>
        explicit
    constexpr expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, std::initializer_list<U>&, Args...>)
    : m_unexpected(il, std::forward<Args>(args)...)
    , m_has_value(false) {}

    // dontFIXME: this should be fixed by clang 16 (?)
    // update from the future (270823): it has been fixed
    // i will slowly move to std::expected now
    constexpr ~expected() = default;

    constexpr ~expected()
        requires (!std::is_trivially_destructible_v<T>) || (!std::is_trivially_destructible_v<E>)
    {
        if (m_has_value) {
            std::destroy_at(std::addressof(m_value));
        } else {
            std::destroy_at(std::addressof(m_unexpected));
        }

        // a little protection for stack-use-after-scope or heap-use-after-free

        if (sizeof(T) > sizeof(E)) {
            std::fill(std::begin(m_fill_t), std::end(m_fill_t), '\0');
        } else {
            std::fill(std::begin(m_fill_e), std::end(m_fill_e), '\0');
        }
    }

    constexpr expected& operator=(expected const& other)
        noexcept(
            std::is_nothrow_copy_constructible_v<T> &&
            std::is_nothrow_copy_constructible_v<E> &&
            std::is_nothrow_copy_assignable_v<T> &&
            std::is_nothrow_copy_assignable_v<E>)
        requires std::is_copy_constructible_v<T> &&
                 std::is_copy_assignable_v<T> &&
                 std::is_copy_constructible_v<E> &&
                 std::is_copy_assignable_v<E> &&
                 (std::is_nothrow_move_constructible_v<T> || std::is_nothrow_move_constructible_v<E>) // why
    {
        if (other.m_has_value) {
            assign<true>(other.m_value);
        } else {
            assign<false>(other.m_unexpected);
        }

        return *this;
    }

    constexpr expected& operator=(expected&& other)
        noexcept(
            std::is_nothrow_move_constructible_v<T> &&
            std::is_nothrow_move_constructible_v<E> &&
            std::is_nothrow_move_assignable_v<T> &&
            std::is_nothrow_move_assignable_v<E>)
        requires std::is_move_constructible_v<T> &&
                 std::is_move_assignable_v<T> &&
                 std::is_move_constructible_v<E> &&
                 std::is_move_assignable_v<E> &&
                 (std::is_nothrow_move_constructible_v<T> || std::is_nothrow_move_constructible_v<E>) // why
    {
        if (other.m_has_value) {
            assign<true>(std::move(other.m_value));
        } else {
            assign<false>(std::move(other.m_unexpected));
        }

        return *this;
    }

    template<typename U = T>
        requires (!std::is_same_v<expected, std::remove_cvref_t<U>>) &&
                 (!is_unexpected_v<U>) &&
                 std::is_constructible_v<T, U> &&
                 std::is_assignable_v<T&, U> &&
                 (std::is_nothrow_constructible_v<T, U> ||
                  std::is_nothrow_move_constructible_v<T> ||
                  std::is_nothrow_move_constructible_v<E>)
    constexpr expected& operator=(U&& v) {
        assign<true>(std::forward<U>(v));
        return *this;
    }

    template<typename U>
        requires std::is_constructible_v<E, U const&> &&
                 std::is_assignable_v<E&, U const&> &&
                 (std::is_nothrow_constructible_v<E, U const&> ||
                  std::is_nothrow_move_constructible_v<T> ||
                  std::is_nothrow_move_constructible_v<E>)
    constexpr expected& operator=(unexpected<U> const& v) {
        assign<false>(v.error());
        return *this;
    }

    template<typename U>
        requires std::is_constructible_v<E, U> &&
                 std::is_assignable_v<E&, U> &&
                 (std::is_nothrow_constructible_v<E, U> ||
                  std::is_nothrow_move_constructible_v<T> ||
                  std::is_nothrow_move_constructible_v<E>)
    constexpr expected& operator=(unexpected<U>&& v) {
        assign<false>(std::move(v).error());
        return *this;
    }

    constexpr void swap(expected& other)
        noexcept(
            std::is_nothrow_move_constructible_v<T> &&
            std::is_nothrow_move_constructible_v<E> &&
            std::is_nothrow_swappable_v<T&> &&
            std::is_nothrow_swappable_v<E&>)
        requires
            std::is_swappable_v<T> &&
            std::is_swappable_v<E> &&
            std::is_move_constructible_v<T> &&
            std::is_move_constructible_v<E> &&
            (std::is_nothrow_move_constructible_v<T> ||
             std::is_nothrow_move_constructible_v<E>)
    {
        // stub
    }

    // clang-format on

    // functions are less complex in their signatures from here, hence no manual formatting

    template<typename... Args>
        requires std::is_nothrow_constructible_v<T, Args...>
    constexpr T& emplace(Args&&... args) noexcept {
        if (m_has_value) {
            std::destroy_at(std::addressof(m_value));
        } else {
            std::destroy_at(std::addressof(m_unexpected));
        }

        std::construct_at(std::addressof(m_value), std::forward<Args>(args)...);
        m_has_value = true;
    }

    template<typename U, typename... Args>
        requires std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>
    constexpr T& emplace(std::initializer_list<U> il, Args&&... args) noexcept {
        if (m_has_value) {
            std::destroy_at(std::addressof(m_value));
        } else {
            std::destroy_at(std::addressof(m_unexpected));
        }

        std::construct_at(std::addressof(m_value), il, std::forward<Args>(args)...);
        m_has_value = true;
    }

    constexpr auto operator->() const noexcept -> const T* { return std::addressof(m_value); }
    constexpr auto operator->() noexcept -> T* { return std::addressof(m_value); }
    constexpr auto operator*() const& noexcept -> T const& { return m_value; }
    constexpr auto operator*() & noexcept -> T& { return m_value; }
    constexpr auto operator*() const&& noexcept -> T const&& { return std::move(m_value); }
    constexpr auto operator*() && noexcept -> T&& { return std::move(m_value); }
    constexpr operator bool() const noexcept { return m_has_value; }
    constexpr auto has_value() const noexcept -> bool { return m_has_value; }
    constexpr auto error() const& noexcept -> E const& { return m_unexpected; }
    constexpr auto error() & noexcept -> E& { return m_unexpected; }
    constexpr auto error() const&& noexcept -> E const&& { return std::move(m_unexpected); }
    constexpr auto error() && noexcept -> E&& { return std::move(m_unexpected); }

#pragma push_macro("VALUE_FACTORY")
#define VALUE_FACTORY(_modifier, _ret_expr)             \
    constexpr auto value() _modifier->T _modifier {     \
        if (!has_value()) {                             \
            throw bad_expected_access<E>(m_unexpected); \
        }                                               \
                                                        \
        return _ret_expr;                               \
    }

    VALUE_FACTORY(const&, m_value)
    VALUE_FACTORY(&, m_value)
    VALUE_FACTORY(const&&, std::move(m_value))
    VALUE_FACTORY(&&, std::move(m_value))

#undef VALUE_FACTORY
#pragma pop_macro("VALUE_FACTORY")

    template<typename U>
    constexpr auto value_or(U&& v) const&                                                           //
      noexcept(std::is_nothrow_copy_constructible_v<T>&& std::is_nothrow_convertible_v<U, T>) -> T  //
        requires std::is_copy_constructible_v<T> && std::is_convertible_v<U, T>
    {
        return m_has_value ? **this : static_cast<T>(std::forward<U>(v));
    }

    template<typename U>
    constexpr auto value_or(U&& v) &&  //
      noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_convertible_v<U, T>) -> T
        requires std::is_move_constructible_v<T> && std::is_convertible_v<U, T>
    {
        return m_has_value ? std::move(**this) : static_cast<T>(std::forward<U>(v));
    }

    template<class T2, class E2>
        requires(!std::is_void_v<T2>) && std::equality_comparable_with<T, T2> &&
                std::equality_comparable_with<E, E2>
                friend constexpr auto operator==(expected const& lhs, expected<T2, E2> const& rhs)  //
                noexcept(noexcept(*lhs == *rhs) && noexcept(lhs.error() == rhs.error())) -> bool {
        if (lhs.has_value() != rhs.has_value())
            return false;

        if (lhs.has_value()) {
            return *lhs == *rhs;
        } else {
            return lhs.error() == rhs.error();
        }
    }

    template<class T2, class E2>
        requires std::is_void_v<T2> && std::equality_comparable_with<E, E2>
    friend constexpr bool operator==(expected const& lhs, expected<T2, E2> const& rhs)  //
      noexcept(noexcept(lhs.error() == rhs.error())) {
        if (lhs.has_value() || rhs.has_value())
            return false;

        return lhs.error() == rhs.error();
    }

    template<class T2>
    // requires std::equality_comparable_with<T, T2>
    //  FIXME: the constraint above causes an infinite recursion, can't be arsed to diagnose it ATM, please fix this
    //  future me
    friend constexpr bool operator==(expected const& lhs, T2 const& rhs)  //
      noexcept(noexcept(*lhs == rhs)) {
        if (!lhs.has_value())
            return false;

        return *lhs == rhs;
    }

    template<class E2>
        requires std::equality_comparable_with<E, E2>
    friend constexpr bool operator==(expected const& lhs, unexpected<E2> const& rhs)  //
      noexcept(noexcept(lhs.error() == rhs.error())) {
        if (lhs.has_value())
            return false;

        return lhs.error() == rhs.error();
    }

private:
    union {
        T m_value;
        E m_unexpected;

        char m_fill_t[sizeof(T)];
        char m_fill_e[sizeof(E)];
    };

    bool m_has_value;

    template<bool IsValue, typename U>
    void assign(U&& v) {
        using prev_type = std::conditional_t<IsValue, E, T>;
        prev_type* prev = nullptr;
        if constexpr (IsValue) {
            prev = std::addressof(m_unexpected);
        } else {
            prev = std::addressof(m_value);
        }

        using target_type = std::conditional_t<IsValue, T, E>;
        target_type* target = nullptr;
        if constexpr (IsValue) {
            target = std::addressof(m_value);
        } else {
            target = std::addressof(m_unexpected);
        }

        if (IsValue == m_has_value) {
            *target = std::forward<U>(v);
        } else {
            if constexpr (std::is_nothrow_constructible_v<target_type, U>) {
                std::destroy_at(prev);
                std::construct_at(target, std::move(v));
            } else if constexpr (std::is_nothrow_move_constructible_v<target_type>) {
                target_type t{std::forward<U>(v)};  // possible exception
                std::destroy_at(prev);
                std::construct_at(target, std::move(t));
            } else {
                static_assert(std::is_nothrow_move_constructible_v<prev_type>);

                prev_type prev_backup;
                std::construct_at(&prev_backup, std::move(*prev));
                std::destroy_at(prev);

                detail::simple_guard guard([&] noexcept { std::construct_at(prev, std::move(prev_backup)); });

                std::construct_at(target, std::forward<U>(v));  // possible exception

                guard.release();
            }
        }

        m_has_value = IsValue;
    }
};

template<typename T, typename E>
    requires(!std::is_void_v<E>) && std::is_void_v<T>
struct expected<T, E> {
private:
    template<typename OtherT, typename OtherE>
    static constexpr bool different_expected_helper =                             //
      std::is_constructible_v<unexpected<E>, expected<OtherT, OtherE>&> ||        //
      std::is_constructible_v<unexpected<E>, expected<OtherT, OtherE>> ||         //
      std::is_constructible_v<unexpected<E>, expected<OtherT, OtherE> const&> ||  //
      std::is_constructible_v<unexpected<E>, const expected<OtherT, OtherE>>;

public:
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;

    template<typename U>
    using rebind = expected<U, E>;

    constexpr expected() noexcept
        : m_value()
        , m_has_value(true) {}

    template<typename Other>
    constexpr expected(std::expected<void, E>&& other)
        : m_has_value((bool)other) {
        if (m_has_value) {
            std::construct_at(std::addressof(m_value));
        } else {
            std::construct_at(std::addressof(m_unexpected), std::move(other).error());
        }
    }

    template<typename Other>
    constexpr expected(std::expected<void, E> const& other)
        : m_has_value((bool)other) {
        if (m_has_value) {
            std::construct_at(std::addressof(m_value));
        } else {
            std::construct_at(std::addressof(m_unexpected), other.error());
        }
    }

    constexpr expected(std::unexpected<E>&& unexpect)
        : expected(stf::unexpected(std::move(unexpect)).error()) {
    }

    constexpr expected(std::unexpected<E> const& unexpect)
        : expected(stf::unexpected(unexpect.error())) {
    }

    constexpr operator std::expected<void, E>() {
        if (has_value()) {
            return {};
        }

        return std::unexpected<E>(std::move(m_unexpected));
    }

    // clang-format off

    constexpr expected(expected const& other) = default;

    constexpr expected(expected const& other)
        noexcept(std::is_nothrow_copy_constructible_v<E>)
        requires std::is_copy_constructible_v<E> && (!std::is_trivially_copy_constructible_v<E>)
    : m_value()
    , m_has_value(other.m_has_value) {
        if (!m_has_value) {
            std::construct_at(std::addressof(m_unexpected), other.m_unexpected);
        }
        // else, m_value is initialised
    }

    constexpr expected(expected&& other) = default;

    constexpr expected(expected&& other)
        noexcept(std::is_nothrow_move_constructible_v<E>)
        requires std::is_move_constructible_v<E> && (!std::is_trivially_move_constructible_v<E>)
    : m_value()
    , m_has_value(other.m_has_value) {
        if (!m_has_value) {
            std::construct_at(std::addressof(m_unexpected), std::move(other).m_unexpected);
        }
    }

    template<typename OtherT, typename OtherE>
        requires std::is_void_v<OtherT> &&
                 std::is_constructible_v<E, OtherE const&> &&
                 (!different_expected_helper<OtherT, OtherE>)
        explicit(!std::is_convertible_v<OtherE const&, E>)
    constexpr expected(expected<OtherT, OtherE> const& other)
    : m_value()
    , m_has_value(other.m_has_value) {
        if (!m_has_value) {
            std::construct_at(std::addressof(m_unexpected), other.m_unexpected);
        }
    }

    template<typename OtherT, typename OtherE>
        requires std::is_void_v<OtherT> &&
                 std::is_constructible_v<E, OtherE> &&
                 (!different_expected_helper<OtherT, OtherE>)
        explicit(!std::is_convertible_v<OtherE, E>)
    constexpr expected(expected<OtherT, OtherE>&& other)
    : m_value()
    , m_has_value(other.m_has_value) {
        if (!m_has_value) {
            std::construct_at(std::addressof(m_unexpected), other.m_unexpected);
        }
    }

    template<typename OtherE = E>
        requires std::is_constructible_v<E, OtherE const&>
        explicit(!std::is_convertible_v<OtherE const&, E>)
    constexpr expected(unexpected<OtherE> const& other)
    : m_unexpected(other.error())
    , m_has_value(false) {}

    template<typename OtherE = E>
        requires std::is_constructible_v<E, OtherE>
        explicit(!std::is_convertible_v<OtherE, E>)
    constexpr expected(unexpected<OtherE>&& other)
    : m_unexpected(std::move(other).error())
    , m_has_value(false) {}

    template<typename... Args>
        requires std::is_constructible_v<E, Args...>
        explicit
    constexpr expected(unexpect_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args...>)
    : m_unexpected(std::forward<Args>(args)...)
    , m_has_value(false) {}

    template<typename U, typename... Args>
        requires std::is_constructible_v<E, std::initializer_list<U>&, Args...>
        explicit
    constexpr expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, std::initializer_list<U>&, Args...>)
    : m_unexpected(il, std::forward<Args>(args)...)
    , m_has_value(false) {}

    explicit constexpr expected(std::in_place_t) noexcept
    : m_value()
    , m_has_value(true) {}

    // dontFIXME: read the fixme above for non-void T specialized `expected`
    constexpr ~expected() = default;

    constexpr ~expected()
        noexcept(std::is_nothrow_destructible_v<E>)
        requires (!std::is_trivially_destructible_v<E>)
    {
        if (!m_has_value) {
            std::destroy_at(std::addressof(m_unexpected));
        }
    }

    constexpr auto operator=(expected const& other)
        noexcept(std::is_nothrow_copy_constructible_v<E>&& std::is_nothrow_copy_assignable_v<E>) -> expected&
        requires std::is_copy_constructible_v<E> && std::is_copy_assignable_v<E>
    {
        if (other.m_has_value) {
            assign<true>(other.m_value);
        } else {
            assign<false>(other.m_unexpected);
        }

        return *this;
    }

    constexpr auto operator=(expected&& other)
        noexcept(std::is_nothrow_move_constructible_v<E>&& std::is_nothrow_move_assignable_v<E>) -> expected&
        requires std::is_move_constructible_v<E> && std::is_move_assignable_v<E>
    {
        if (other.m_has_value) {
            assign<true>(std::move(other.m_value));
        } else {
            assign<false>(std::move(other.m_unexpected));
        }

        return *this;
    }

    template<typename OtherE>
    constexpr auto operator=(unexpected<OtherE> const& other)
        noexcept(std::is_nothrow_copy_constructible_v<E>&& std::is_nothrow_copy_assignable_v<E>) -> expected&
        requires std::is_copy_constructible_v<E> && std::is_copy_assignable_v<E>
    {
        assign<false>(other.error());

        return *this;
    }

    template<typename OtherE>
    constexpr auto operator=(unexpected<OtherE>&& other)
        noexcept(std::is_nothrow_copy_constructible_v<E>&& std::is_nothrow_copy_assignable_v<E>) -> expected&
        requires std::is_copy_constructible_v<E> && std::is_copy_assignable_v<E>
    {
        assign<false>(std::move(other.error()));

        return *this;
    }

    constexpr void swap(expected& other)
        noexcept(std::is_nothrow_swappable_v<E&> && std::is_nothrow_move_constructible_v<E>)
        requires std::is_swappable_v<E> && std::is_move_constructible_v<E>
    {
        // STUB
    }

    // clang-format on

    constexpr void emplace() noexcept { assign<true>(); }

    constexpr explicit operator bool() const noexcept { return m_has_value; }
    constexpr auto has_value() const noexcept -> bool { return m_has_value; }
    constexpr auto error() const& noexcept -> E const& { return m_unexpected; }
    constexpr auto error() & noexcept -> E& { return m_unexpected; }
    constexpr auto error() const&& noexcept -> E const&& { return std::move(m_unexpected); }
    constexpr auto error() && noexcept -> E&& { return std::move(m_unexpected); }

#pragma push_macro("VALUE_FACTORY")
#define VALUE_FACTORY(_modifier)                        \
    constexpr void value() _modifier {                  \
        if (!has_value()) {                             \
            throw bad_expected_access<E>(m_unexpected); \
        }                                               \
                                                        \
        return;                                         \
    }

    VALUE_FACTORY(const&)
    VALUE_FACTORY(&)
    VALUE_FACTORY(const&&)
    VALUE_FACTORY(&&)

#undef VALUE_FACTORY
#pragma pop_macro("VALUE_FACTORY")

private:
    union {
        struct {
        } m_value;
        E m_unexpected;

        char m_fill[sizeof(E)];
    };

    bool m_has_value;

    template<bool IsValue, typename U>
    constexpr void assign(U&& v)  //
      noexcept(IsValue ? std::is_nothrow_destructible_v<E> ://
                         std::is_nothrow_constructible_v<E, U> && std::is_nothrow_assignable_v<E, U>)  //
    {
        if constexpr (IsValue) {
            if (!m_has_value) {
                std::destroy_at(std::addressof(m_unexpected));
            }

            m_value = {};
        } else {
            if (m_has_value) {
                std::construct_at(std::addressof(m_unexpected), std::forward<U>(v));
            } else {
                m_unexpected = std::forward<U>(v);
            }
        }

        m_has_value = IsValue;
    }
};

}  // namespace stf
