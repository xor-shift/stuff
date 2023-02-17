#include <stuff/cemath.hpp>

#include <gtest/gtest.h>

namespace {

template<std::unsigned_integral T>
struct test_entry {
    using result_pair = std::pair<T, stf::cemath::classification>;

    result_pair original;

    result_pair ceil_result;
    result_pair floor_result;
    result_pair trunc_result;
    result_pair round_result;

#define FACTORY_FACTORY(_name)                              \
    constexpr auto _name(result_pair v) const->test_entry { \
        auto copy = *this;                                  \
        copy._name##_result = v;                            \
        return copy;                                        \
    }                                                       \
                                                            \
    constexpr auto _name(T v, stf::cemath::classification cls) const->test_entry { return _name({v, cls}); }

    FACTORY_FACTORY(ceil);
    FACTORY_FACTORY(floor);
    FACTORY_FACTORY(trunc);
    FACTORY_FACTORY(round);
};

}  // namespace

template<typename Type, std::unsigned_integral Repr, size_t N>
static void test_entries(test_entry<Repr> const (&tests)[N]) {
    using stf::cemath::classification;

    auto check = [](auto result, typename test_entry<Repr>::result_pair expected, size_t i) {
        classification res_class = result.classify();

        ASSERT_EQ(res_class, expected.second) << "test #" << i;

        if (expected.second == classification::nan) {
            return;
        }

        if (expected.second == classification::infinite) {
            ASSERT_EQ(result.sign_bit(), Type::from_repr(expected.first).sign_bit()) << "test #" << i;
            return;
        }

        ASSERT_EQ(result.get_repr(), expected.first) << "test #" << i;
    };

    for (size_t i = 0; auto const& test_case : tests) {
        auto orig_parts = Type::from_repr(test_case.original.first);
        ASSERT_EQ(orig_parts.classify(), test_case.original.second) << "test #" << i;

        check(auto{orig_parts}.ceil(), test_case.ceil_result, i);
        check(auto{orig_parts}.floor(), test_case.floor_result, i);
        check(auto{orig_parts}.trunc(), test_case.trunc_result, i);
        check(auto{orig_parts}.round(), test_case.round_result, i);

        ++i;
    }
}

TEST(cemath, soft_float) {
    using stf::cemath::classification;

    auto infc = classification::infinite;
    auto nanc = classification::nan;
    auto normc = classification::normal;
    auto snormc = classification::subnormal;
    auto zeroc = classification::zero;

    using stf::u32;

    // common values
    test_entry<u32>::result_pair zero{0x00000000, zeroc};         //  0
    test_entry<u32>::result_pair succ_zero{0x00000001, snormc};   //  0.00..01
    test_entry<u32>::result_pair pred_mzero{0x80000001, snormc};  // -0.00..01
    test_entry<u32>::result_pair mzero{0x80000000, zeroc};        // -0

    test_entry<u32>::result_pair pred_one{0x3f7fffff, normc};   //  0.99..99
    test_entry<u32>::result_pair one{0x3f800000, normc};        //  1
    test_entry<u32>::result_pair succ_one{0x3f800001, normc};   //  1.00..01
    test_entry<u32>::result_pair pred_mone{0xbf800001, normc};  // -1.00..01
    test_entry<u32>::result_pair mone{0xbf800000, normc};       // -1
    test_entry<u32>::result_pair succ_mone{0xbf7fffff, normc};  // -0.99..99

    test_entry<u32>::result_pair pred_two{0x3fffffff, normc};   //  1.99..99
    test_entry<u32>::result_pair two{0x40000000, normc};        //  2
    test_entry<u32>::result_pair succ_two{0x40000001, normc};   //  2.00..01
    test_entry<u32>::result_pair pred_mtwo{0xc0000001, normc};  // -2.00..01
    test_entry<u32>::result_pair mtwo{0xc0000000, normc};       // -2
    test_entry<u32>::result_pair succ_mtwo{0xbfffffff, normc};  // -1.99..99

    test_entry<u32>::result_pair nan{-1, nanc};
    test_entry<u32>::result_pair inf{0x7f800000, infc};
    test_entry<u32>::result_pair minf{0xff800000, infc};

    // orig, ceil, floor, trunc, round
    test_entry<u32> float_tests[]{
      {zero, zero, zero, zero, zero},
      {succ_zero, one, zero, zero, zero},
      {mzero, mzero, mzero, mzero, mzero},
      {pred_mzero, mzero, mone, mzero, mzero},

      {inf, inf, inf, inf, inf},
      {minf, minf, minf, minf, minf},

      {{0x7f812345, nanc}, nan, nan, nan, nan},
      {{0x7f800001, nanc}, nan, nan, nan, nan},
      {{0x7f900000, nanc}, nan, nan, nan, nan},
      {{0x7ff00000, nanc}, nan, nan, nan, nan},
      {{0x7fffffff, nanc}, nan, nan, nan, nan},
      {{0xff812345, nanc}, nan, nan, nan, nan},
      {{0xff800001, nanc}, nan, nan, nan, nan},
      {{0xff900000, nanc}, nan, nan, nan, nan},
      {{0xffffffff, nanc}, nan, nan, nan, nan},
      {{0xffffffff, nanc}, nan, nan, nan, nan},

      {succ_one, two, one, one, one},
      {pred_one, one, zero, zero, one},
      {pred_mone, {0xbf800000, normc}, {0xc0000000, normc}, {0xbf800000, normc}, {0xbf800000, normc}},
      {succ_mone, {0x80000000, zeroc}, {0xbf800000, normc}, {0x80000000, zeroc}, {0xbf800000, normc}},

      {{0x3effffff, normc}, one, zero, zero, zero},  // 0.49.99
      {{0x3f000000, normc}, one, zero, zero, one},   // 0.5
      {{0x3f000001, normc}, one, zero, zero, one},   // 0.50..01

      {{0xbeffffff, normc}, mzero, mone, mzero, mzero},  // -0.49.99
      {{0xbf000000, normc}, mzero, mone, mzero, mone},   // -0.5
      {{0xbf000001, normc}, mzero, mone, mzero, mone},   // -0.50..01

      {{0x3fbfffff, normc}, two, one, one, one},  // 1.49.99
      {{0x3fc00000, normc}, two, one, one, two},  // 1.5
      {{0x3fc00001, normc}, two, one, one, two},  // 1.50..01

      {{0xbfbfffff, normc}, mone, mtwo, mone, mone},  // -1.49.99
      {{0xbfc00000, normc}, mone, mtwo, mone, mtwo},  // -1.5
      {{0xbfc00001, normc}, mone, mtwo, mone, mtwo},  // -1.50..01
    };

    test_entries<stf::cemath::native_float_t<float>>(float_tests);
}
