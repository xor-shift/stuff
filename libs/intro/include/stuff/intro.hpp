#pragma once

#include <stuff/intro/aggregate.hpp>
#include <stuff/intro/concepts.hpp>

namespace stf::intro {

namespace detail {

template<typename T>
concept has_adl_introspector = requires(T const& v) { _stf_adl_introspector(v); };

}  // namespace detail

template<typename T>
struct introspector;

template<typename T>
    requires(detail::has_adl_introspector<T>)
struct introspector<T> : std::type_identity<decltype(_stf_adl_introspector(std::declval<T const&>()))> {};

template<typename T>
    requires(std::is_aggregate_v<T> && !detail::has_adl_introspector<T>)
struct introspector<T> : std::type_identity<detail::aggregate_introspector<T>> {};

template<typename T>
using introspector_t = typename introspector<std::remove_cvref_t<T>>::type;

template<typename T>
inline static constexpr bool has_introspector = requires { typename introspector<T>::type; };

}  // namespace stf::intro

#define STF_INTRO_HAS_BEEN_INCLUDED
