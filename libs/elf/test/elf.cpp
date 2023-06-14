#include <stuff/elf.hpp>

#include "./test_files/rv32gc.hpp"
#include "./test_files/rv64gc.hpp"

#include <gtest/gtest.h>

namespace elf = stf::elf;

TEST(elf, header_ident_32) {
    const auto ident = elf::identification::from_bytes({rvtest32_elf});

    ASSERT_TRUE(static_cast<bool>(ident)) << "error: " << (ident.has_value() ? "" : ident.error());
    ASSERT_EQ(ident->magic[0], 0x7f);
    ASSERT_EQ(ident->magic[1], 'E');
    ASSERT_EQ(ident->magic[2], 'L');
    ASSERT_EQ(ident->magic[3], 'F');
    ASSERT_EQ(ident->ei_class, elf::elf_class::class_32);
    ASSERT_EQ(ident->ei_data, elf::data_format::le_two);
    ASSERT_EQ(ident->version, 1);
    ASSERT_EQ(ident->os_abi, static_cast<u8>(elf::os_abi::system_v));
    ASSERT_EQ(ident->abi_version, 0);
}

TEST(elf, header_ident_64) {
    const auto ident = elf::identification::from_bytes({rvtest64_elf});

    ASSERT_TRUE(static_cast<bool>(ident)) << "error: " << (ident.has_value() ? "" : ident.error());
    ASSERT_EQ(ident->magic[0], 0x7f);
    ASSERT_EQ(ident->magic[1], 'E');
    ASSERT_EQ(ident->magic[2], 'L');
    ASSERT_EQ(ident->magic[3], 'F');
    ASSERT_EQ(ident->ei_class, elf::elf_class::class_64);
    ASSERT_EQ(ident->ei_data, elf::data_format::le_two);
    ASSERT_EQ(ident->version, 1);
    ASSERT_EQ(ident->os_abi, static_cast<u8>(elf::os_abi::system_v));
    ASSERT_EQ(ident->abi_version, 0);
}

TEST(elf, header_32) {
    const auto header = elf::header::from_bytes({rvtest32_elf});

    ASSERT_TRUE(static_cast<bool>(header)) << "error: " << (header.has_value() ? "" : header.error());
    ASSERT_EQ(header->elf_type, static_cast<u16>(elf::elf_type::executable));
    ASSERT_EQ(header->machine_type, static_cast<u16>(elf::machine_type::risc_v));
    ASSERT_EQ(header->elf_version, 1);

    ASSERT_EQ(header->entry_point, 0);
    ASSERT_EQ(header->program_header_table_offset, 0x0034);
    ASSERT_EQ(header->segment_header_table_offset, 0x13d4);

    ASSERT_EQ(header->flags, 1);
    ASSERT_EQ(header->header_size, 52);

    ASSERT_EQ(header->program_header_table_entry_size, 32);
    ASSERT_EQ(header->program_header_table_entry_count, 3);
    ASSERT_EQ(header->segment_header_table_entry_size, 40);
    ASSERT_EQ(header->segment_header_table_entry_count, 15);
    ASSERT_EQ(header->segment_names_index, 14);
}

TEST(elf, header_64) {
    const auto header = elf::header::from_bytes({rvtest64_elf});

    ASSERT_TRUE(static_cast<bool>(header)) << "error: " << (header.has_value() ? "" : header.error());
    ASSERT_EQ(header->elf_type, static_cast<u16>(elf::elf_type::executable));
    ASSERT_EQ(header->machine_type, static_cast<u16>(elf::machine_type::risc_v));
    ASSERT_EQ(header->elf_version, 1);

    ASSERT_EQ(header->entry_point, 0);
    ASSERT_EQ(header->program_header_table_offset, 0x0040);
    ASSERT_EQ(header->segment_header_table_offset, 0x14d8);

    ASSERT_EQ(header->flags, 1);
    ASSERT_EQ(header->header_size, 64);

    ASSERT_EQ(header->program_header_table_entry_size, 56);
    ASSERT_EQ(header->program_header_table_entry_count, 3);
    ASSERT_EQ(header->segment_header_table_entry_size, 64);
    ASSERT_EQ(header->segment_header_table_entry_count, 15);
    ASSERT_EQ(header->segment_names_index, 14);
}

