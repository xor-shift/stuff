#pragma once

#include <stuff/medium_int/medium_int.hpp>

#include <stuff/hash.hpp>

template<usize Bits, typename Repr>
struct std::hash<stf::medium_int<Bits, Repr>> {
    constexpr auto operator()(stf::medium_int<Bits, Repr> const& v) const -> bool {
        return stf::range_hash(v.get_repr());
    }
};
