#pragma once

#include <stuff/ply/detail/header.hpp>
#include <stuff/ply/detail/token.hpp>

#include <stuff/core.hpp>

namespace stf::ply {

template<typename It, std::sentinel_for<It> Sentinel>
struct context {
    constexpr context(It it, Sentinel sentinel)
        : m_iter(it)
        , m_sentinel(sentinel) {}

    template<std::ranges::range Range>
    constexpr context(Range&& range)
        : m_iter(std::ranges::begin(range))
        , m_sentinel(std::ranges::end(range)) {}

    constexpr auto read_header() -> stf::expected<file_header, error> { return m_header = TRYX(read_header_impl()); }

    constexpr auto header() const& -> file_header const& { return m_header; }
    constexpr auto header() & -> file_header& { return m_header; }

    template<typename ElementType>
    constexpr auto read_element(usize element_index) -> stf::expected<ElementType, error> {
        using element_traits = detail::element_traits<ElementType>;

        if constexpr (element_traits::property_count == 0) {
            static_assert(std::is_default_constructible_v<ElementType>);
            return {};
        } else {
            auto ret = ElementType{};

            if (m_header.m_format.m_data_format == data_format::ascii) {
                const auto line = TRYX(read_line());
                TRYX(read_element_impl_ascii<ElementType>(ret, line.tokens()));
            } else {
                const auto endian = m_header.m_format.m_data_format == data_format::binary_be ? std::endian::big : std::endian::little;
                TRYX(read_element_impl_binary<ElementType>(ret, endian, m_header.m_elements[element_index].m_properties));
            }

            return ret;
        }
    }

private:
    It m_iter;
    Sentinel m_sentinel;

    usize m_row = 0;
    usize m_binary_offset = 0;
    file_header m_header;

    constexpr auto read_line() -> stf::expected<detail::line, error> {
        auto ret = std::string{};

        while (m_iter != m_sentinel) {
            const auto c = *m_iter++;

            if (c == '\n') {
                if (ret.ends_with('\r')) {
                    ret.pop_back();
                }

                return detail::line{
                  .m_content = std::move(ret),
                  .m_row = m_row++,
                };
            }

            ret.push_back(c);
        }

        return stf::unexpected{error{
          .m_class = error_class::premature_eof,
          .m_location = std::monostate{},
        }};
    }

