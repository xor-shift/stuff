#pragma once

#include <stuff/core.hpp>

#include <map>
#include <unordered_map>

namespace stf::intro {

template<typename K, typename V>
struct map_introspector {
    using key_type = std::string_view;
    using mapped_type = V;

    using intro_type = std::unordered_map<key_type, mapped_type>;  // remember that this type is just a hint
                                                                   // other parameters are unnecessary

    template<typename T>
    inline static constexpr bool is_related_type =
      std::is_same_v<typename std::remove_cvref_t<T>::key_type, key_type> &&
      std::is_same_v<typename std::remove_cvref_t<T>::mapped_type, mapped_type>;

    static constexpr auto size(intro_type const& v) -> usize { return v.size(); }

    template<typename Map>
        requires(is_related_type<Map>)
    static constexpr auto index(Map&& v, std::string_view i) -> decltype(auto) {
        if constexpr (std::is_lvalue_reference_v<Map>) {
            return v.at(i);
        } else {
            return std::move(v.at(i));
        }
    }

    template<typename Map>
        requires(is_related_type<Map>)
    static constexpr auto begin(Map&& v) -> decltype(auto) {
        return v.begin();
    }

    template<typename Map>
        requires(is_related_type<Map>)
    static constexpr auto end(Map&& v) -> decltype(auto) {
        return v.end();
    }

    template<typename Map, typename Fn>
    static constexpr void iterate(Map&& map, Fn&& fn) {
        using arg_type_c = std::conditional_t<std::is_const_v<Map>, const mapped_type, mapped_type>;
        using arg_type_cv = std::conditional_t<std::is_volatile_v<Map>, volatile arg_type_c, arg_type_c>;
        using arg_type = std::conditional_t<std::is_lvalue_reference_v<Map>, arg_type_cv&, arg_type_cv&&>;

        for (auto it = begin(map); it != end(map); it++) {
            std::invoke<Fn, key_type const&, arg_type>(
              std::forward<Fn>(fn), it->first, static_cast<arg_type>(it->second)
            );
        }
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
    return ::stf::intro::map_introspector<K, V>{};
}

template<typename K, typename V, typename Compare = less<K>, typename Allocator = allocator<pair<const K, V>>>
constexpr auto _stf_adl_introspector(map<K, V, Compare, Allocator> const&) {
    return ::stf::intro::map_introspector<K, V>{};
}

}  // namespace std
