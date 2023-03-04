#pragma once

namespace stf::paeno::detail {

// simplification helpers

template<typename T, typename Stack>
struct can_push<succ<T>, Stack>
    : std::bool_constant<std::is_same_v<top_t<Stack>, succ<null>> || std::is_same_v<top_t<Stack>, void>> {};

template<typename T, typename Stack>
struct can_push<pred<T>, Stack>
    : std::bool_constant<std::is_same_v<top_t<Stack>, pred<null>> || std::is_same_v<top_t<Stack>, void>> {};

// forming a stack out of an integer

template<typename Stack>
struct form_stack<null, Stack> : std::type_identity<Stack> {};

template<typename Nested, typename Stack>
struct form_stack<succ<Nested>, Stack> {
    using if_push = typename form_stack<Nested, push_t<Stack, succ<null>>>::type;
    using if_pop = typename form_stack<Nested, pop_t<Stack>>::type;

    using type = std::conditional_t<can_push_v<succ<Nested>, Stack>, if_push, if_pop>;
};

template<typename Nested, typename Stack>
struct form_stack<pred<Nested>, Stack> {
    using if_push = typename form_stack<Nested, push_t<Stack, pred<null>>>::type;
    using if_pop = typename form_stack<Nested, pop_t<Stack>>::type;

    using type = std::conditional_t<can_push_v<pred<Nested>, Stack>, if_push, if_pop>;
};

// forming an integer out of a stack

template<typename... Ts>
struct from_stack<type_stack<succ<null>, Ts...>>
    : std::type_identity<succ<typename from_stack<type_stack<Ts...>>::type>> {};

template<typename... Ts>
struct from_stack<type_stack<pred<null>, Ts...>>
    : std::type_identity<pred<typename from_stack<type_stack<Ts...>>::type>> {};

template<>
struct from_stack<type_stack<>> : std::type_identity<null> {};

}  // namespace stf::paeno::detail
