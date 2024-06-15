#pragma once

#include <exception>
#include <functional>
#include <memory>

#include <stuff/scope/detail/scope_guard.ipp>

namespace stf {

template<typename EF>
struct scope_exit : detail::scope_base<EF> {
    template<typename Fn>
        requires(std::is_constructible_v<detail::scope_base<EF>, detail::scope_type, Fn>)
    constexpr scope_exit(Fn&& fn)  //
      noexcept(noexcept(detail::scope_base<EF>(detail::scope_type(), std::forward<Fn>(fn))))
        : detail::scope_base<EF>(detail::scope_type::scope_exit, std::forward<Fn>(fn)) {}
};

template<class EF>
scope_exit(EF) -> scope_exit<EF>;

template<typename EF>
struct scope_fail : detail::scope_base<EF> {
    template<typename Fn>
        requires(std::is_constructible_v<detail::scope_base<EF>, detail::scope_type, Fn>)
    constexpr scope_fail(Fn&& fn)  //
      noexcept(noexcept(detail::scope_base<EF>(detail::scope_type(), std::forward<Fn>(fn))))
        : detail::scope_base<EF>(detail::scope_type::scope_fail, std::forward<Fn>(fn)) {}
};

template<class EF>
scope_fail(EF) -> scope_fail<EF>;

template<typename EF>
struct scope_success : detail::scope_base<EF> {
    template<typename Fn>
        requires(std::is_constructible_v<detail::scope_base<EF>, detail::scope_type, Fn>)
    constexpr scope_success(Fn&& fn)  //
      noexcept(noexcept(detail::scope_base<EF>(detail::scope_type(), std::forward<Fn>(fn))))
        : detail::scope_base<EF>(detail::scope_type::scope_success, std::forward<Fn>(fn)) {}
};

template<class EF>
scope_success(EF) -> scope_success<EF>;

}  // namespace stf::scope
