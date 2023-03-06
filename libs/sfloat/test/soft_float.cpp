#include <stuff/sfloat.hpp>

#include <gtest/gtest.h>

#include <span>

using namespace stf::integers;

namespace {

template<std::unsigned_integral T>
struct test_entry {
    using result_pair = std::pair<T, stf::sfloat::classification>;

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
    constexpr auto _name(T v, stf::sfloat::classification cls) const->test_entry { return _name({v, cls}); }

    FACTORY_FACTORY(ceil);
    FACTORY_FACTORY(floor);
    FACTORY_FACTORY(trunc);
    FACTORY_FACTORY(round);
};

static std::span<const test_entry<u32>> get_sp_test_entries() {
    using stf::sfloat::classification;

    auto nanc = classification::nan;
    auto infc = classification::infinite;
    auto zeroc = classification::zero;
    auto snormc = classification::subnormal;
    auto normc = classification::normal;

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
    static test_entry<u32> float_tests[]{
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

    return float_tests;
}

}  // namespace

template<typename Type, std::unsigned_integral Repr>
static void test_soft_float_rounding(std::span<const test_entry<Repr>> tests) {
    using stf::sfloat::classification;

    auto check = [](auto result, typename test_entry<Repr>::result_pair expected, size_t i) {
        classification res_class = classify(result);

        ASSERT_EQ(res_class, expected.second) << "test #" << i;

        if (expected.second == classification::nan) {
            return;
        }

        if (expected.second == classification::infinite) {
            ASSERT_EQ(result.sign_bit(), Type(expected.first).sign_bit()) << "test #" << i;
            return;
        }

        ASSERT_EQ(result.get_repr(), expected.first) << "test #" << i;
    };

    for (size_t i = 0; auto const& test_case : tests) {
        auto orig_parts = Type(test_case.original.first);
        ASSERT_EQ(classify(orig_parts), test_case.original.second) << "test #" << i;

        check(ceil(auto{orig_parts}), test_case.ceil_result, i);
        check(floor(auto{orig_parts}), test_case.floor_result, i);
        check(trunc(auto{orig_parts}), test_case.trunc_result, i);
        check(round(auto{orig_parts}), test_case.round_result, i);

        ++i;
    }
}

TEST(sfloat, rounding) {
    test_soft_float_rounding<stf::sfloat::native_float_t<float>>(get_sp_test_entries());
}

TEST(sfloat, nextafter) {
    using stf::sfloat::exception;
    using stf::sfloat::make_soft;
    using stf::sfloat::ieee::nextafter;
    using float_type = stf::sfloat::native_float_t<float>;
    using U = typename float_type::repr_type;

    constexpr U min_subnormal = 1;
    constexpr U neg_min_subnormal = static_cast<U>(1) << (float_type::exponent_bits + float_type::mantissa_bits) | 1;

    stf::sfloat::fenv env {};

    ASSERT_EQ(nextafter(make_soft(0.f), make_soft(1.f), env).repr, min_subnormal);
    ASSERT_TRUE(env.test_except(exception::inexact));
    ASSERT_TRUE(env.test_except(exception::underflow));
    env.clear_except(exception::inexact);
    env.clear_except(exception::underflow);
    ASSERT_FALSE(env.test_except(exception::all));

    ASSERT_EQ(nextafter(make_soft(0.f), make_soft(-1.f), env).repr, neg_min_subnormal);
    ASSERT_TRUE(env.test_except(exception::inexact));
    ASSERT_TRUE(env.test_except(exception::underflow));
    env.clear_except(exception::inexact);
    env.clear_except(exception::underflow);
    ASSERT_FALSE(env.test_except(exception::all));

    ASSERT_EQ(nextafter(make_soft(1.f), make_soft(2.f), env), make_soft(1.00000011921f));
    ASSERT_FALSE(env.test_except(exception::all));

    ASSERT_EQ(nextafter(make_soft(1.f), make_soft(-2.f), env), make_soft(0.999999940395f));
    ASSERT_FALSE(env.test_except(exception::all));

    ASSERT_EQ(nextafter(make_soft(1.f), make_soft(2.f), env).repr, 0x3f800001);
    ASSERT_FALSE(env.test_except(exception::all));

    ASSERT_EQ(nextafter(make_soft(1.f), make_soft(-2.f), env).repr, 0x3f7fffff);
    ASSERT_FALSE(env.test_except(exception::all));
}
