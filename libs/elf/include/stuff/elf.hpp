#pragma once

#include <stuff/elf/detail/header.hpp>
#include <stuff/elf/detail/program_header.hpp>
#include <stuff/elf/detail/section_header.hpp>

namespace stf::elf {

namespace detail {

template<typename T>
struct segment_iterator {
    using value_type = std::expected<T, std::string_view>;
    using reference = std::expected<T, std::string_view>&;

    data_consumer m_consumer;

    constexpr auto operator*() -> value_type {
        return T::from_consumer(m_consumer);
    }

    constexpr auto operator++() -> segment_iterator& { return *this; }
};

template<typename T>
constexpr auto operator!=(segment_iterator<T> const& lhs, segment_iterator<T> const& rhs) {
    return lhs.m_consumer.read_head != rhs.m_consumer.read_head;
}

struct segment_iterator_sentinel {
    usize m_end_address;
};

template<typename T>
constexpr auto operator!=(segment_iterator<T> const& lhs, segment_iterator_sentinel rhs) {
    return lhs.m_consumer.read_head < rhs.m_end_address;
}

template<typename T>
struct segment_range {
    data_consumer m_consumer;
    usize m_end_address;

    constexpr auto begin() const -> segment_iterator<T> { return { m_consumer };}
    constexpr auto end() const -> segment_iterator_sentinel { return { m_end_address };}
};

}

struct elf {
    header m_header;
    std::span<const u8> m_bytes{};

    template<usize Extent = std::dynamic_extent>
    static constexpr auto from_bytes(std::span<const u8, Extent> bytes) -> std::expected<elf, std::string_view> {
        auto ret = elf{
          .m_header = TRYX(header::from_bytes(bytes)),
          .m_bytes = bytes,
        };

        if (auto err = ret.get_error(); err) {
            return std::unexpected{*err};
        }

        return ret;
    }

    constexpr auto get_program_header(usize i) const -> std::expected<program_header, std::string_view> {
        auto consumer = get_consumer(m_header.program_header_table_offset + i * m_header.program_header_table_entry_size);
        const auto header = TRYX(program_header::from_consumer(consumer));

        if (!range_is_contained(header.file_offset, header.file_size)) {
            return std::unexpected { "section contents are out of range" };
        }

        return header;
    }


    constexpr auto get_section_header(usize i) const -> std::expected<section_header, std::string_view> {
        auto consumer = get_consumer(m_header.segment_header_table_offset + i * m_header.segment_header_table_entry_size);
        const auto header = TRYX(section_header::from_consumer(consumer));

        if (!range_is_contained(header.file_offset, header.file_size) && header.type != static_cast<u32>(section_type::no_bits)) {
            return std::unexpected { "section contents are out of range" };
        }

        return header;
    }

    constexpr auto sections() -> detail::segment_range<section_header> {
        return {
            .m_consumer = get_consumer(m_header.segment_header_table_offset),
            .m_end_address = m_header.segment_header_table_offset + m_header.segment_header_table_entry_count * m_header.segment_header_table_entry_size,
        };
    }

    constexpr auto section_name(section_header const& header) const -> auto {
        const auto res = get_section_header(m_header.segment_names_index);

        if (!res) {
            return "[bad shstrtab section]"s;
        }

        const auto shstrtab = *res;

        const auto* ptr = m_bytes.data() + header.name_offset + shstrtab.file_offset;

        if consteval {
            std::string ret;
            while (*ptr) {
                ret.push_back(static_cast<char>(*ptr++));
            }
            return ret;
        } else {
            return std::string(reinterpret_cast<const char*>(ptr));
        }
    }

private:
    constexpr auto get_error() const -> std::optional<std::string_view> {
        if (!range_is_contained(m_header.program_header_table_offset, m_header.program_header_table_entry_count * m_header.program_header_table_entry_size)) {
            return "program header table is not contained within the elf";
        }

        if (!range_is_contained(m_header.segment_header_table_offset, m_header.segment_header_table_entry_count * m_header.segment_header_table_entry_size)) {
            return "segment header table is not contained within the elf";
        }

        return std::nullopt;
    }

    constexpr auto range_is_contained(u64 address, u64 size) const -> bool { return size == 0 || (m_bytes.size() > address && m_bytes.size() >= (address + size)); }

    constexpr auto get_consumer(usize at_offset) const -> detail::data_consumer {
        auto consumer = detail::data_consumer{
          .bytes = m_bytes,
          .read_head = at_offset,
          .is_64 = m_header.e_ident.ei_class == elf_class::class_64,
          .endianness = m_header.e_ident.ei_data == data_format::be_two ? std::endian::big : std::endian::little,
        };

        return consumer;
    }
};

}  // namespace stf::elf
