#pragma once

#include <stuff/core.hpp>
#include <stuff/intro/aggregate.hpp>

#include <functional>

namespace stf::intro {

namespace detail {

template<typename Object, typename T, string_literal Name, typename Accessor>
    requires std::is_default_constructible_v<Accessor>
struct built_field {
    using object_type = Object;
    using field_type = T;
    using accessor_type = Accessor;
    inline static constexpr string_literal field_name = Name;
};

template<string_literal ToFind>
struct named_field_finder {
    template<typename Field>
    struct predicate : std::bool_constant<ToFind == Field::field_name> {};
};

template<usize I>
struct quick_getter {
    template<typename T>
    constexpr auto operator()(T&& v) -> decltype(auto) {
        return std::get<I>(tie_aggregate(std::forward<T>(v)));
    }
};

}  // namespace detail

template<typename Object, typename... Fields>
struct intro_builder {
    template<typename FieldType, string_literal FieldName, typename Accessor>
    using add_field = intro_builder<Object, Fields..., detail::built_field<Object, FieldType, FieldName, Accessor>>;

    using intro_type = Object;

    static constexpr auto size() -> usize { return sizeof...(Fields); }
    static constexpr auto size(intro_type const&) -> usize { return size(); }

    /// Returns the field information of a field and index <code>I</code>
    template<usize I>
        requires(I < size() && size() != 0)
    using nth_field = typename bunch_of_types<Fields...>::template nth_type<I>;

    /// Returns the type of the field at the index <code>I</code>
    template<usize I>
        requires(I < size() && size() != 0)
    using nth_type = typename nth_field<I>::field_type;

    /// Returns the index of a field with name <code>Name</code>. Returns size()
    /// if the field with the given name could not be found.
    template<string_literal Name>
    inline static constexpr usize named_field = (typename bunch_of_types<Fields...>::template find_type<detail::named_field_finder<Name>::template predicate>){}.value;

    /// Returns the name of the field at index <code>I</code>
    template<usize I>
        requires(I < size() && size() != 0)
    inline static constexpr string_literal nth_name = nth_field<I>::field_name;

    template<usize I, typename U>
        requires(std::is_same_v<std::remove_cvref_t<U>, Object>)
    static constexpr auto get(U&& obj) -> decltype(auto) {
        using accessor = typename nth_field<I>::accessor_type;
        return accessor{}.template operator()<U>(std::forward<U>(obj));
    }

    template<string_literal I, typename U>
        requires(std::is_same_v<std::remove_cvref_t<U>, Object>)
    static constexpr auto get(U&& obj) -> decltype(auto) {
        return get<named_field<I>, U>(std::forward<U>(obj));
    }

    template<typename Fn, typename U>
        requires(std::is_same_v<std::remove_cvref_t<U>, Object>)
    static constexpr void iterate(U&& v, Fn&& fn) {
        return tuple_iterate<intro_builder<Object, Fields...>, Fn, U, 0>(std::forward<U>(v), std::forward<Fn>(fn));
    }

    template<typename FieldType, string_literal FieldName, usize I = sizeof...(Fields)>
    using add_field_quick = add_field<FieldType, FieldName, detail::quick_getter<I>>;
};

template<typename Object>
struct intro_builder<Object> {
    template<typename FieldType, string_literal FieldName, typename Accessor>
    using add_field = intro_builder<Object, detail::built_field<Object, FieldType, FieldName, Accessor>>;

    template<typename FieldType, string_literal FieldName, usize I = 0>
    using add_field_quick = add_field<FieldType, FieldName, detail::quick_getter<I>>;

private:
    template<typename = intro_builder<Object>, usize = 0, string_literal...>
    struct quick_builder;

    template<typename Builder, usize Index, string_literal Current, string_literal... Rest>
        requires(sizeof...(Rest) != 0)
    struct quick_builder<Builder, Index, Current, Rest...> {
        using upto = typename Builder::template add_field_quick<member_type_t<Object, Index>, Current>;
        using type = typename quick_builder<upto, Index + 1, Rest...>::type;
    };

