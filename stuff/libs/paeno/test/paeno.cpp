#include <stuff/paeno.hpp>

#include <gtest/gtest.h>

TEST(paeno, paeno) {
    using std::is_same_v;
    using stf::paeno::add_t;
    using stf::paeno::negate_t;
    using stf::paeno::null;
    using stf::paeno::pred;
    using stf::paeno::simplify_t;
    using stf::paeno::sub_t;
    using stf::paeno::succ;
    using stf::paeno::detail::form_stack_t;
    using stf::paeno::detail::from_stack_t;
    using stf::paeno::detail::from_value_t;
    using stf::paeno::detail::type_stack;

    static_assert(is_same_v<form_stack_t<null>, type_stack<>>);
    static_assert(is_same_v<form_stack_t<pred<succ<null>>>, type_stack<>>);
    static_assert(is_same_v<form_stack_t<succ<pred<null>>>, type_stack<>>);
    static_assert(is_same_v<form_stack_t<succ<null>>, type_stack<succ<null>>>);
    static_assert(is_same_v<
                  typename type_stack<succ<null>>::template push<succ<null>>, type_stack<succ<null>, succ<null>>>);
    static_assert(is_same_v<form_stack_t<succ<succ<null>>>, type_stack<succ<null>, succ<null>>>);

    static_assert(is_same_v<from_stack_t<type_stack<succ<null>, succ<null>>>, succ<succ<null>>>);
    static_assert(is_same_v<from_stack_t<type_stack<pred<null>, succ<null>>>, pred<succ<null>>>);
    static_assert(is_same_v<from_stack_t<type_stack<succ<null>, pred<null>>>, succ<pred<null>>>);

    static_assert(is_same_v<simplify_t<null>, null>);
    static_assert(is_same_v<simplify_t<succ<null>>, succ<null>>);
    static_assert(is_same_v<simplify_t<pred<null>>, pred<null>>);
    static_assert(is_same_v<simplify_t<succ<pred<null>>>, null>);

    static_assert(is_same_v<negate_t<null>, null>);
    static_assert(is_same_v<negate_t<succ<null>>, pred<null>>);
    static_assert(is_same_v<negate_t<succ<succ<null>>>, pred<pred<null>>>);
    static_assert(is_same_v<negate_t<pred<null>>, succ<null>>);
    static_assert(is_same_v<negate_t<pred<pred<null>>>, succ<succ<null>>>);

    static_assert(is_same_v<sub_t<null, null>, null>);
    static_assert(is_same_v<sub_t<succ<null>, null>, succ<null>>);
    static_assert(is_same_v<sub_t<null, succ<null>>, pred<null>>);
    static_assert(is_same_v<sub_t<null, pred<null>>, succ<null>>);
    static_assert(is_same_v<sub_t<null, succ<succ<null>>>, pred<pred<null>>>);
    static_assert(is_same_v<sub_t<succ<null>, succ<null>>, null>);

    using i = null;
    using j = succ<null>;

    using x = add_t<i, j>;
    using y = add_t<x, j>;

    static_assert(is_same_v<x, succ<null>>);
    static_assert(is_same_v<y, succ<succ<null>>>);

    static_assert(value_of(i{}) == 0);
    static_assert(value_of(j{}) == 1);
    static_assert(value_of(x{}) == 1);
    static_assert(value_of(y{}) == 2);

    static_assert(value_of(from_value_t<value_of(i{})>{}) == 0);
    static_assert(value_of(from_value_t<value_of(j{})>{}) == 1);
    static_assert(value_of(from_value_t<value_of(x{})>{}) == 1);
    static_assert(value_of(from_value_t<value_of(y{})>{}) == 2);
}
