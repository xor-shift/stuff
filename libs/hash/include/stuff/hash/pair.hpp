#pragma once

#include <stuff/hash/combine.hpp>

#include <utility>

template<typename T0, typename T1>
struct std::hash<std::pair<T0, T1>> {
    constexpr auto operator()(std::pair<T0, T1> const& v) const -> usize {
        const auto lhs = std::hash<T0>{}(v.first);
        const auto rhs = std::hash<T1>{}(v.second);
        return ::stf::hash_combine(lhs, rhs);
    }
};