    template<typename Builder, usize Index, string_literal Current>
    struct quick_builder<Builder, Index, Current> {
        using type = typename Builder::template add_field_quick<member_type_t<Object, Index>, Current>;
    };

public:
    template<string_literal... FieldNames>
    // requires(sizeof...(FieldNames) == arity_v<Object>)
    using quick_build = typename quick_builder<intro_builder<Object>, 0, FieldNames...>::type;
};

template<typename Object, typename... Fields>
struct intro_builder<Object&, Fields...> : intro_builder<Object, Fields...> {};

template<typename Object, typename... Fields>
struct intro_builder<Object&&, Fields...> : intro_builder<Object, Fields...> {};

template<typename Object, typename... Fields>
struct intro_builder<const Object, Fields...> : intro_builder<Object, Fields...> {};

template<typename Object, typename... Fields>
struct intro_builder<volatile Object, Fields...> : intro_builder<Object, Fields...> {};

template<typename Object, typename... Fields>
struct intro_builder<const volatile Object, Fields...> : intro_builder<Object, Fields...> {};

namespace detail::agg::ct_tests {

static_assert(([] constexpr->bool {
    struct square {
        int a;
        const int b;
        int& c;
        int const& d;
        int&& e;
        int const&& f;
    };

    using intro_0 = stf::intro::intro_builder<square>::quick_build<"a", "b", "c", "d", "e", "f">;
    static_assert(stf::intro::concepts::tuple_introspector<intro_0>);
    static_assert(stf::intro::concepts::named_tuple_introspector<intro_0>);

    static_assert(std::is_same_v<intro_0::nth_type<0>, int>);
    static_assert(std::is_same_v<intro_0::nth_type<1>, const int>);
    static_assert(std::is_same_v<intro_0::nth_type<2>, int&>);
    static_assert(std::is_same_v<intro_0::nth_type<3>, int const&>);
    static_assert(std::is_same_v<intro_0::nth_type<4>, int&&>);
    static_assert(std::is_same_v<intro_0::nth_type<5>, int const&&>);

    int v = 0;
    square sq{
      .a = v,
      .b = v,
      .c = v,
      .d = v,
      .e = std::move(v),
      .f = std::move(v),
    };

    static_assert(std::is_same_v<decltype(intro_0::get<0>(sq)), int&>);
    static_assert(std::is_same_v<decltype(intro_0::get<1>(sq)), int const&>);
    static_assert(std::is_same_v<decltype(intro_0::get<2>(sq)), int&>);
    static_assert(std::is_same_v<decltype(intro_0::get<3>(sq)), int const&>);
    static_assert(std::is_same_v<decltype(intro_0::get<4>(sq)), int&&>);
    static_assert(std::is_same_v<decltype(intro_0::get<5>(sq)), int const&&>);

    // clang-format off
    static_assert(std::is_same_v<decltype(intro_0::get<0>(square{v, v, v, v, std::move(v), std::move(v)})), int&&>);
    static_assert(std::is_same_v<decltype(intro_0::get<1>(square{v, v, v, v, std::move(v), std::move(v)})), int const&&>);
    static_assert(std::is_same_v<decltype(intro_0::get<2>(square{v, v, v, v, std::move(v), std::move(v)})), int&&>);
    static_assert(std::is_same_v<decltype(intro_0::get<3>(square{v, v, v, v, std::move(v), std::move(v)})), int const&&>);
    static_assert(std::is_same_v<decltype(intro_0::get<4>(square{v, v, v, v, std::move(v), std::move(v)})), int&&>);
    static_assert(std::is_same_v<decltype(intro_0::get<5>(square{v, v, v, v, std::move(v), std::move(v)})), int const&&>);
    // clang-format on

    return true;
})());

}

}  // namespace stf::intro
