#pragma once

namespace stf::paeno::detail {

template<typename... Ts>
struct type_stack;

template<>
struct type_stack<> {
    using top = void;

    using pop = type_stack<>;

    template<typename U>
    using push = type_stack<U>;
};

template<typename T, typename... Ts>
struct type_stack<T, Ts...> {
    using top = T;

    using pop = type_stack<Ts...>;

    template<typename U>
    using push = type_stack<U, T, Ts...>;
};

template<typename T>
using top_t = typename T::top;

template<typename T>
using pop_t = typename T::pop;

template<typename T, typename U>
using push_t = typename T::template push<U>;

}  // namespace stf::paeno::detail
