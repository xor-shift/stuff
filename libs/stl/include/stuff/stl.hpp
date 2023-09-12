#pragma once

#include <stuff/stl/detail/ascii.hpp>
#include <stuff/stl/detail/binary.hpp>

namespace stf::stl {

template<typename IIt>
constexpr auto read_header(IIt beg, std::sentinel_for<IIt> auto end) -> stf::expected<std::pair<header, IIt>, std::string_view> {
    auto it = beg;

    auto leading_bytes = std::array<u8, 5>{};
    constexpr auto ascii_leading_bytes = std::array<u8, 5>{'s', 'o', 'l', 'i', 'd'};

    if (!detail::try_read_range(it, end, leading_bytes)) {
        return stf::unexpected{"unexpected EOF while trying to determine file type"};
    }

    if (leading_bytes == ascii_leading_bytes) {
        auto [res_header, res_it] = TRYX(detail::read_header_ascii(it, end));
        return std::pair{res_header, res_it};
    }

    auto [res_header, res_it] = TRYX(detail::read_header_binary(it, end, leading_bytes));
    return std::pair{res_header, res_it};
}

template<typename IIt, typename Fn>
constexpr auto read_triangles(header const& header, IIt it, std::sentinel_for<IIt> auto end, Fn&& callback) -> stf::expected<IIt, std::string_view> {
    if (std::holds_alternative<header_binary>(header)) {
        return detail::read_triangles_binary(std::get<header_binary>(header), it, end, std::forward<Fn>(callback));
    }

    if (std::holds_alternative<header_ascii>(header)) {
        return detail::read_triangles_ascii(std::get<header_ascii>(header), it, end, std::forward<Fn>(callback));
    }

    std::unreachable();
}

}  // namespace stf::stl
