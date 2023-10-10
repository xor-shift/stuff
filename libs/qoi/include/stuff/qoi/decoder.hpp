#pragma once

#include <stuff/qoi/color.hpp>
#include <stuff/qoi/detail/common.hpp>

#include <expected>
#include <span>

namespace stf::qoi::detail {

constexpr auto decode(std::span<color> out_pixels, std::span<const u8> in_data) -> std::expected<void, std::string_view>;

}  // namespace stf::qoi::detail

#include <stuff/qoi/detail/decoder.ipp>
