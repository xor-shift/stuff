#pragma once

#include <stuff/core.hpp>

#include <map>
#include <unordered_map>

namespace stf::intro::detail {

template<typename K, typename V>
struct map_introspector {
    using key_type = std::string_view;
    using mapped_type = V;

    using intro_type = std::unordered_map<key_type, mapped_type>;  // remember that this type is just a hint
                                                                  // other parameters are unnecessary

    static constexpr auto size(intro_type const& v) -> usize { return v.size(); }

    template<typename Map>
        requires(std::is_same_v<typename std::decay_t<Map>::key_type, key_type> &&
                 std::is_same_v<typename std::decay_t<Map>::mapped_type, mapped_type>)
    static constexpr auto index(Map&& v, std::string_view i) -> decltype(auto) {
        if constexpr (std::is_lvalue_reference_v<Map>) {
            return v.at(i);
        } else {
            return std::move(v.at(i));
        }
    }

    template<typename Map>
        requires(std::is_same_v<typename std::decay_t<Map>::key_type, key_type> &&
                 std::is_same_v<typename std::decay_t<Map>::mapped_type, mapped_type>)
    static constexpr auto begin(Map&& v) -> decltype(auto) {
        return v.begin();
    }

    template<typename Map>
        requires(std::is_same_v<typename std::decay_t<Map>::key_type, key_type> &&
                 std::is_same_v<typename std::decay_t<Map>::mapped_type, mapped_type>)
    static constexpr auto end(Map&& v) -> decltype(auto) {
        return v.end();
    }
};

// static_assert(concepts::tuple_introspector<tuple_introspector<int, float, double>>);

}  // namespace stf::intro::detail

namespace std {

template<
  typename K,
  typename V,
  typename Hash = hash<K>,
  typename KeyEqual = equal_to<K>,
  typename Allocator = allocator<pair<const K, V>>>
constexpr auto _stf_adl_introspector(unordered_map<K, V, Hash, KeyEqual, Allocator> const&) {
    return ::stf::intro::detail::map_introspector<K, V>{};
}

template<typename K, typename V, typename Compare = less<K>, typename Allocator = allocator<pair<const K, V>>>
constexpr auto _stf_adl_introspector(map<K, V, Compare, Allocator> const&) {
    return ::stf::intro::detail::map_introspector<K, V>{};
}

}  // namespace std
