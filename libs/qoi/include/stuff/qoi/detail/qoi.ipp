#pragma once

#include <stuff/bit.hpp>

namespace stf::qoi {

constexpr void decode_from_memory(detail::raw_header raw_header, std::span<const u8> encoded, std::span<color> out) {
    std::array<color, 64> hash_table{};
    std::fill(hash_table.begin(), hash_table.end(), color{0, 0, 0, 0});
    color last_color{0, 0, 0, 255};

    for (;;) {
        // switch (determine_op())
        break;
    }
}

template<typename Allocator>
constexpr auto image<Allocator>::from_memory(std::span<const u8> data) -> std::expected<void, std::string_view> {
    if (data.size() < 14)
        return std::unexpected{"the data is too small to contain a header"};

    std::array<char, sizeof(detail::raw_header)> header_data;
    std::copy_n(data.begin(), 14, header_data.begin());
    detail::raw_header raw_header = std::bit_cast<detail::raw_header>(header_data);
    raw_header.m_width = stf::bit::convert_endian(raw_header.m_width, std::endian::big, std::endian::native);
    raw_header.m_height = stf::bit::convert_endian(raw_header.m_height, std::endian::big, std::endian::native);

    channels channels;
    color_space color_space;

    switch (raw_header.m_channels) {
        case static_cast<u8>(channels::rgb): channels = channels::rgb; break;
        case static_cast<u8>(channels::rgba): channels = channels::rgba; break;
        default: return std::unexpected{"invalid channel specifier"};
    }

    switch (raw_header.m_colorspace) {
        case static_cast<u8>(color_space::srgb_linear_alpha): color_space = color_space::srgb_linear_alpha; break;
        case static_cast<u8>(color_space::all_linear): color_space = color_space::all_linear; break;
        default: return std::unexpected{"invalid colorspace specifier"};
    }

    create(raw_header.m_width, raw_header.m_height, color_space);

    TRYX(detail::decode({m_data, m_width * m_height}, data.subspan(14)));

    return {};
}

template<typename Allocator>
template<typename It>
constexpr auto image<Allocator>::to_memory(It out, double loss_tolerance) const -> std::expected<It, std::string_view> {
    detail::raw_header raw_header{
      .m_magic = {'q', 'o', 'i', 'f'},
      .m_width = m_width,
      .m_height = m_height,
      .m_channels = static_cast<u8>(channels::rgba),
      .m_colorspace = static_cast<u8>(m_color_space),
    };
    raw_header.m_width = stf::bit::convert_endian(raw_header.m_width, std::endian::native, std::endian::big);
    raw_header.m_height = stf::bit::convert_endian(raw_header.m_height, std::endian::native, std::endian::big);

    auto raw_header_data = std::bit_cast<std::array<u8, 16>>(raw_header);

    out = std::copy_n(raw_header_data.data(), 14, out);

    if (loss_tolerance == 0)
        out = TRYX(detail::encode_lossless(out, pixels()));
    else
        out = TRYX(detail::encode_lossy(out, pixels(), loss_tolerance));

    out = std::fill_n(out, 7, u8(0));
    *out++ = u8(1);

    return out;
}

}  // namespace stf::qoi
