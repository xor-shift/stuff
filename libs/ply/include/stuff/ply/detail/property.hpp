#pragma once

#include <stuff/ply/detail/token.hpp>

#include <stuff/bit.hpp>
#include <stuff/core.hpp>

#include <charconv>
#include <expected>
#include <span>

namespace stf::ply {

enum class basic_field_type {
    u8,
    u16,
    u32,
    u64,
    i8,
    i16,
    i32,
    i64,
    f32,
    f64,
};

constexpr auto enum_name(basic_field_type v) -> std::string_view {
    using namespace std::string_view_literals;

    switch (v) {
        using enum basic_field_type;

        case u8: return "uint8"sv;
        case u16: return "uint16"sv;
        case u32: return "uint32"sv;
        case u64: return "uint64"sv;
        case i8: return "int8"sv;
        case i16: return "int16"sv;
        case i32: return "int32"sv;
        case i64: return "int64"sv;
        case f32: return "float32"sv;
        case f64: return "float64"sv;
    }

    std::unreachable();
}

namespace detail {

constexpr auto field_byte_width(basic_field_type type) -> usize {
    switch (type) {
        using enum basic_field_type;

        case u8: return sizeof(::u8);
        case u16: return sizeof(::u16);
        case u32: return sizeof(::u32);
        case u64: return sizeof(::u64);
        case i8: return sizeof(::i8);
        case i16: return sizeof(::i16);
        case i32: return sizeof(::i32);
        case i64: return sizeof(::i64);
        case f32: return sizeof(float);
        case f64: return sizeof(double);
    }

    std::unreachable();
}

constexpr auto field_type_sign(basic_field_type type) -> bool {
    switch (type) {
        using enum basic_field_type;

        case u8: return false;
        case u16: return false;
        case u32: return false;
        case u64: return false;
        case i8: return true;
        case i16: return true;
        case i32: return true;
        case i64: return true;
        case f32: return false;
        case f64: return false;
    }

    std::unreachable();
}

constexpr auto parse_basic_field_type(token const& token) -> std::expected<basic_field_type, error> {
    using namespace std::string_view_literals;

    if (token.m_content.starts_with("uint"sv)) {
        if (token.m_content.size() == 4) {
            return basic_field_type::u32;
        }

        const auto width = TRYX(token.substring(4).as_scalar<usize>());

        switch (width) {
            using enum basic_field_type;
            case 8: return u8;
            case 16: return u16;
            case 32: return u32;
            case 64: return u64;
            default: return token.generate_error(error_class::bad_data_in_token, "unsupported property width for uint");
        }
    }

    if (token.m_content.starts_with("int"sv)) {
        if (token.m_content.size() == 3) {
            return basic_field_type::i32;
        }

        const auto width = TRYX(token.substring(3).as_scalar<usize>());

        switch (width) {
            using enum basic_field_type;
            case 8: return i8;
            case 16: return i16;
            case 32: return i32;
            case 64: return i64;
            default: return token.generate_error(error_class::bad_data_in_token, "unsupported property width for int");
        }
    }

    if (token.m_content.starts_with("float"sv)) {
        if (token.m_content.size() == 5) {
            return basic_field_type::f32;
        }

        const auto width = TRYX(token.substring(5).as_scalar<usize>());

        switch (width) {
            using enum basic_field_type;
            case 32: return f32;
            case 64: return f64;
            default: return token.generate_error(error_class::bad_data_in_token, "unsupported property width for float");
        }
    }

    if (token.m_content == "char") {
        return basic_field_type::i8;
    }

    if (token.m_content == "uchar") {
        return basic_field_type::u8;
    }

    if (token.m_content == "short") {
        return basic_field_type::i16;
    }

    if (token.m_content == "ushort") {
        return basic_field_type::u16;
    }

    if (token.m_content == "long") {
        return basic_field_type::i64;
    }

    if (token.m_content == "ulong") {
        return basic_field_type::u64;
    }

    if (token.m_content == "double") {
        return basic_field_type::f64;
    }

        return token.generate_error(error_class::bad_data_in_token, "invalid basic property type (doesn't start with one of: \"uint\", \"int\", \"float\", \"double\" or is not a known type name)");
}

}  // namespace detail

using list_field_type = std::pair<basic_field_type, basic_field_type>;

using field_type = std::variant<basic_field_type, list_field_type>;

struct property {
    field_type m_field;
    std::string m_name;

