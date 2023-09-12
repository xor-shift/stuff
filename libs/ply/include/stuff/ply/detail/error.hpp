#pragma once

#include <stuff/core.hpp>

#include <limits>
#include <string>
#include <variant>

namespace stf::ply {

enum class error_class {
    internal,
    bad_magic,
    premature_eof,

    unsupported_version,
    insufficient_tokens,
    insufficient_or_excess_tokens,
    excess_tokens,
    excess_data_in_token,
    bad_data_in_token,
};

struct binary_location {
    usize m_byte_offset;
    usize m_byte_span;
};

struct ascii_location {
    usize m_row;
    usize m_col;
    usize m_col_span;
};

using error_location = std::variant<std::monostate, binary_location, ascii_location>;

struct error {
    error_class m_class;
    error_location m_location;
    std::string_view m_additional;

    static constexpr auto new_line_error(error_class error_class, usize line, std::string_view additional = ""sv) {
        return error {
            .m_class = error_class,
            .m_location = ascii_location {
              .m_row = line,
              .m_col = 0,
              .m_col_span = std::numeric_limits<usize>::max(),
            },
            .m_additional = additional,
        };
    }

    static constexpr auto new_binary_error(error_class error_class, usize byte_offset, std::string_view additional = ""sv, usize byte_span = 0) {
        return error {
            .m_class = error_class,
            .m_location = binary_location {
              .m_byte_offset = byte_offset,
              .m_byte_span = byte_span
            },
            .m_additional = additional,
        };
    }

    constexpr auto str() const -> std::string {
        return "TODO";
    }

    constexpr operator std::string() const { return str(); }
};

}  // namespace stf::ply
