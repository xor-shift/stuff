#pragma once

namespace stf::paeno::detail {

// negate

template<>
struct negate<null> {
    using type = null;
};

template<typename Nested>
struct negate<succ<Nested>> {
    using type = pred<typename negate<Nested>::type>;
};

template<typename Nested>
struct negate<pred<Nested>> {
    using type = succ<typename negate<Nested>::type>;
};

// add

template<typename T>
struct add<T, null> : std::type_identity<T> {};

template<typename T, typename Nested>
struct add<T, succ<Nested>> : std::type_identity<succ<typename add<T, Nested>::type>> {};

template<typename T, typename Nested>
struct add<T, pred<Nested>> : std::type_identity<pred<typename add<T, Nested>::type>> {};

// from_value

template<int I>
    requires(I > 0)
struct from_value<I> : std::type_identity<succ<typename from_value<I - 1>::type>> {};

template<int I>
    requires(I < 0)
struct from_value<I> : std::type_identity<pred<typename from_value<I + 1>::type>> {};

template<>
struct from_value<0> : std::type_identity<null> {};

}
