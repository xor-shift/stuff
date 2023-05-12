#pragma once

#ifdef STF_INTRO_HAS_BEEN_INCLUDED
# error you should include introspector related headers before <stuff/intro.hpp>
#endif

#include <stuff/core.hpp>
#include <stuff/intro/concepts.hpp>

#include <set>
#include <unordered_set>

namespace stf::intro {

template<typename T>
struct set_introspector {
    using intro_type = std::unordered_set<T>;
    using value_type = T;
};

}

namespace std {

template<typename V, typename Hash = hash<V>, typename KeyEqual = equal_to<V>, typename Allocator = allocator<V>>
constexpr auto _stf_adl_introspector(unordered_set<V, Hash, KeyEqual, Allocator> const&) {
    return ::stf::intro::set_introspector<V>{};
}

template<typename V, typename Compare = less<V>, typename Allocator = allocator<V>>
constexpr auto _stf_adl_introspector(set<V, Compare, Allocator> const&) {
    return ::stf::intro::set_introspector<V>{};
}

}  // namespace std
