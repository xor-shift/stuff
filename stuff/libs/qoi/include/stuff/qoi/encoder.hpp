#pragma once

#include <stuff/expected.hpp>
#include <stuff/qoi/color.hpp>
#include <stuff/qoi/detail/common.hpp>

#include <span>

namespace stf::qoi::detail {

template<typename It>
constexpr auto encode_lossless(It out_data, std::span<const color> in_pixels)
  -> expected::expected<It, std::string_view>;

template<typename It>
constexpr auto encode_lossy(It out_data, std::span<const color> in_pixels, i8 tolerance = 1)
  -> expected::expected<It, std::string_view>;

}  // namespace stf::qoi::detail

#include <stuff/qoi/detail/encoder.ipp>
