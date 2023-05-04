#pragma once

#include <stuff/intro/detail/aggregate/bsearch.hpp>
#include <stuff/intro/detail/aggregate/initializable_n.hpp>

namespace stf::intro::detail::agg {

namespace detail {

template<typename T, usize LowerBound = 0>
constexpr auto faux_arity_lower_bound() -> usize {
    if constexpr (initializable_n<T, LowerBound>) {
        return LowerBound;
    } else {
        return faux_arity_lower_bound<T, LowerBound + 1>();
    }
}

template<typename T, usize LowerBound = faux_arity_lower_bound<T>()>
constexpr auto faux_arity_upper_bound() -> usize {
    constexpr size_t next_index = LowerBound == 0 ? 1 : LowerBound * 2;
    if constexpr (!initializable_n<T, next_index>) {
        return next_index;
    } else {
        return faux_arity_upper_bound<T, next_index>();
    }
}

}

namespace ct_tests {

static_assert(([] constexpr->bool {
    struct Anon {
        int& a;
        int b[2];
        int c;
        int d;
    };

    static_assert(detail::faux_arity_upper_bound<Anon>() == 8);
    return true;
})());

}

namespace detail {

template<typename T>
struct faux_arity_predicate {
    template<usize N>
    struct predicate : std::bool_constant<initializable_n<T, N>> {};
};


}

// clang-format off
/// Returns the number of elements a <code>T</code> can be initialised with after brace-elision.\n
/// For example, a type of <code>struct { int& a; int b; int c[3]; }</code> would result in a value of 5.\n
template<typename T>
inline constexpr usize faux_arity_v = binary_search<detail::faux_arity_predicate<T>::template predicate, detail::faux_arity_lower_bound<T>(), detail::faux_arity_upper_bound<T>()>::type::value;
// clang-format on

namespace ct_tests {

static_assert(([] constexpr->bool {
    struct Anon {
        int& a;
        int b[2];
        int c;
        int d;
    };

    static_assert(faux_arity_v<Anon> == 5);

    return true;
})());

}

}
