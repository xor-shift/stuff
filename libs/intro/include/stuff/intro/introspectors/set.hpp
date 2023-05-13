#pragma once

#ifdef STF_INTRO_HAS_BEEN_INCLUDED
# error you should include introspector related headers before <stuff/intro.hpp>
#endif

#include <stuff/core.hpp>
#include <stuff/intro/concepts.hpp>

#include <set>
#include <unordered_set>

namespace stf::intro {

namespace detail {

template<typename T, typename K>
concept set_of =                                                             //
  (std::is_same_v<const typename std::remove_cvref_t<T>::key_type, const K>) &&  //
  (std::is_same_v<typename std::remove_cvref_t<T>::value_type, K>);

}

template<typename T>
struct set_introspector {
    using intro_type = std::unordered_set<T>;
    using value_type = T;

    template<detail::set_of<T> Set>
    static constexpr auto size(Set&& v) -> usize {
        return v.size();
    }

    template<detail::set_of<T> Set>
    static constexpr auto contains(Set&& v, value_type const& key) -> bool {
        return v.contains(key);
    }

    template<detail::set_of<T> Set>
    static constexpr auto begin(Set&& v) {
        return v.begin();
    }

    template<detail::set_of<T> Set>
    static constexpr auto end(Set&& v) {
        return v.end();
    }
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
