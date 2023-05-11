#pragma once

#include <stuff/intro/detail/aggregate/arity.hpp>
#include <stuff/intro/detail/aggregate/member_types.hpp>
#include <stuff/intro/detail/aggregate/tie.hpp>

#include <stuff/intro/introspectors/tuple.hpp>

namespace stf::intro::detail {

template<typename T>
struct aggregate_introspector {
    using intro_type = std::remove_cvref_t<T>;

    static constexpr auto size() -> usize { return arity<T>; }
    static constexpr auto size(intro_type const& v) -> usize { return size(); }

    template<usize I>
        requires(I < size())
    using nth_type = member_type_t<T, I>;

    template<usize I, typename Aggregate>
        requires(std::is_same_v<std::remove_cvref_t<Aggregate>, intro_type>)
    static constexpr auto get(Aggregate&& v) -> decltype(auto) {
        return std::get<I>(tie_aggregate(std::forward<Aggregate>(v)));
    }

    template<typename Fn, typename Aggregate>
        requires(std::is_same_v<std::remove_cvref_t<Aggregate>, intro_type>)
    static constexpr void iterate(Aggregate&& v, Fn&& fn) {
        return tuple_iterate<aggregate_introspector<T>, Fn, Aggregate, 0>(
          std::forward<Aggregate>(v), std::forward<Fn>(fn)
        );
    }
};

namespace agg::ce_tests {

static_assert(([] constexpr->bool {
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
