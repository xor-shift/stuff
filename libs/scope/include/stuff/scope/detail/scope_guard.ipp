#pragma once

namespace stf::scope::detail {

template<typename Fn>
    requires std::is_nothrow_move_constructible_v<Fn> &&  //
             std::is_nothrow_copy_constructible_v<Fn> &&  //
             std::is_nothrow_invocable_v<Fn>
struct simple_guard {
    constexpr simple_guard(Fn&& fn) noexcept
        : m_no_init(true) {
        if constexpr (std::is_lvalue_reference_v<Fn>) {
            std::construct_at(&m_fn, fn);
        } else {
            std::construct_at(&m_fn, std::forward<Fn>(fn));
        }
    }

    constexpr ~simple_guard() noexcept {
        if (m_armed) {
            m_fn();
        }
    }

    constexpr void release() { m_armed = false; }

private:
    union {
        Fn m_fn;
        bool m_no_init;
    };

    bool m_armed = true;
};

template<typename Fn>
simple_guard(Fn) -> simple_guard<Fn>;

enum class scope_type {
    scope_exit,
    scope_fail,
    scope_success,
};

template<typename EF>
struct scope_base {
    template<typename Fn>
        requires std::is_constructible_v<EF, Fn> && (!std::is_same_v<std::remove_cvref_t<Fn>, scope_base>)
    explicit constexpr scope_base(scope_type scope_type, Fn&& fn)
      noexcept(std::is_nothrow_constructible_v<EF, Fn> || //
               std::is_nothrow_constructible_v<EF, Fn&>)
        : m_scope_type(scope_type)
        , m_no_init(true) {
        if !consteval {
            m_uncaught = std::uncaught_exceptions();
        }

        if constexpr (!std::is_lvalue_reference_v<Fn> && std::is_nothrow_constructible_v<EF, Fn>) {
            std::construct_at(&m_fun, std::forward<Fn>(fn));
        } else if (scope_type != scope_type::scope_success) {
            // "For scope_exit and scope_fail, if the initialization of exit_function throws an exception, calls f()."

            detail::simple_guard guard{[&] { fn(); }};
            std::construct_at(&m_fun, fn);
            guard.disarm();
        } else {
            std::construct_at(&m_fun, fn);
        }

        m_armed = true;
    }

    constexpr scope_base(scope_base&& other)
      noexcept(std::is_nothrow_move_constructible_v<EF> ||//
               std::is_nothrow_copy_constructible_v<EF>)
        : m_scope_type(other.m_scope_type)
        , m_no_init(true)
        , m_armed(other.m_armed)
        , m_uncaught(other.m_uncaught){
        if constexpr (std::is_nothrow_move_constructible_v<EF>) {
            std::construct_at(&m_fun, std::forward<EF>(other.m_fun));
        } else {
            std::construct_at(&m_fun, other.m_fun);
        }

        other.release();
    }

    constexpr scope_base(scope_base const&) = delete;

    constexpr ~scope_base() noexcept {
        int uncaught;

        if consteval {
            uncaught = 0;
        } else {
            uncaught = std::uncaught_exceptions();
        }

        bool execute = false;
        execute |= m_scope_type == scope_type::scope_success && m_uncaught >= uncaught;
        execute |= m_scope_type == scope_type::scope_fail && m_uncaught < uncaught;
        execute |= m_scope_type == scope_type::scope_exit;

        if (execute && m_armed) {
            m_fun();
        }
    }

    constexpr void release() { m_armed = false; }

private:
    scope_type m_scope_type;

    union {
        EF m_fun;
        bool m_no_init;
    };

    bool m_armed = false;
    int m_uncaught = 0;
};

}