    constexpr auto read_header_impl() -> stf::expected<file_header, error> {
        TRYX(parse_magic_line(TRYX(read_line()).tokens()));

        enum class state {
            expecting_magic,
            expecting_format,
            expecting_element,
            expecting_property,
            expecting_element_or_property,
        } state = state::expecting_format;

        enum class line_type {
            empty,
            magic,
            comment,
            format,
            element,
            property,
            end_header,
        };

        constexpr auto get_line_type = [](detail::token leading) -> stf::expected<line_type, error> {
            if (leading.m_content == "ply") {
                return line_type::magic;
            }

            if (leading.m_content == "comment") {
                return line_type::comment;
            }

            if (leading.m_content == "format") {
                return line_type::format;
            }

            if (leading.m_content == "element") {
                return line_type::element;
            }

            if (leading.m_content == "property") {
                return line_type::property;
            }

            if (leading.m_content == "end_header") {
                return line_type::end_header;
            }

            return leading.generate_error(error_class::bad_data_in_token, "unexpected line type");
        };

        auto ret = file_header{};

        auto process_comment = [&ret](detail::line&& line, line_type type) {
            if (type != line_type::comment) {
                return false;
            }

            stf::assume(line.m_content.starts_with("comment"));

            line.m_content.erase(0, "comment"sv.size());
            if (const auto pos = line.m_content.find_first_not_of(' '); pos != std::string::npos) {
                line.m_content.erase(0, pos);
            }

            ret.m_comments.emplace_back(std::move(line.m_content));

            return true;
        };

        auto cur_elem = std::optional<element>{std::nullopt};
        auto flush_element = [&ret, &cur_elem] {
            auto candidate = std::move(cur_elem);
            if (!candidate) {
                return;
            }

            ret.m_elements.emplace_back(std::move(*candidate));
        };

        auto process_element = [&flush_element, &cur_elem](std::span<const detail::token> tokens, usize row) -> stf::expected<void, error> {
            flush_element();

            cur_elem = TRYX(element::from_tokens(tokens, row));

            return {};
        };

        auto process_property = [&cur_elem](std::span<const detail::token> tokens, usize row) -> stf::expected<void, error> {
            if (!cur_elem) {
                return stf::unexpected{error::new_line_error(error_class::internal, row, "state is expecting_property despite cur_elem being std::nullopt")};
            }

            cur_elem->m_properties.emplace_back(TRYX(property::from_tokens(tokens, row)));

            return {};
        };

        for (;;) {
            auto&& line = TRYX(read_line());
            const auto tokens = line.tokens();

            const auto line_type = tokens.empty() ? line_type::empty : TRYX(get_line_type(tokens[0]));

            if (process_comment(std::move(line), line_type)) {
                continue;
            }

            if (line_type == line_type::end_header) {
                break;
            }

            if (line_type == line_type::empty) {
                continue;
            }

            switch (state) {
                case state::expecting_format:
                    if (line_type != line_type::format) {
                        return tokens[0].generate_error(error_class::bad_data_in_token, "expected format");
                    }

                    ret.m_format = TRYX(file_format::from_tokens(tokens, m_row - 1));

                    state = state::expecting_element;
                    break;

                case state::expecting_element:
                    if (line_type != line_type::element) {
                        return tokens[0].generate_error(error_class::bad_data_in_token, "expected element");
                    }

                    TRYX(process_element(tokens, m_row - 1));

                    state = state::expecting_property;
                    break;

                case state::expecting_property:
                    if (line_type != line_type::property) {
                        return tokens[0].generate_error(error_class::bad_data_in_token, "expected property");
                    }

                    TRYX(process_property(tokens, m_row - 1));

                    state = state::expecting_element_or_property;
                    break;

                case state::expecting_element_or_property:
                    if (line_type == line_type::element) {
                        TRYX(process_element(tokens, m_row - 1));
                        break;
                    }

                    if (line_type == line_type::property) {
                        TRYX(process_property(tokens, m_row - 1));
                        break;
                    }

                    return tokens[0].generate_error(error_class::bad_data_in_token, "expected element or property");
            }
        }

        flush_element();

        return ret;
    }

    template<typename ElementType, usize I = 0>
    constexpr auto read_element_impl_ascii(ElementType& out, std::span<const detail::token> tokens) -> stf::expected<void, error> {
        using element_traits = detail::element_traits<ElementType>;
        using property = element_traits::template nth_type<I>;
        using property_traits = detail::property_traits<property>;

        auto [res, consumed] = TRYX(property_traits::parse_from_ascii(tokens, m_row - 1));

        if constexpr (element_traits::is_tuple) {
            std::get<I>(out) = res;
        } else {
            out = res;
        }

        if constexpr (I + 1 < detail::element_traits<ElementType>::property_count) {
            return read_element_impl_ascii<ElementType, I + 1>(out, tokens.subspan(consumed));
        }

        return {};
    }

    template<typename ElementType, usize I = 0>
    constexpr auto read_element_impl_binary(ElementType& out, std::endian endianness, std::span<const property> property_descriptions) -> stf::expected<void, error> {
        using element_traits = detail::element_traits<ElementType>;
        using property = element_traits::template nth_type<I>;
        using property_traits = detail::property_traits<property>;

        auto const& description = property_descriptions[I];

        auto [res, consumed] = TRYX(property_traits::parse_from_binary(m_iter, m_sentinel, endianness, description.m_field, m_binary_offset));

        if constexpr (element_traits::is_tuple) {
            std::get<I>(out) = res;
        } else {
            out = res;
        }

        m_binary_offset += consumed;

        if constexpr (I + 1 < detail::element_traits<ElementType>::property_count) {
            return read_element_impl_binary<ElementType, I + 1>(out, endianness, property_descriptions);
        }

        return {};
    }

    static constexpr auto parse_magic_line(std::span<const detail::token> tokens) -> stf::expected<void, error> {
        if (tokens.size() != 1) {
            return stf::unexpected{error::new_line_error(error_class::insufficient_or_excess_tokens, 0)};
        }

        if (tokens[0].m_content != "ply") {
            return tokens[0].generate_error(error_class::bad_data_in_token, "expected \"ply\"");
        }

        return {};
    }
};

template<std::ranges::range Range>
context(Range&& range) -> context<decltype(std::ranges::begin(std::declval<Range>())), decltype(std::ranges::end(std::declval<Range>()))>;

}  // namespace stf::ply