    static constexpr auto from_tokens(std::span<const detail::token> tokens, usize line) -> std::expected<property, error> {
        if (tokens.size() != 3 && tokens.size() != 5) {
            return std::unexpected{error::new_line_error(error_class::insufficient_tokens, line)};
        }

        if (tokens[0].m_content != "property") {
            return tokens[0].generate_error(error_class::internal, "expected \"property\"");
        }

        const auto is_list = tokens.size() == 5;

        if (is_list && tokens[1].m_content != "list") {
            return tokens[1].generate_error(error_class::bad_data_in_token, "expected \"list\"");
        }

        auto ret = property{
          .m_name = std::string(tokens.back().m_content),
        };

        if (is_list) {
            const auto index_type = TRYX(detail::parse_basic_field_type(tokens[2]));
            const auto value_type = TRYX(detail::parse_basic_field_type(tokens[3]));

            ret.m_field = list_field_type{index_type, value_type};
        } else {
            const auto type = TRYX(detail::parse_basic_field_type(tokens[1]));

            ret.m_field = type;
        }

        return ret;
    }
};

namespace detail {

template<typename T, typename It, std::sentinel_for<It> Sentinel>
    requires std::integral<T>
static constexpr auto read_scalar(It& it, Sentinel sentinel, std::endian endian, usize file_offset, basic_field_type field_type) -> std::expected<T, error> {
    auto error_if_eof = [&it, sentinel, file_offset] -> std::expected<void, error> {
        if (it == sentinel) {
            return std::unexpected{error::new_binary_error(error_class::premature_eof, file_offset, "not enough bytes for the declared type")};
        }

        return {};
    };

    const auto file_width = field_byte_width(field_type);

    auto buf = std::array<u8, sizeof(u64)>{};

    if (file_width > buf.size()) {
        return std::unexpected{error::new_binary_error(error_class::internal, file_offset, "file size is larger than the largest expected size (which is 8)")};
    }

    for (usize i = 0; i < file_width; i++) {
        TRYX(error_if_eof());
        auto v = *it++;
        buf[i] = static_cast<u8>(v);
    }

    const auto excess_bytes = buf.size() - file_width;

    if (endian == std::endian::big) {
        std::shift_right(buf.begin(), buf.end(), excess_bytes);
    }

    if (endian == std::endian::big) {
        const auto sign = (buf[excess_bytes - 1] & 0x80) != 0;
        if (field_type_sign(field_type) && sign) {
            std::fill(buf.begin(), buf.begin() + excess_bytes, 0xff);
        } else {
            std::fill(buf.begin(), buf.begin() + excess_bytes, 0x00);
        }
    } else {
        const auto sign = (buf[file_width - 1] & 0x80) != 0;
        if (field_type_sign(field_type) && sign) {
            std::fill(buf.begin() + file_width, buf.end(), 0xff);
        } else {
            std::fill(buf.begin() + file_width, buf.end(), 0x00);
        }
    }

    const auto temp_res = std::bit_cast<u64>(buf);
    return static_cast<T>(stf::bit::convert_endian(temp_res, endian, std::endian::native));
}

template<typename T, typename It, std::sentinel_for<It> Sentinel>
    requires std::floating_point<T>
static constexpr auto read_scalar(It& it, Sentinel sentinel, std::endian endian, usize file_offset, basic_field_type field_type) -> std::expected<T, error> {
    const auto file_width = field_byte_width(field_type);
    auto ret = T{};

    if (file_width == sizeof(u32)) {
        const auto temp = TRYX(read_scalar<u32>(it, sentinel, endian, file_offset, field_type));
        ret = static_cast<T>(std::bit_cast<float>(temp));
    } else if (file_width == sizeof(u64)) {
        const auto temp = TRYX(read_scalar<u64>(it, sentinel, endian, file_offset, field_type));
        ret = static_cast<T>(std::bit_cast<double>(temp));
    } else {
        return std::unexpected{error::new_binary_error(error_class::internal, file_offset, "unsupported file width for floating point data, expected 4 or 8")};
    }

    return ret;
}

template<typename T>
struct property_traits;

template<typename T>
    requires(std::integral<T> || std::floating_point<T>)
struct property_traits<T> {
    static constexpr auto parse_from_ascii(std::span<const token> tokens, usize line) -> std::expected<std::pair<T, usize>, error> {
        if (tokens.empty()) {
            return std::unexpected{error::new_line_error(error_class::insufficient_tokens, line, "not ebnough tokens to parse a scalar")};
        }

        const auto res = TRYX(tokens[0].as_scalar<T>());

        return std::pair(res, 1);
    }

