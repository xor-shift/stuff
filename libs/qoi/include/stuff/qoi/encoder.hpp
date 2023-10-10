#pragma once

#include <stuff/qoi/color.hpp>
#include <stuff/qoi/detail/common.hpp>
#include <stuff/scope.hpp>

#include <cmath>
#include <expected>
#include <span>

namespace stf::qoi::detail {

template<typename It>
constexpr auto encode_lossless(It out_data, std::span<const color> in_pixels) -> std::expected<It, std::string_view>;

template<typename It>
constexpr auto encode_lossy(It out_data, std::span<const color> in_pixels, double tolerance = 1)
  -> std::expected<It, std::string_view>;

}  // namespace stf::qoi::detail

#include <stuff/qoi/detail/encoder.ipp>
