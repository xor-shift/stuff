#pragma once

#include <stuff/scope/scope_guard.hpp>

#include <utility>

namespace stf {

template<typename R, typename D>
struct unique_resource {
private:
    using res_type = std::conditional_t<std::is_reference_v<R>, std::reference_wrapper<std::remove_reference_t<R>>, R>;

    template<typename RR, typename DD>
    static constexpr bool m_noexcept_helper =
      (std::is_nothrow_constructible_v<res_type, RR> || std::is_nothrow_constructible_v<res_type, RR&>)&&  //
      (std::is_nothrow_constructible_v<D, DD> || std::is_nothrow_constructible_v<D, DD&>);

public:
    constexpr unique_resource()                                                                                 //
      noexcept(std::is_nothrow_default_constructible_v<res_type>&& std::is_nothrow_default_constructible_v<D>)  //
        requires(std::is_default_constructible_v<res_type> && std::is_default_constructible_v<D>)
    : m_resource()
    , m_deleter()
    , m_armed(false) {}

    template<typename RR, typename DD>
        requires std::is_constructible_v<res_type, RR> &&                                                        //
                   std::is_constructible_v<D, DD> &&                                                             //
                   (std::is_nothrow_constructible_v<res_type, RR> || std::is_constructible_v<res_type, RR&>) &&  //
                   (std::is_nothrow_constructible_v<D, DD> || std::is_constructible_v<D, DD&>)
    constexpr unique_resource(RR&& r, DD&& d) noexcept(m_noexcept_helper<RR, DD>)
        : m_resource_noinit(true)
        , m_deleter_noinit(true) {
        if constexpr (std::is_nothrow_constructible_v<res_type, RR>) {
            std::construct_at(&m_resource, std::forward<RR>(r));
        } else {
            detail::simple_guard fail_guard([&] mutable noexcept { d(r); });
            std::construct_at(&m_resource, r);
            fail_guard.disarm();
        }

        if constexpr (std::is_nothrow_constructible_v<D, DD>) {
            std::construct_at(&m_deleter, std::forward<DD>(d));
        } else {
            detail::simple_guard fail_guard([&] mutable noexcept { d(m_resource); });
            std::construct_at(&m_deleter, d);
            fail_guard.disarm();
        }

        m_armed = true;
    }

    constexpr unique_resource(unique_resource&& other)           //
      noexcept(std::is_nothrow_move_constructible_v<res_type>&&  //
                 std::is_nothrow_move_constructible_v<D>)
        : m_resource_noinit(true)
        , m_deleter_noinit(true) {
        if constexpr (std::is_nothrow_move_constructible_v<res_type>) {
            std::construct_at(&m_resource, std::move(other.m_resource));
        } else {
            std::construct_at(&m_resource, other.m_resource);  // don't care about exceptions
        }

        if constexpr (std::is_nothrow_move_constructible_v<D>) {
            std::construct_at(&m_deleter, std::move(other.m_deleter));
        } else {
            detail::simple_guard fail_guard([&] mutable noexcept {
                other.m_deleter(m_resource);
                other.release();
            });
            std::construct_at(&m_deleter, other.m_deleter);
            fail_guard.disarm();
        }

        using std::exchange;
        m_armed = exchange(other.m_armed, false);
    }

    constexpr ~unique_resource() noexcept {
        reset();
    }

    constexpr auto operator=(unique_resource&& other)  // vvvv epic formatting here, clang-format
      noexcept(std::is_nothrow_move_assignable_v<res_type>&& std::is_nothrow_move_assignable_v<D>) -> unique_resource& {
        reset();

        if constexpr (std::is_nothrow_move_assignable_v<res_type>) {
            if constexpr (std::is_nothrow_move_assignable_v<D>) {
                m_deleter = std::move(other.m_deleter);
            } else {
                m_deleter = other.m_deleter;
            }

            m_resource = std::move(other.m_resource);
        } else {
            m_resource = other.m_resource;

            if constexpr (std::is_nothrow_move_assignable_v<D>) {
                m_deleter = std::move(other.m_deleter);
            } else {
                m_deleter = other.m_deleter;
            }
        }

        using std::exchange;
        m_armed = exchange(other.m_armed, false);
    }

    constexpr void release() noexcept { m_armed = false; }

    constexpr void reset() noexcept {
        if (m_armed) {
            m_armed = false;
            m_deleter(m_resource);
        }
    }

    template<typename RR>
    constexpr void reset(RR&& r) {
        reset();

        if constexpr (std::is_nothrow_assignable_v<res_type&, RR>) {
            m_resource = std::forward<RR>(r);
        } else {
            m_resource = std::as_const(r);
        }

        m_armed = true;
    }

    constexpr auto get() const noexcept -> const R& { return m_resource; }
    constexpr auto get() noexcept -> R& { return m_resource; }
    constexpr auto get_deleter() const noexcept -> const D& { return m_deleter; }

    constexpr auto operator*() const noexcept -> std::add_lvalue_reference_t<std::remove_pointer_t<R>>
        requires std::is_pointer_v<R> && (!std::is_void_v<std::remove_pointer_t<R>>)
    {
        return *get();
    }

    constexpr auto operator->() const noexcept -> R
        requires std::is_pointer_v<R>
    {
        return m_resource;
    }

private:
    union {
        res_type m_resource;
        bool m_resource_noinit;
    };

    union {
        D m_deleter;
        bool m_deleter_noinit;
    };

    bool m_armed = false;
};

template<class R, class D>
unique_resource(R, D) -> unique_resource<R, D>;

template<class R, class D, class S = std::decay_t<R>>
    requires requires(R&& r, S const& invalid) {
                 { r == invalid } -> std::convertible_to<bool>;
             }
constexpr auto make_unique_resource_checked(R&& resource, S const& invalid, D&& d)  //
  noexcept(std::is_nothrow_constructible_v<std::decay_t<R>, R>&&                    //
             std::is_nothrow_constructible_v<std::decay_t<D>, D>)                   //
  -> unique_resource<std::decay_t<R>, std::decay_t<D>> {
    bool is_invalid = bool(resource == invalid);

    if (is_invalid) {
        return {};
    } else {
        return {std::forward<R>(resource), std::forward<D>(d)};
    }
}

}  // namespace stf::scope
