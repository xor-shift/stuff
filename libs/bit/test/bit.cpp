#include <stuff/bit.hpp>

#include <gtest/gtest.h>

TEST(bit, reverse_bits) {
    using stf::bit::reverse_bits;

    ASSERT_EQ(reverse_bits(static_cast<uint8_t>(1)), UINT8_C(0x80));
    ASSERT_EQ(reverse_bits(static_cast<uint16_t>(1)), UINT16_C(0x8000));
    ASSERT_EQ(reverse_bits(static_cast<uint32_t>(1)), UINT32_C(0x8000'0000));
    ASSERT_EQ(reverse_bits(static_cast<uint64_t>(1)), UINT64_C(0x8000'0000'0000'0000));

    static_assert(reverse_bits(static_cast<uint8_t>(1)) == UINT8_C(0x80));
}

TEST(bit, reverse_bytes) {
    using stf::bit::reverse_bytes;

    ASSERT_EQ(reverse_bytes(static_cast<uint8_t>(1)), UINT8_C(0x01));
    ASSERT_EQ(reverse_bytes(static_cast<uint16_t>(1)), UINT16_C(0x0100));
    ASSERT_EQ(reverse_bytes(static_cast<uint32_t>(1)), UINT32_C(0x0100'0000));
    ASSERT_EQ(reverse_bytes(static_cast<uint64_t>(1)), UINT64_C(0x0100'0000'0000'0000));

    static_assert(reverse_bytes(static_cast<uint8_t>(1)) == UINT8_C(0x01));
}

TEST(bit, bit_extraction_info) {
    using stf::bit::detail::generate_extraction_info;

    ASSERT_EQ(std::get<0>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[0]), 0b0000'0011'0000);
    ASSERT_EQ(std::get<0>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[1]), 0b0011'1100'0000);
    ASSERT_EQ(std::get<0>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[2]), 0b0000'0000'0100);
    ASSERT_EQ(std::get<0>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[3]), 0b0000'0000'1000);
    ASSERT_EQ(std::get<1>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[0]), 0);
    ASSERT_EQ(std::get<1>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[1]), 0);
    ASSERT_EQ(std::get<1>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[2]), 0);
    ASSERT_EQ(std::get<1>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[3]), 0);
    ASSERT_EQ(std::get<2>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[0]), 7);
    ASSERT_EQ(std::get<2>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[1]), 1);
    ASSERT_EQ(std::get<2>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[2]), 4);
    ASSERT_EQ(std::get<2>(generate_extraction_info<u32, 12, "5:4|9:6|2|3">()[3]), 2);

    ASSERT_EQ(std::get<2>(generate_extraction_info<u32, 12, "5|4~0|4:0">()[0]), 7);
    ASSERT_EQ(std::get<2>(generate_extraction_info<u32, 12, "5|4~0|4:0">()[1]), 2);
}

TEST(bit, bit_extract) {
    using stf::bit::extract;

    ASSERT_EQ((extract<u16, 12, "5:4|9:6|2|3">(0x18bc)), 120);
    ASSERT_EQ((extract<u16, 12, "5|4~0|4:0">(0x9051)), 52);
}
