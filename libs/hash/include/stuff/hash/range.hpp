#pragma once

#include <stuff/hash/combine.hpp>

namespace stf {

template<std::ranges::range Range>
    requires requires(std::ranges::range_value_t<Range> const& value) {
        { std::hash<std::ranges::range_value_t<Range>>{}(value) } -> std::convertible_to<usize>;
    }
constexpr auto range_hash(Range range) {
    return std::ranges::fold_left(range, 0uz, [](auto lhs, auto const& rhs) {
        return hash_combine(lhs, std::hash<std::remove_cvref_t<decltype(rhs)>>{}(rhs));
    });
}

template<typename T>
struct hash;

}  // namespace stf
