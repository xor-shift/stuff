#pragma once

#include <stuff/ply/detail/token.hpp>

#include <charconv>
#include <expected>

namespace stf::ply {

enum class data_format {
    ascii,
    binary_le,
    binary_be,
};

struct file_format {
    data_format m_data_format;
    usize m_version_major;
    usize m_version_minor;

    static constexpr auto from_tokens(std::span<const detail::token> tokens, usize line) -> std::expected<file_format, error> {
        if (tokens.size() != 3) {
            return std::unexpected{error::new_line_error(error_class::insufficient_tokens, line)};
        }

        if (tokens[0].m_content != "format") {
            return tokens[0].generate_error(error_class::internal);
        }

        auto data_fmt = data_format{};

        if (tokens[1].m_content == "ascii") {
            data_fmt = data_format::ascii;
        } else if (tokens[1].m_content == "binary_little_endian") {
            data_fmt = data_format::binary_le;
        } else if (tokens[1].m_content == "binary_big_endian") {
            data_fmt = data_format::binary_be;
        } else {
            return tokens[1].generate_error(error_class::bad_data_in_token, "expected one of: \"ascii\", \"binary_little_endian\", \"binary_big_endian\"");
        }

        const auto decimal_pos = tokens[2].m_content.find('.');
        if (decimal_pos == std::string_view::npos) {
            return tokens[2].generate_error(error_class::bad_data_in_token, "expected a decimal separator for the version string");
        }

        auto&& major_token = tokens[2].substring(0, decimal_pos);
        auto&& minor_token = tokens[2].substring(decimal_pos + 1);

        return file_format{
          .m_data_format = data_fmt,
          .m_version_major = TRYX(major_token.as_scalar<usize>()),
          .m_version_minor = TRYX(minor_token.as_scalar<usize>()),
        };
    }
};

}  // namespace stf::ply
