#pragma once

#include <stuff/intro/aggregate.hpp>
#include <stuff/intro/concepts.hpp>

namespace stf::intro {

namespace detail {

template<typename T>
concept has_adl_introspector = requires(T const& v) { _stf_adl_introspector(v); };

template<typename T>
concept has_member_introspector = requires { typename T::introspector_type; };

}  // namespace detail

template<typename T>
struct introspector;

template<typename T>
    requires(detail::has_adl_introspector<T> && !detail::has_member_introspector<T>)
struct introspector<T> : std::type_identity<decltype(_stf_adl_introspector(std::declval<T const&>()))> {};

template<typename T>
    requires(detail::has_member_introspector<T> && !detail::has_adl_introspector<T>)
struct introspector<T> : std::type_identity<typename T::introspector_type> {};

template<typename T>
    requires(detail::has_member_introspector<T> && detail::has_adl_introspector<T>)
struct introspector<T> {
    static_assert(
      detail::has_member_introspector<T> && detail::has_adl_introspector<T>,
      "A type cannot have both an ADL introspector and a member introspector"
    );
};

template<typename T>
    requires(std::is_aggregate_v<T> && !detail::has_adl_introspector<T> && !detail::has_member_introspector<T>)
struct introspector<T> : std::type_identity<detail::aggregate_introspector<T>> {};

template<typename T>
using introspector_t = typename introspector<T>::type;

}  // namespace stf::intro
