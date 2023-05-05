#pragma once

#include <stuff/core.hpp>

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
    struct predicate : std::bool_constant<true> {};
};

}  // namespace detail

template<typename Object, typename... Fields>
struct intro_builder {
    template<typename FieldType, string_literal FieldName, typename Accessor>
    using add_field = intro_builder<Object, Fields..., detail::built_field<Object, FieldType, FieldName, Accessor>>;

    using intro_type = Object;

    static constexpr auto size() -> usize { return sizeof...(Fields); }
    static constexpr auto size(intro_type const&) -> usize { return size(); }

    template<usize I>
        requires(I < size() && size() != 0)
    using nth_field = typename bunch_of_types<Fields...>::template nth_type<I>;

    template<string_literal Name>
    using named_field = typename  //
      bunch_of_types<Fields...>::template find_type<detail::named_field_finder<Name>::template predicate>;

    template<usize I>
        requires(I < size() && size() != 0)
    using nth_type = typename nth_field<I>::field_type;

    template<usize I, typename U>
        requires(std::is_same_v<std::remove_cvref_t<U>, Object>)
    static constexpr auto get(U&& obj) -> decltype(auto) {
        using accessor = typename nth_field<I>::accessor_type;
        return std::invoke(accessor{}, std::forward<U>(obj));
    }

    template<string_literal I, typename U>
        requires(std::is_same_v<std::remove_cvref_t<U>, Object>)
    static constexpr auto get(U&& obj) -> decltype(auto) {
        return get<named_field<I>::value, U>(std::forward<U>(obj));
    }

    template<typename Fn, typename U>
        requires(std::is_same_v<std::remove_cvref_t<U>, Object>)
    static constexpr void iterate(U&& v, Fn&& fn) {
        return tuple_iterate<intro_builder<Object, Fields...>, Fn, U, 0>(std::forward<U>(v), std::forward<Fn>(fn));
    }
};

template<typename Object>
struct intro_builder<Object> {
    template<typename FieldType, string_literal FieldName, typename Accessor>
    using add_field = intro_builder<Object, detail::built_field<Object, FieldType, FieldName, Accessor>>;
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

}  // namespace stf::intro
