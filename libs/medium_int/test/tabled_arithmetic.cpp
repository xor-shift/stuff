#include "./random.hpp"

#include <stuff/medium_int.hpp>

#include <gtest/gtest.h>

namespace {

}

TEST(medium_int, arithmetic_tabled_add_sub) {
    using mi_u8_8 = stf::medium_int<8, u8>;
    using mi_u8_7 = stf::medium_int<7, u8>;

    ASSERT_EQ(mi_u8_8(0) + mi_u8_8(0), mi_u8_8(0));
    ASSERT_EQ(mi_u8_8(1) + mi_u8_8(0), mi_u8_8(1));
    ASSERT_EQ(mi_u8_8(0) + mi_u8_8(1), mi_u8_8(1));
    ASSERT_EQ(mi_u8_8(1) + mi_u8_8(1), mi_u8_8(2));
    ASSERT_EQ(mi_u8_8(1) + mi_u8_8(255), mi_u8_8(0));
    ASSERT_EQ(mi_u8_8(1) + mi_u8_8(127), mi_u8_8(128));
    ASSERT_EQ(mi_u8_7(1) + mi_u8_7(127), mi_u8_7(0));
    ASSERT_EQ(mi_u8_7(1) + mi_u8_7(126), mi_u8_7(127));
}
