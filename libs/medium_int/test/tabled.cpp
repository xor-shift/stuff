#include "./stower.hpp"

#include <gtest/gtest.h>

namespace {

template<typename T, usize Bits>
constexpr auto mint_elem_ct = (Bits / std::numeric_limits<T>::digits) + ((Bits % std::numeric_limits<T>::digits) != 0 ? 1 : 0);

template<std::unsigned_integral T, usize Bits, typename TableType, usize TableExtent>
void test_table(auto stown_ptr, std::span<std::tuple<TableType, u64, std::array<T, mint_elem_ct<T, Bits>>>, TableExtent> table, usize table_no) {
    for (auto [test_no, table_row] : table | std::views::enumerate) {
        auto [value, expected_get_result, expected_repr] = table_row;

        auto test_integer = stf::medium_int<Bits, T>{};
        test_integer.set(value);

        const std::unsigned_integral auto re_get_value = test_integer.template get<u64>();
        ASSERT_EQ(re_get_value, expected_get_result);

        for (auto [i, comp] : std::views::zip(expected_repr, test_integer.*stown_ptr) | std::views::enumerate) {
            auto [expected, got] = comp;
            ASSERT_EQ(expected, got) << "table " << table_no << ", row " << test_no << ", repr byte #" << i;
        }
    }
}

template<typename T, usize Bits, usize TableNo = 0>
void test_tables(auto stown_ptr, auto table, auto... tables) {
    test_table<T, Bits>(stown_ptr, std::span{table}, TableNo);
    if constexpr (sizeof...(tables) != 0) {
        return test_tables<T, Bits, TableNo + 1>(stown_ptr, tables...);
    }
}

}

