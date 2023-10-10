#pragma once

#include <stuff/elf/detail/consumer.hpp>
#include <stuff/elf/detail/ident.hpp>

namespace stf::elf {

enum class elf_type : u16 {
    none = 0x00,
    relocatable = 0x01,
    executable = 0x02,
    shared_object = 0x03,
    core_dump = 0x04,
};

enum class machine_type : u16 {
    none = 0x00,
    we32100 = 0x01,
    sparc = 0x02,
    i386 = 0x03,
    m68k = 0x04,
    m88k = 0x05,
    i860 = 0x06,
    mips = 0x08,
    pa_risc = 0x0f,
    powerpc = 0x14,
    powerpc64 = 0x15,
    s390 = 0x16,
    arm = 0x28,  //!< arm up to Armv7/AArch32
    super_h = 0x2a,
    sparc_v9 = 0x2b,
    ia64 = 0x32,
    amd64 = 0x3e,
    vax = 0x4b,
    risc_v = 0xf3,
};

struct header {
    identification e_ident;

    u16 elf_type;                          //!< e_type
    u16 machine_type;                      //!< e_machine
    u32 elf_version;                       //!< e_version

    u64 entry_point;                       //!< e_entry
    u64 program_header_table_offset;       //!< e_phoff
    u64 segment_header_table_offset;       //!< e_shoff

    u32 flags;                             //!< e_flags
    u16 header_size;                       //!< e_ehsize

    u16 program_header_table_entry_size;   //!< e_phentsize
    u16 program_header_table_entry_count;  //!< e_phnum
    u16 segment_header_table_entry_size;   //!< e_shentsize
    u16 segment_header_table_entry_count;  //!< e_shnum
    u16 segment_names_index;               //!< e_shstrndx

    template<usize Extent = std::dynamic_extent>
    static constexpr auto from_bytes(std::span<const u8, Extent> bytes) -> std::expected<header, std::string_view> {
        auto ret = header{.e_ident = TRYX(identification::from_bytes(bytes))};

        auto consumer = detail::data_consumer{
          .bytes = bytes,
          .read_head = 16,
          .is_64 = ret.e_ident.ei_class == elf_class::class_64,
          .endianness = ret.e_ident.ei_data == data_format::be_two ? std::endian::big : std::endian::little,
        };

        if (bytes.size() < (consumer.is_64 ? 64 : 52)) {
            return std::unexpected{"not enough bytes for an elf header"};
        }

        ret.elf_type = consumer.consume(std::type_identity<u16>{});
        ret.machine_type = consumer.consume(std::type_identity<u16>{});
        ret.elf_version = consumer.consume(std::type_identity<u32>{});

        ret.entry_point = consumer.consume_address();
        ret.program_header_table_offset = consumer.consume_address();
        ret.segment_header_table_offset = consumer.consume_address();

        ret.flags = consumer.consume(std::type_identity<u32>{});
        ret.header_size = consumer.consume(std::type_identity<u16>{});
        ret.program_header_table_entry_size = consumer.consume(std::type_identity<u16>{});
        ret.program_header_table_entry_count = consumer.consume(std::type_identity<u16>{});
        ret.segment_header_table_entry_size = consumer.consume(std::type_identity<u16>{});
        ret.segment_header_table_entry_count = consumer.consume(std::type_identity<u16>{});
        ret.segment_names_index = consumer.consume(std::type_identity<u16>{});

        if (auto err = ret.get_error(); err) {
            return std::unexpected{*err};
        }

        return ret;
    }

private:
    constexpr auto get_error() const -> std::optional<std::string_view> {
        const auto is_64 = e_ident.ei_class == elf_class::class_64;

        const auto expected_e_ehsize = is_64 ? 64 : 52;
        const auto expected_e_phentsize = is_64 ? 56 : 32;
        const auto expected_e_shentsize = is_64 ? 64 : 40;

        if (header_size != expected_e_ehsize) {
            return "inconsistent e_ehsize";
        }
        if (program_header_table_entry_size != expected_e_phentsize) {
            return "inconsistent e_phentsize";
        }
        if (segment_header_table_entry_size != expected_e_shentsize) {
            return "inconsistent e_shentsize";
        }

        return std::nullopt;
    }
};

}  // namespace stf::elf
