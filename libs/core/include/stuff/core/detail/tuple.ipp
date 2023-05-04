#pragma once

namespace stf::detail {

template<typename... Ts, typename... Us> struct tuple_combiner<std::tuple<Ts...>, std::tuple<Us...>> {
    using type = std::tuple<Ts..., Us...>;
};

template<typename T> struct tuple_generator<T, 0> {
    using type = std::tuple<>;
};

template<typename T> struct tuple_generator<T, 1> {
    using type = std::tuple<T>;
};

template<typename T, size_t N> struct tuple_generator {
    using lhs_type = typename tuple_generator<T, N / 2>::type;
    using rhs_type = typename tuple_generator<T, N / 2 + N % 2>::type;

    using type = typename tuple_combiner<lhs_type, rhs_type>::type;
};

}