TEST(medium_int, basic_u8_8_unsigned) {
    test_tables<u8, 8>(
      STOWN_PTR(u8, 8),  //
      std::array<std::tuple<u8, u64, std::array<u8, 1>>, 3>{{
        {0, 0, {0}},
        {1, 1, {1}},
        {0xFF, 0xFF, {0xFF}},
      }},
      std::array<std::tuple<u16, u64, std::array<u8, 1>>, 5>{{
        {0, 0, {0}},
        {1, 1, {1}},
        {0xFF, 0xFF, {0xFF}},
        {0x1FF, 0xFF, {0xFF}},
        {0xFFFE, 0xFE, {0xFE}},
      }},
      std::array<std::tuple<i32, u64, std::array<u8, 1>>, 5>{{
        {0, 0, {0}},
        {1, 1, {1}},
        {0xFF, 0xFF, {0xFF}},
        {0x1FF, 0xFF, {0xFF}},
        {0xFFFE, 0xFE, {0xFE}},
      }},
      std::array<std::tuple<u64, u64, std::array<u8, 1>>, 2>{{
        {0, 0, {0}},
        {0x1000'0000'0000'00FDull, 0xFD, {0xFD}},
      }}
    );
}

TEST(medium_int, basic_u8_7_unsigned) {
    test_tables<u8, 7>(
      STOWN_PTR(u8, 7),  //
      std::array<std::tuple<u8, u64, std::array<u8, 1>>, 3>{{
        {0, 0, {0}},
        {1, 1, {1}},
        {0xFF, 0x7F, {0x7F}},
      }},
      std::array<std::tuple<u16, u64, std::array<u8, 1>>, 5>{{
        {0, 0, {0}},
        {1, 1, {1}},
        {0xFF, 0x7F, {0x7F}},
        {0x1FF, 0x7F, {0x7F}},
        {0xFFFE, 0x7E, {0x7E}},
      }},
      std::array<std::tuple<i32, u64, std::array<u8, 1>>, 5>{{
        {0, 0, {0}},
        {1, 1, {1}},
        {0xFF, 0x7F, {0x7F}},
        {0x1FF, 0x7F, {0x7F}},
        {0xFFFE, 0x7E, {0x7E}},
      }},
      std::array<std::tuple<u64, u64, std::array<u8, 1>>, 2>{{
        {0, 0, {0}},
        {0x1000'0000'0000'00FDull, 0x7D, {0x7D}},
      }}
    );
}

TEST(medium_int, basic_u8_16_unsigned) {
    test_tables<u8, 16>(
      STOWN_PTR(u8, 16),  //
      std::array<std::tuple<u8, u64, std::array<u8, 2>>, 3>{{
        {0, 0, {0, 0}},
        {1, 1, {1, 0}},
        {0xFF, 0xFF, {0xFF, 0}},
      }},
      std::array<std::tuple<u16, u64, std::array<u8, 2>>, 5>{{
        {0, 0, {0, 0}},
        {1, 1, {1, 0}},
        {0xFFFF, 0xFFFF, {0xFF, 0xFF}},
        {0x0100, 0x0100, {0, 1}},
        {0xFEFD, 0xFEFD, {0xFD, 0xFE}},
      }},
      std::array<std::tuple<i32, u64, std::array<u8, 2>>, 6>{{
        {0, 0, {0, 0}},
        {1, 1, {1, 0}},
        {0xFFFF, 0xFFFF, {0xFF, 0xFF}},
        {0x0100, 0x0100, {0, 1}},
        {0xFEFD, 0xFEFD, {0xFD, 0xFE}},
        {0x1FEFD, 0xFEFD, {0xFD, 0xFE}},
      }},
      std::array<std::tuple<u64, u64, std::array<u8, 2>>, 6>{{
        {0, 0, {0, 0}},
        {0xFFFF'FFFF'FFFF'FBFAull, 0xFBFA, {0xFA, 0xFB}},
      }}
    );
}

TEST(medium_int, basic_u8_15_unsigned) {
    test_tables<u8, 15>(
      STOWN_PTR(u8, 15),  //
      std::array<std::tuple<u8, u64, std::array<u8, 2>>, 3>{{
        {0, 0, {0, 0}},
        {1, 1, {1, 0}},
        {0xFF, 0xFF, {0xFF, 0}},
      }},
      std::array<std::tuple<u16, u64, std::array<u8, 2>>, 5>{{
        {0, 0, {0, 0}},
        {1, 1, {1, 0}},
        {0xFFFF, 0x7FFF, {0xFF, 0x7F}},
        {0x0100, 0x0100, {0, 1}},
        {0xFEFD, 0x7EFD, {0xFD, 0x7E}},
      }},
      std::array<std::tuple<i32, u64, std::array<u8, 2>>, 6>{{
        {0, 0, {0, 0}},
        {1, 1, {1, 0}},
        {0xFFFF, 0x7FFF, {0xFF, 0x7F}},
        {0x0100, 0x0100, {0, 1}},
        {0xFEFD, 0x7EFD, {0xFD, 0x7E}},
        {0x1FEFD, 0x7EFD, {0xFD, 0x7E}},
      }},
      std::array<std::tuple<u64, u64, std::array<u8, 2>>, 6>{{
        {0, 0, {0, 0}},
        {0xFFFF'FFFF'FFFF'FBFAull, 0x7BFA, {0xFA, 0x7B}},
      }}
    );
}

TEST(medium_int, basic_u8_9_unsigned) {
    test_tables<u8, 9>(
      STOWN_PTR(u8, 9),  //
      std::array<std::tuple<u8, u64, std::array<u8, 2>>, 3>{{
        {0, 0, {0, 0}},
        {1, 1, {1, 0}},
        {0xFF, 0xFF, {0xFF, 0}},
      }},
      std::array<std::tuple<u16, u64, std::array<u8, 2>>, 5>{{
        {0, 0, {0, 0}},
        {1, 1, {1, 0}},
        {0xFFFF, 0x1FF, {0xFF, 1}},
        {0x0100, 0x100, {0, 1}},
        {0xFEFD, 0x0FD, {0xFD, 0}},
      }},
      std::array<std::tuple<i32, u64, std::array<u8, 2>>, 6>{{
        {0, 0, {0, 0}},
        {1, 1, {1, 0}},
        {0xFFFF, 0x1FF, {0xFF, 1}},
        {0x0100, 0x100, {0, 1}},
        {0xFEFD, 0x0FD, {0xFD, 0}},
        {0x1FEFD, 0x0FD, {0xFD, 0}},
      }},
      std::array<std::tuple<u64, u64, std::array<u8, 2>>, 6>{{
        {0, 0, {0, 0}},
        {0xFFFF'FFFF'FFFF'FBFAull, 0x1FA, {0xFA, 1}},
      }}
    );
}

TEST(medium_int, basic_u8_8_signed) {
    test_tables<u8, 8>(
      STOWN_PTR(u8, 8),  //
      std::array<std::tuple<i8, u64, std::array<u8, 1>>, 3>{{
        {0, 0, {0}},
        {-1, 0xFF, {0xFF}},
      }},
      std::array<std::tuple<i32, u64, std::array<u8, 1>>, 5>{{
        {0, 0, {0}},
        {-1, 0xFF, {0xFF}},
        {-2, 0xFE, {0xFE}},
        {-128, 0x80, {0x80}},
      }}
    );
}

TEST(medium_int, basic_u8_7_signed) {
    test_tables<u8, 7>(
      STOWN_PTR(u8, 7),  //
      std::array<std::tuple<i8, u64, std::array<u8, 1>>, 3>{{
        {0, 0, {0}},
        {-1, 0x7F, {0x7F}},
      }},
      std::array<std::tuple<i32, u64, std::array<u8, 1>>, 5>{{
        {0, 0, {0}},
        {-1, 0x7F, {0x7F}},
        {-2, 0x7E, {0x7E}},
        {-128, 0x00, {0x00}},
        {-64, 0x40, {0x40}},
      }}
    );
}
