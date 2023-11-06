#include "./random.hpp"

#include <stuff/medium_int.hpp>
#include <stuff/random.hpp>

#include <gtest/gtest.h>

namespace {

template<typename ValueType, std::unsigned_integral ReprType, usize Bits = std::numeric_limits<std::make_unsigned_t<ValueType>>::digits>
void test_random_roundtrip(usize test_no) {
    auto rng_engine = stf::random::xoshiro_256pp{stf::tests::get_rng_seed()};
    auto rng_dist = std::uniform_int_distribution<ValueType>{std::numeric_limits<ValueType>::min(), std::numeric_limits<ValueType>::max()};

    for (const auto iteration : std::views::iota(0uz, 1024uz)) {
        auto test_integer = stf::medium_int<Bits, ReprType>{};

        const auto set_value = rng_dist(rng_engine);
        test_integer.set(set_value);
        const auto got_value = test_integer.template get<ValueType>();

        static constexpr auto unimportant_bits = Bits > std::numeric_limits<ValueType>::digits ? 0 : std::numeric_limits<ValueType>::digits - Bits;

        using intermediate_type = std::make_unsigned_t<ValueType>;

        if constexpr (std::unsigned_integral<ValueType>) {
            const auto expected_value = static_cast<intermediate_type>(set_value << unimportant_bits) >> unimportant_bits;
            ASSERT_EQ(expected_value, got_value) << "set_value: " << set_value    //
                                                 << ", test #" << test_no         //
                                                 << ", iteration #" << iteration  //
                                                 << " with seed: " << stf::tests::get_rng_seed();
        } else {
        }
    }
}

}  // namespace

TEST(medium_int, roundtrip_unsigned_random) {
    auto i = 0uz;

    test_random_roundtrip<u8, u8>(i++);
    test_random_roundtrip<u16, u8>(i++);
    test_random_roundtrip<u32, u8>(i++);
    test_random_roundtrip<u64, u8>(i++);

    test_random_roundtrip<u8, u16>(i++);
    test_random_roundtrip<u16, u16>(i++);
    test_random_roundtrip<u32, u16>(i++);
    test_random_roundtrip<u64, u16>(i++);

    test_random_roundtrip<u8, u32>(i++);
    test_random_roundtrip<u16, u32>(i++);
    test_random_roundtrip<u32, u32>(i++);
    test_random_roundtrip<u64, u32>(i++);

    test_random_roundtrip<u8, u64>(i++);
    test_random_roundtrip<u16, u64>(i++);
    test_random_roundtrip<u32, u64>(i++);
    test_random_roundtrip<u64, u64>(i++);

    test_random_roundtrip<u8, u8, 7>(i++);
    test_random_roundtrip<u16, u8, 7>(i++);
    test_random_roundtrip<u32, u8, 7>(i++);
    test_random_roundtrip<u64, u8, 7>(i++);
    test_random_roundtrip<u8, u8, 9>(i++);
    test_random_roundtrip<u16, u8, 9>(i++);
    test_random_roundtrip<u32, u8, 9>(i++);
    test_random_roundtrip<u64, u8, 9>(i++);

    test_random_roundtrip<u8, u16, 15>(i++);
    test_random_roundtrip<u16, u16, 15>(i++);
    test_random_roundtrip<u32, u16, 15>(i++);
    test_random_roundtrip<u64, u16, 15>(i++);
    test_random_roundtrip<u8, u16, 17>(i++);
    test_random_roundtrip<u16, u16, 17>(i++);
    test_random_roundtrip<u32, u16, 17>(i++);
    test_random_roundtrip<u64, u16, 17>(i++);

    test_random_roundtrip<u8, u32, 31>(i++);
    test_random_roundtrip<u16, u32, 31>(i++);
    test_random_roundtrip<u32, u32, 31>(i++);
    test_random_roundtrip<u64, u32, 31>(i++);
    test_random_roundtrip<u8, u32, 33>(i++);
    test_random_roundtrip<u16, u32, 33>(i++);
    test_random_roundtrip<u32, u32, 33>(i++);
    test_random_roundtrip<u64, u32, 33>(i++);

    test_random_roundtrip<u8, u64, 63>(i++);
    test_random_roundtrip<u16, u64, 63>(i++);
    test_random_roundtrip<u32, u64, 63>(i++);
    test_random_roundtrip<u64, u64, 63>(i++);
    test_random_roundtrip<u8, u64, 65>(i++);
    test_random_roundtrip<u16, u64, 65>(i++);
    test_random_roundtrip<u32, u64, 65>(i++);
    test_random_roundtrip<u64, u64, 65>(i++);

    static_cast<void>(i);
}

TEST(medium_int, roundtrip_signed_random) {
    auto i = 0uz;

    test_random_roundtrip<i8, u8>(i++);
    test_random_roundtrip<i16, u8>(i++);
    test_random_roundtrip<i32, u8>(i++);
    test_random_roundtrip<i64, u8>(i++);

    test_random_roundtrip<i8, u16>(i++);
    test_random_roundtrip<i16, u16>(i++);
    test_random_roundtrip<i32, u16>(i++);
    test_random_roundtrip<i64, u16>(i++);

    test_random_roundtrip<i8, u32>(i++);
    test_random_roundtrip<i16, u32>(i++);
    test_random_roundtrip<i32, u32>(i++);
    test_random_roundtrip<i64, u32>(i++);

    test_random_roundtrip<i8, u64>(i++);
    test_random_roundtrip<i16, u64>(i++);
    test_random_roundtrip<i32, u64>(i++);
    test_random_roundtrip<i64, u64>(i++);

    test_random_roundtrip<i8, u8, 7>(i++);
    test_random_roundtrip<i16, u8, 7>(i++);
    test_random_roundtrip<i32, u8, 7>(i++);
    test_random_roundtrip<i64, u8, 7>(i++);
    test_random_roundtrip<i8, u8, 9>(i++);
    test_random_roundtrip<i16, u8, 9>(i++);
    test_random_roundtrip<i32, u8, 9>(i++);
    test_random_roundtrip<i64, u8, 9>(i++);

    test_random_roundtrip<i8, u16, 15>(i++);
    test_random_roundtrip<i16, u16, 15>(i++);
    test_random_roundtrip<i32, u16, 15>(i++);
    test_random_roundtrip<i64, u16, 15>(i++);
    test_random_roundtrip<i8, u16, 17>(i++);
    test_random_roundtrip<i16, u16, 17>(i++);
    test_random_roundtrip<i32, u16, 17>(i++);
    test_random_roundtrip<i64, u16, 17>(i++);

    test_random_roundtrip<i8, u32, 31>(i++);
    test_random_roundtrip<i16, u32, 31>(i++);
    test_random_roundtrip<i32, u32, 31>(i++);
    test_random_roundtrip<i64, u32, 31>(i++);
    test_random_roundtrip<i8, u32, 33>(i++);
    test_random_roundtrip<i16, u32, 33>(i++);
    test_random_roundtrip<i32, u32, 33>(i++);
    test_random_roundtrip<i64, u32, 33>(i++);

    test_random_roundtrip<i8, u64, 63>(i++);
    test_random_roundtrip<i16, u64, 63>(i++);
    test_random_roundtrip<i32, u64, 63>(i++);
    test_random_roundtrip<i64, u64, 63>(i++);
    test_random_roundtrip<i8, u64, 65>(i++);
    test_random_roundtrip<i16, u64, 65>(i++);
    test_random_roundtrip<i32, u64, 65>(i++);
    test_random_roundtrip<i64, u64, 65>(i++);

    static_cast<void>(i);
}
