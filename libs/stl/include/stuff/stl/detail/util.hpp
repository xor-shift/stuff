#pragma once

#include <stuff/bit.hpp>

namespace stf::stl::detail {

template<typename IIt, typename OIt>
constexpr auto try_read(IIt& it, std::sentinel_for<IIt> auto const& end, OIt out_beg, std::sentinel_for<OIt> auto out_end) -> bool {
    auto out_it = out_beg;
    while (it != end && out_it != out_end) {
        *out_it++ = *it++;
    }

    return out_it == out_end;
}

template<typename IIt>
constexpr auto try_read_range(IIt& it, std::sentinel_for<IIt> auto const& end_it, auto&& range) -> bool {
    using std::begin;
    using std::end;
    return try_read(it, end_it, begin(range), end(range));
}

template<typename T, typename IIt>
constexpr auto try_read(IIt& it, std::sentinel_for<IIt> auto const& end, T& out) -> bool {
    auto buf = std::array<u8, sizeof(T)>{};
    if (!try_read_range(it, end, buf)) {
        return false;
    }

    out = stf::bit::convert_endian(std::bit_cast<T>(buf), std::endian::little, std::endian::native);
    return true;
}

}
