#pragma once

#ifdef STF_INTRO_HAS_BEEN_INCLUDED
#    error you should include introspector related headers before <stuff/intro.hpp>
#endif

#include <stuff/core.hpp>

namespace stf::intro {

template<typename T>
struct function_introspector;

template<typename T>
struct function_introspector<T const&> : function_introspector<T> {};

template<typename T>
struct function_introspector<T&> : function_introspector<T> {};

template<typename T>
struct function_introspector<T const&&> : function_introspector<T> {};

template<typename T>
struct function_introspector<T&&> : function_introspector<T> {};

template<typename Ret>
struct function_introspector<Ret()> {
    using return_type = Ret;
    static constexpr auto arg_count = 0uz;

    template<usize I>
        requires(I == 0)
    using nth_argument = void;
};

template<typename Ret>
struct function_introspector<Ret() noexcept> : function_introspector<Ret()> {};

template<typename Ret, typename Arg, typename... Args>
struct function_introspector<Ret(Arg, Args...)> {
    using return_type = Ret;
    static constexpr auto arg_count = 1uz + sizeof...(Args);

    template<usize I>
    using nth_argument = typename stf::bunch_of_types<Arg, Args...>::template nth_type<I>;
};

template<typename Ret, typename Arg, typename... Args>
struct function_introspector<Ret(Arg, Args...) noexcept> : function_introspector<Ret(Arg, Args...)> {};

template<typename Ret, typename... Args>
struct function_introspector<Ret (*)(Args...)> : function_introspector<Ret(Args...)> {};

namespace detail {

template<typename Class, typename Func>
struct call_operator_helper;

template<typename Class, typename Func>
struct call_operator_helper<Class const&, Func> : call_operator_helper<Class, Func> {};

template<typename Class, typename Func>
struct call_operator_helper<Class&, Func> : call_operator_helper<Class, Func> {};

template<typename Class, typename Func>
struct call_operator_helper<Class const&&, Func> : call_operator_helper<Class, Func> {};

template<typename Class, typename Func>
struct call_operator_helper<Class&&, Func> : call_operator_helper<Class, Func> {};

template<typename Class, typename Ret, typename... Args>
struct call_operator_helper<Class, Ret (*)(Args...)> : function_introspector<Ret(Args...)> {};

///

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

template<typename Class, typename Ret, typename... Args>
struct call_operator_helper<Class, Ret (Class::*)(Args...) noexcept> : call_operator_helper<Class, Ret (Class::*)(Args...)> {};

template<typename Class, typename Ret, typename... Args>
struct call_operator_helper<Class, Ret (Class::*)(Args...) const noexcept> : call_operator_helper<Class, Ret (Class::*)(Args...)> {};

template<typename Class, typename Ret, typename... Args>
struct call_operator_helper<Class, Ret (Class::*)(Args...) volatile noexcept> : call_operator_helper<Class, Ret (Class::*)(Args...)> {};

template<typename Class, typename Ret, typename... Args>
struct call_operator_helper<Class, Ret (Class::*)(Args...) const volatile noexcept> : call_operator_helper<Class, Ret (Class::*)(Args...)> {};

struct call_operator_checker_helper {
    constexpr void operator()() const noexcept {}
};

template<typename T>
concept unambigously_has_call_operator = requires { &T::operator(); };

template<typename T>
    requires std::is_class_v<std::remove_cvref_t<T>>
struct call_operator_checker : call_operator_checker_helper, std::remove_cvref_t<T> {};

template<typename T>
concept has_call_operator = !unambigously_has_call_operator<call_operator_checker<T>>;

}  // namespace detail

template<detail::has_call_operator T>
    requires detail::unambigously_has_call_operator<T>
struct function_introspector<T> : detail::call_operator_helper<T, decltype(&T::operator())> {};

}  // namespace stf::intro