TEST(elf, program_header_32) {
    const auto elf = elf::elf::from_bytes({rvtest32_elf});

    ASSERT_TRUE(static_cast<bool>(elf)) << "error: " << (elf.has_value() ? "" : elf.error());

    const stf::expected<elf::program_header, std::string_view> p_hdrs[]{
      elf->get_program_header(0),
      elf->get_program_header(1),
      elf->get_program_header(2),
    };

    for (usize i = 0; auto const& header : p_hdrs) {
        ASSERT_TRUE(header) << "phdr #" << i++ << ", error: " << (header.has_value() ? "" : header.error());
    }

    EXPECT_EQ(p_hdrs[0]->type, 0x7000'0003);
    EXPECT_TRUE(p_hdrs[0]->is_read());
    EXPECT_FALSE(p_hdrs[0]->is_write());
    EXPECT_FALSE(p_hdrs[0]->is_execute());
    EXPECT_EQ(p_hdrs[0]->file_offset, 0x103e);
    EXPECT_EQ(p_hdrs[0]->virtual_address, 0x0000);
    EXPECT_EQ(p_hdrs[0]->physical_address, 0x0000);
    EXPECT_EQ(p_hdrs[0]->file_size, 0x002f);
    EXPECT_EQ(p_hdrs[0]->memory_size, 0x0000);
    EXPECT_EQ(p_hdrs[0]->alignment, 1);

    EXPECT_EQ(p_hdrs[1]->type, static_cast<u32>(elf::program_segment_type::load));
    EXPECT_TRUE(p_hdrs[1]->is_read());
    EXPECT_TRUE(p_hdrs[1]->is_write());
    EXPECT_TRUE(p_hdrs[1]->is_execute());
    EXPECT_EQ(p_hdrs[1]->file_offset, 0x1000);
    EXPECT_EQ(p_hdrs[1]->virtual_address, 0x0000);
    EXPECT_EQ(p_hdrs[1]->physical_address, 0x0000);
    EXPECT_EQ(p_hdrs[1]->file_size, 0x003e);
    EXPECT_EQ(p_hdrs[1]->memory_size, 0x0040);
    EXPECT_EQ(p_hdrs[1]->alignment, 0x1000);

    EXPECT_EQ(p_hdrs[2]->type, static_cast<u32>(elf::program_segment_type::load));
    EXPECT_TRUE(p_hdrs[2]->is_read());
    EXPECT_TRUE(p_hdrs[2]->is_write());
    EXPECT_FALSE(p_hdrs[2]->is_execute());
    EXPECT_EQ(p_hdrs[2]->file_offset, 0x0800);
    EXPECT_EQ(p_hdrs[2]->virtual_address, 0x3f800);
    EXPECT_EQ(p_hdrs[2]->physical_address, 0x3f800);
    EXPECT_EQ(p_hdrs[2]->file_size, 0x0000);
    EXPECT_EQ(p_hdrs[2]->memory_size, 0x0800);
    EXPECT_EQ(p_hdrs[2]->alignment, 0x1000);
}

TEST(elf, section_header_32) {
    const auto elf = elf::elf::from_bytes({rvtest32_elf});

    ASSERT_TRUE(static_cast<bool>(elf)) << "error: " << (elf.has_value() ? "" : elf.error());

    std::vector<elf::section_header> s_hdrs;

    for (usize i = 0; i < elf->m_header.segment_header_table_entry_count; i++) {
        auto&& header = elf->get_section_header(i);
        ASSERT_TRUE(header) << "shdr #" << i++ << ", error: " << (header.has_value() ? "" : header.error());
        s_hdrs.emplace_back(std::move(*header));
    }

    // clang-format off
    static constexpr elf::section_header expected_hdrs[]{
    // name type                                                    flags                                                                                      address  offset  size    lk  inf al es
      {0,   0,                                                      0,                                                                                         0,       0,      0,      0,  0,  0, 0},  // null
      {27,  static_cast<u32>(elf::section_type::program_bits),      static_cast<u32>(elf::section_flag::alloc) | static_cast<u32>(elf::section_flag::exec),    0,       0x1000, 0x1c,   0,  0,  4, 0},  // .init
      {33,  static_cast<u32>(elf::section_type::program_bits),      static_cast<u32>(elf::section_flag::alloc) | static_cast<u32>(elf::section_flag::exec),    0x0001c, 0x101c, 0x22,   0,  0,  4, 0},  // .text
      {39,  static_cast<u32>(elf::section_type::no_bits),           static_cast<u32>(elf::section_flag::alloc) | static_cast<u32>(elf::section_flag::write),   0x0003e, 0x103e, 0x2,    0,  0,  1, 0},  // .rodata
      {47,  static_cast<u32>(elf::section_type::preinit_array),     static_cast<u32>(elf::section_flag::alloc) | static_cast<u32>(elf::section_flag::write),   0x00040, 0x103e, 0,      0,  0,  1, 4},  // .preinit_array
      {62,  static_cast<u32>(elf::section_type::init_array),        static_cast<u32>(elf::section_flag::alloc) | static_cast<u32>(elf::section_flag::write),   0x00040, 0x103e, 0,      0,  0,  1, 4},  // .init_array
      {74,  static_cast<u32>(elf::section_type::fini_array),        static_cast<u32>(elf::section_flag::alloc) | static_cast<u32>(elf::section_flag::write),   0x00040, 0x103e, 0,      0,  0,  1, 4},  // .fini_array
      {86,  static_cast<u32>(elf::section_type::program_bits),      static_cast<u32>(elf::section_flag::alloc) | static_cast<u32>(elf::section_flag::write),   0x20000, 0x103e, 0,      0,  0,  1, 0},  // ..data
      {92,  static_cast<u32>(elf::section_type::no_bits),           static_cast<u32>(elf::section_flag::alloc) | static_cast<u32>(elf::section_flag::write),   0x20000, 0,      0,      0,  0,  1, 0},  // .bss
      {97,  static_cast<u32>(elf::section_type::no_bits),           static_cast<u32>(elf::section_flag::alloc) | static_cast<u32>(elf::section_flag::write),   0x3f800, 0x1800, 0x0800, 0,  0,  1, 0},  // ._user_heap_stack
      {115, static_cast<u32>(elf::section_type::risc_v_attributes), 0,                                                                                         0,       0x103e, 0x002f, 0,  0,  1, 0},  // .riscv.attributes
      {133, static_cast<u32>(elf::section_type::program_bits),      static_cast<u32>(elf::section_flag::merge) | static_cast<u32>(elf::section_flag::strings), 0,       0x106d, 0x001b, 0,  0,  1, 1},  // .comment
      {1,   static_cast<u32>(elf::section_type::symbol_table),      0,                                                                                         0,       0x1088, 0x0200, 13, 19, 4, 16}, // .symtab
      {9,   static_cast<u32>(elf::section_type::string_table),      0,                                                                                         0,       0x1288, 0x00be, 0,  0,  1, 0},  // .strtab
      {17,  static_cast<u32>(elf::section_type::string_table),      0,                                                                                         0,       0x1346, 0x008e, 0,  0,  1, 0},  // .shstrtab
    };
    // clang-format on

    static constexpr const char* section_names[]{
      "",                   //
      ".init",              //
      ".text",              //
      ".rodata",            //
      ".preinit_array",     //
      ".init_array",        //
      ".fini_array",        //
      ".data",              //
      ".bss",               //
      "._user_heap_stack",  //
      ".riscv.attributes",  //
      ".comment",           //
      ".symtab",            //
      ".strtab",            //
      ".shstrtab",          //
    };

    ASSERT_EQ(std::size(s_hdrs), std::size(expected_hdrs));

    for (usize i = 0; i < std::size(expected_hdrs); i++) {
        const auto got = s_hdrs[i];
        const auto expected = expected_hdrs[i];
        EXPECT_EQ(got, expected) << "shdr #" << i;
        EXPECT_EQ(elf->section_name(got), section_names[i]);
    }
}
