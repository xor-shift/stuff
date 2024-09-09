#pragma once

#include <stuff/intro/detail/aggregate/arity.hpp>
#include <stuff/intro/detail/aggregate/member_types.hpp>
#include <stuff/intro/detail/aggregate/tie.hpp>

#include <stuff/intro/introspectors/tuple.hpp>

namespace stf::intro::detail {

template<typename Intro, usize I>
struct aggregate_member_offset_helper {
    // we don't really have access to the member pointers, sad!
    // also: i VIL use template meta-functions, i VIL be happy, and there's nothing you can do.
    // <sub>(they should be faster than constexpr/eval functions anyway)</sub>
    //
    // what do you mean HTML tags don't work in comments

    static constexpr auto offset_upto_here = aggregate_member_offset_helper<Intro, I - 1>::value;
    static constexpr auto next_available_char = offset_upto_here + sizeof(typename Intro::template nth_type<I - 1>);
    static constexpr auto off_by = next_available_char % alignof(typename Intro::template nth_type<I>);
    static constexpr auto value = next_available_char + off_by;
};

template<typename Intro, usize I>
    requires (I == Intro::size())
struct aggregate_member_offset_helper<Intro, I> {
    static constexpr auto value = sizeof(typename Intro::intro_type);
};

template<typename Intro>
struct aggregate_member_offset_helper<Intro, 0> : std::integral_constant<usize, 0> {};

template<typename T>
struct aggregate_introspector {
    using intro_type = std::remove_cvref_t<T>;

    static constexpr auto size() -> usize { return arity<T>; }
    static constexpr auto size([[maybe_unused]] intro_type const& v) -> usize { return size(); }

    template<usize I>
        requires(I < size())
    using nth_type = member_type_t<T, I>;

    template<usize I>
        requires(I <= size())
    static constexpr auto nth_offset = aggregate_member_offset_helper<aggregate_introspector<T>, I>::value;

    template<usize I, typename Aggregate>
        requires(std::is_same_v<std::remove_cvref_t<Aggregate>, intro_type>)
    static constexpr auto get(Aggregate&& v) -> decltype(auto) {
        return std::get<I>(tie_aggregate(std::forward<Aggregate>(v)));
    }

    template<typename Fn, typename Aggregate>
        requires(std::is_same_v<std::remove_cvref_t<Aggregate>, intro_type>)
    static constexpr void iterate(Aggregate&& v, Fn&& fn) {
        return tuple_iterate<aggregate_introspector<T>, Fn, Aggregate, 0>(std::forward<Aggregate>(v), std::forward<Fn>(fn));
    }

    template<typename Fn>
    static constexpr void iterate_types(Fn&& fn) {
        return iterate_types_impl<Fn, 0>(std::forward<Fn>(fn));
    }

private:
    template<typename Fn, usize I>
    static constexpr void iterate_types_impl(Fn&& fn) {
        std::invoke(std::forward<Fn>(fn), std::integral_constant<usize, I>{}, std::type_identity<nth_type<I>>{});

        if constexpr (I + 1 < size()) {
            return iterate_types_impl<Fn, I + 1>(std::forward<Fn>(fn));
        }
    }
};

namespace ce_tests {

static_assert(([] constexpr -> bool {
    struct test_struct {
        int a;
        float b;
        double c;
    };

    static_assert(concepts::tuple_introspector<aggregate_introspector<test_struct>>);

    return true;
})());

}

}  // namespace stf::intro::detail