    template<typename It, std::sentinel_for<It> Sentinel>
    static constexpr auto parse_from_binary(It& it, Sentinel sentinel, std::endian endian, field_type field, usize offset) -> std::expected<std::pair<T, usize>, error> {
        if (!std::holds_alternative<basic_field_type>(field)) {
            return std::unexpected{error::new_binary_error(error_class::internal, offset, "expected to parse a scalar type")};
        }

        const auto basic_field = std::get<basic_field_type>(field);
        const auto size_in_file = field_byte_width(basic_field);

        return std::pair(TRYX(read_scalar<T>(it, sentinel, endian, offset, basic_field)), size_in_file);
    }
};

template<typename T>
struct property_traits<std::vector<T>> {
    static constexpr auto parse_from_ascii(std::span<const token> tokens, usize line) -> std::expected<std::pair<std::vector<T>, usize>, error> {
        if (tokens.empty()) {
            return std::unexpected{error::new_line_error(error_class::insufficient_tokens, line, "not enough tokens to parse a list")};
        }

        const auto list_size = TRYX(tokens[0].as_scalar<usize>());

        if (tokens.size() + 1 < list_size) {
            return tokens[0].generate_error(error_class::bad_data_in_token, "list declares more items than there are tokens available");
        }

        auto ret = std::vector<T>(list_size);

        for (auto i = 0uz; i < list_size; i++) {
            ret[i] = TRYX(tokens[i + 1].as_scalar<T>());
        }

        return std::pair(std::move(ret), list_size + 1);
    }

    template<typename It, std::sentinel_for<It> Sentinel>
    static constexpr auto parse_from_binary(It& it, Sentinel sentinel, std::endian endian, field_type field, usize offset)
      -> std::expected<std::pair<std::vector<T>, usize>, error> {
        if (!std::holds_alternative<list_field_type>(field)) {
            return std::unexpected{error::new_binary_error(error_class::internal, offset, "expected to parse a scalar type")};
        }
        const auto [index_type, value_type] = std::get<list_field_type>(field);

        const auto list_size = TRYX(read_scalar<usize>(it, sentinel, endian, offset, index_type));

        auto ret = std::vector<T>(list_size);

        for (auto i = 0uz; i < list_size; i++) {
            const auto cur_offset = offset + field_byte_width(index_type) + i * field_byte_width(value_type);
            ret[i] = TRYX(read_scalar<T>(it, sentinel, endian, cur_offset, value_type));
        }

        const auto total_size = field_byte_width(index_type) + list_size * field_byte_width(value_type);

        return std::pair(ret, total_size);
    }
};

template<typename T, usize N>
struct property_traits<std::array<T, N>> {
    static constexpr auto parse_from_ascii(std::span<const token> tokens, usize line) -> std::expected<std::pair<std::array<T, N>, usize>, error> {
        if (tokens.empty()) {
            return std::unexpected{error::new_line_error(error_class::insufficient_tokens, line, "not enough tokens to parse a fixed-size list")};
        }

        const auto list_size = TRYX(tokens[0].template as_scalar<usize>());

        if (tokens.size() + 1 < list_size) {
            return tokens[0].generate_error(error_class::bad_data_in_token, "list declares more items than there are tokens available");
        }

        if (list_size != N) {
            return tokens[0].generate_error(error_class::bad_data_in_token, "list declares an unexpected number of items");
        }

        auto ret = std::array<T, N>{};

        for (auto i = 0uz; i < list_size; i++) {
            ret[i] = TRYX(tokens[i + 1].template as_scalar<T>());
        }

        return std::pair(ret, list_size + 1);
    }

    template<typename It, std::sentinel_for<It> Sentinel>
    static constexpr auto parse_from_binary(It& it, Sentinel sentinel, std::endian endian, field_type field, usize offset)
      -> std::expected<std::pair<std::array<T, N>, usize>, error> {
        if (!std::holds_alternative<list_field_type>(field)) {
            return std::unexpected{error::new_binary_error(error_class::internal, offset, "expected to parse a scalar type")};
        }
        const auto [index_type, value_type] = std::get<list_field_type>(field);

        const auto list_size = TRYX(read_scalar<usize>(it, sentinel, endian, offset, index_type));

        if (list_size != N) {
            return std::unexpected{error::new_binary_error(error_class::bad_data_in_token, offset, "list declares an unexpected number of items")};
        }

        auto ret = std::array<T, N>();

        for (auto i = 0uz; i < list_size; i++) {
            const auto cur_offset = offset + field_byte_width(index_type) + i * field_byte_width(value_type);
            ret[i] = TRYX(read_scalar<T>(it, sentinel, endian, cur_offset, value_type));
        }

        const auto total_size = field_byte_width(index_type) + list_size * field_byte_width(value_type);

        return std::pair(ret, total_size);
    }
};

}  // namespace detail

}  // namespace stf::ply
