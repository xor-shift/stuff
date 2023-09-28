#pragma once

#ifdef STF_INTRO_HAS_BEEN_INCLUDED
#    error you should include introspector related headers before <stuff/intro.hpp>
#endif

#include <stuff/core.hpp>

namespace stf::intro {

template<typename T>
struct function_introspector;

template<typename Ret>
struct function_introspector<Ret()> {
    using return_type = Ret;
    static constexpr auto arg_count = 0uz;

    template<usize I>
        requires(I == 0)
    using nth_argument = void;
};

template<typename Ret, typename Arg, typename... Args>
struct function_introspector<Ret(Arg, Args...)> {
    using return_type = Ret;
    static constexpr auto arg_count = 1uz + sizeof...(Args);

    template<usize I>
    using nth_argument = typename stf::bunch_of_types<Arg, Args...>::template nth_type<I>;
};

template<typename Ret, typename... Args>
struct function_introspector<Ret (*)(Args...)> : function_introspector<Ret(Args...)> {};

namespace detail {

template<typename Class, typename Func>
struct call_operator_helper;

template<typename Class, typename Ret, typename... Args>
struct call_operator_helper<Class, Ret (*)(Args...)> : function_introspector<Ret(Args...)> {};

template<typename Class, typename Ret>
struct call_operator_helper<Class, Ret (Class::*)()> {
    using return_type = Ret;
    static constexpr auto arg_count = 0uz;

    template<usize I>
        requires(I == 0)
    using nth_argument = void;
};

template<typename Class, typename Ret, typename Arg, typename... Args>
struct call_operator_helper<Class, Ret (Class::*)(Arg, Args...)> {
    using return_type = Ret;
    static constexpr auto arg_count = sizeof...(Args);

    template<usize I>
    using nth_argument = typename stf::bunch_of_types<Arg, Args...>::template nth_type<I>;
};

template<typename Class, typename Ret, typename... Args>
struct call_operator_helper<Class, Ret (Class::*)(Args...) const> : call_operator_helper<Class, Ret (Class::*)(Args...)> {};

template<typename Class, typename Ret, typename... Args>
struct call_operator_helper<Class, Ret (Class::*)(Args...) volatile> : call_operator_helper<Class, Ret (Class::*)(Args...)> {};

template<typename Class, typename Ret, typename... Args>
struct call_operator_helper<Class, Ret (Class::*)(Args...) const volatile> : call_operator_helper<Class, Ret (Class::*)(Args...)> {};

}  // namespace detail

template<typename T>
    requires requires() { &T::operator(); }
struct function_introspector<T> : detail::call_operator_helper<T, decltype(&T::operator())> {};

}  // namespace stf::intro
