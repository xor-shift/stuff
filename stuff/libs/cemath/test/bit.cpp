#include <stuff/cemath.hpp>

#include <gtest/gtest.h>

TEST(cemath, bit) {
    using stf::cemath::reverse_bits;
    using stf::cemath::reverse_bytes;

    ASSERT_EQ(reverse_bits(static_cast<uint8_t>(1)), UINT8_C(0x80));
    ASSERT_EQ(reverse_bits(static_cast<uint16_t>(1)), UINT16_C(0x8000));
    ASSERT_EQ(reverse_bits(static_cast<uint32_t>(1)), UINT32_C(0x8000'0000));
    ASSERT_EQ(reverse_bits(static_cast<uint64_t>(1)), UINT64_C(0x8000'0000'0000'0000));

    static_assert(reverse_bits(static_cast<uint8_t>(1)) == UINT8_C(0x80));

    ASSERT_EQ(reverse_bytes(static_cast<uint8_t>(1)), UINT8_C(0x01));
    ASSERT_EQ(reverse_bytes(static_cast<uint16_t>(1)), UINT16_C(0x0100));
    ASSERT_EQ(reverse_bytes(static_cast<uint32_t>(1)), UINT32_C(0x0100'0000));
    ASSERT_EQ(reverse_bytes(static_cast<uint64_t>(1)), UINT64_C(0x0100'0000'0000'0000));

    static_assert(reverse_bytes(static_cast<uint8_t>(1)) == UINT8_C(0x01));
}
