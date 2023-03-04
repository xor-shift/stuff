#pragma once

#include <stuff/paeno/detail/stack.hpp>
#include <stuff/paeno/types.hpp>

#include <type_traits>
#include <utility>

namespace stf::paeno::detail {

template<typename T, typename Stack>
struct can_push;

template<typename T, typename Stack>
inline constexpr bool can_push_v = can_push<T, Stack>::value;

template<typename Push, typename Stack>
struct form_stack;

template<typename T>
using form_stack_t = typename form_stack<T, type_stack<>>::type;
//using form_stack_t = decltype(form_stack(std::declval<T>(), type_stack{}));

template<typename T>
struct from_stack;

template<typename Stack>
using from_stack_t = typename from_stack<Stack>::type;

template<typename T>
using simplify_t = from_stack_t<form_stack_t<T>>;

}  // namespace stf::paeno::detail

#include <stuff/paeno/detail/simplify.ipp>
