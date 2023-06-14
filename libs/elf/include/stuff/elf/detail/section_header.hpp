#pragma once

#include <stuff/elf/detail/consumer.hpp>

#include <stuff/expected.hpp>

namespace stf::elf {

enum class section_type : u32 {
    inactive = 0x00,                  //!< SHT_NULL
    program_bits = 0x01,              //!< SHT_PROGBITS
    symbol_table = 0x02,              //!< SHT_SYMTAB
    string_table = 0x03,              //!< SHT_STRTAB
    relocation_entries = 0x04,        //!< SHT_RELA, relocation entries with explicit addends
    hash_table = 0x05,                //!< SHT_HASH
    dynamic = 0x06,                   //!< SHT_DYNAMIC
    note = 0x07,                      //!< SHT_NOTE
    no_bits = 0x08,                   //!< SHT_NOBITS
    relocation_offsets = 0x09,        //!< SHT_REL, relocation offsets without explicit addends

    dynamic_linking_symbols = 0x0B,   //!< SHT_DYNSYM

    init_array = 0x0E,                //!< SHT_INIT_ARRAY
    fini_array = 0x0F,                //!< SHT_FINI_ARRAY
    preinit_array = 0x10,             //!< SHT_PREINIT_ARRAY

    section_group = 0x11,             //!< SHT_GROUP
    extended_section_indices = 0x12,  //!< SHT_SYMTAB_SHNDX

    risc_v_attributes = 0x70000003,   //!< RISCV_ATTRIBUTES
};

enum class section_flag : u64 {
    write = 0x0001,                 //!< SHF_WRITE
    alloc = 0x0002,                 //!< SHF_ALLOC
    exec = 0x0004,                  //!< SHF_EXECINSTR
    merge = 0x0010,                 //!< SHF_MERGE
    strings = 0x0020,               //!< SHF_STRINGS
    info_link = 0x0040,             //!< SHF_INFO_LINK, the <code>info</code> member contains section header index in the SH table, TODO: not a descriptive name
    preserve_link_order = 0x0080,   //!< SHF_LINK_ORDER
    os_nonconforming = 0x0100,      //!< SHF_OS_NONCONFORMING
    member_of_group = 0x0200,       //!< SHF_GROUP
    thread_local_storage = 0x0400,  //!< SHF_TLS
};

struct section_header {
    u32 name_offset;
    u32 type;
    u64 flags;
    u64 virtual_address;
    u64 file_offset;
    u64 file_size;
    u32 index;
    u32 info;
    u64 alignment;
    u64 entry_size;

    static constexpr auto from_consumer(detail::data_consumer& consumer) -> stf::expected<section_header, std::string_view> {
        auto ret = section_header{
          .name_offset = consumer.consume<u32>(),
          .type = consumer.consume<u32>(),
          .flags = consumer.consume_address(),
          .virtual_address = consumer.consume_address(),
          .file_offset = consumer.consume_address(),
          .file_size = consumer.consume_address(),
          .index = consumer.consume<u32>(),
          .info = consumer.consume<u32>(),
          .alignment = consumer.consume_address(),
          .entry_size = consumer.consume_address(),
        };

        if (const auto err = ret.get_error(); err) {
            return stf::unexpected{*err};
        }

        return ret;
    }

    friend constexpr auto operator==(section_header const& lhs, section_header const& rhs) -> bool {
        return lhs.name_offset == rhs.name_offset &&          //
               lhs.type == rhs.type &&                        //
               lhs.flags == rhs.flags &&                      //
               lhs.virtual_address == rhs.virtual_address &&  //
               lhs.file_offset == rhs.file_offset &&          //
               lhs.file_size == rhs.file_size &&              //
               lhs.index == rhs.index &&                      //
               lhs.info == rhs.info &&                        //
               lhs.alignment == rhs.alignment &&              //
               lhs.entry_size == rhs.entry_size;
    }

private:
    constexpr auto get_error() const -> std::optional<std::string_view> { return std::nullopt; }
};

}  // namespace stf::elf
