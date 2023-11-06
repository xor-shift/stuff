#include <stuff/medium_int.hpp>

#include <gtest/gtest.h>

namespace {

template<std::unsigned_integral ReprType, usize Bits, std::ranges::range TestRange, typename ValueType = std::ranges::range_value_t<TestRange>>
void test_roundtrip(usize test_no, TestRange test_values) {
    for (auto [i, val] : test_values | std::views::enumerate) {
        auto medium_int = stf::medium_int<Bits, ReprType>{};

        if constexpr (std::integral<ValueType>) {
            medium_int.set(val);
            const std::integral auto got = medium_int.template get<ValueType>();
            ASSERT_EQ(val, got) << "table #" << test_no << ", row #" << i;
        } else if constexpr (requires { typename std::tuple_size<ValueType>; }) {
            auto [value_to_set, expected_value] = val;
            using actual_value_type = decltype(expected_value);

            medium_int.set(value_to_set);
            const std::integral auto got = medium_int.template get<actual_value_type>();

            ASSERT_EQ(expected_value, got) << "table #" << test_no << ", row #" << i;
        } else {
            std::unreachable();
        }
    }
}

}  // namespace

TEST(medium_int, roundtrip_u8_modular_table) {
    test_roundtrip<u8, 7>(0, (std::array<u16, 3>){0, 1, 0x7F});
    test_roundtrip<u8, 8>(1, (std::array<u16, 3>){0, 1, 0xFF});
    test_roundtrip<u8, 9>(2, (std::array<u16, 3>){0, 1, 0x1FF});
    test_roundtrip<u8, 16>(3, (std::array<u16, 3>){0, 1, 0xFFFF});
    test_roundtrip<u8, 17>(4, (std::array<u32, 3>){0, 1, 0x1FFFF});

    test_roundtrip<u8, 17>(
      5, (std::array<std::pair<u32, u16>, 4>){{
           {0, 0},
           {1, 1},
           {0x1FFFF, 0xFFFF},
           {0x2FFFF, 0xFFFF},
         }}
    );

    test_roundtrip<u8, 17>(
      6, (std::array<std::pair<u32, u32>, 5>){{
           {0, 0},
           {1, 1},
           {0x1FFFF, 0x1FFFF},
           {0x2FFFF, 0xFFFF},
           {0x3FFFF, 0x1FFFF},
         }}
    );

    test_roundtrip<u8, 16>(
      7, (std::array<std::pair<u16, i16>, 4>){{
           {0, 0},
           {1, 1},
           {0xFFFF, -1},
           {0x8000, -0x8000},
         }}
    );

    test_roundtrip<u8, 16>(8, (std::array<i16, 4>){{0, -1, -2, -0x8000}});
    test_roundtrip<u8, 16>(9, (std::array<i32, 4>){{0, -1, -2, -0x8000}});
}
