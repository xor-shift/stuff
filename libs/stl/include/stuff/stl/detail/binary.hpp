#pragma once

#include <stuff/stl/detail/header.hpp>
#include <stuff/stl/detail/triangle.hpp>
#include <stuff/stl/detail/util.hpp>

#include <stuff/expected.hpp>

namespace stf::stl::detail {

template<typename IIt>
constexpr auto read_header_binary(IIt it, std::sentinel_for<IIt> auto end, std::array<u8, 5> const& leading_bytes) -> stf::expected<std::pair<header, IIt>, std::string_view> {
    auto header_bytes = std::array<u8, 80>{};
    if (!try_read(it, end, std::copy(leading_bytes.begin(), leading_bytes.end(), header_bytes.begin()), header_bytes.end())) {
        return stf::unexpected{"unexpected EOF while reading binary header"};
    }

    auto num_triangles_bytes = std::array<u8, 4>{};
    if (!try_read_range(it, end, num_triangles_bytes)) {
        return stf::unexpected{"unexpected EOF while reading the number of triangles"};
    }

    const auto num_triangles = stf::bit::convert_endian(std::bit_cast<u32>(num_triangles_bytes), std::endian::little, std::endian::native);

    return std::pair{header_binary{header_bytes, num_triangles}, it};
}

template<typename IIt, std::invocable<triangle<float>&&> Fn>
constexpr auto read_triangles_binary(header_binary const& header, IIt it, std::sentinel_for<IIt> auto end, Fn&& callback) -> stf::expected<IIt, std::string_view> {
    auto n = 0uz;
    for (; it != end; n++) {
        if (n >= header.m_num_triangles) {
            return stf::unexpected{"excess data"};
        }

        auto ret = triangle<float>{};
        auto ok = detail::try_read(it, end, ret.m_normal[0]) &&       //
                  detail::try_read(it, end, ret.m_normal[1]) &&       //
                  detail::try_read(it, end, ret.m_normal[2]) &&       //
                  detail::try_read(it, end, ret.m_vertices[0][0]) &&  //
                  detail::try_read(it, end, ret.m_vertices[0][1]) &&  //
                  detail::try_read(it, end, ret.m_vertices[0][2]) &&  //
                  detail::try_read(it, end, ret.m_vertices[1][0]) &&  //
                  detail::try_read(it, end, ret.m_vertices[1][1]) &&  //
                  detail::try_read(it, end, ret.m_vertices[1][2]) &&  //
                  detail::try_read(it, end, ret.m_vertices[2][0]) &&  //
                  detail::try_read(it, end, ret.m_vertices[2][1]) &&  //
                  detail::try_read(it, end, ret.m_vertices[2][2]) &&  //
                  detail::try_read(it, end, ret.m_attributes);

        if (!ok) {
            return stf::unexpected{"eof"};
        }

        std::invoke(std::forward<Fn>(callback), std::move(ret));
    }

    if (n != header.m_num_triangles) {
        return stf::unexpected{"the file contains less triangles than the header would suggest"};
    }

    return it;
}

}
