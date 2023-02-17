#include <stuff/cemath.hpp>

#include <gtest/gtest.h>

TEST(cemath, abs) {
    using stf::cemath::detail::abs;

    ASSERT_EQ(abs(-1), 1);
    ASSERT_EQ(abs(1), 1);
    ASSERT_EQ(abs(-1l), 1l);
    ASSERT_EQ(abs(1l), 1l);
    ASSERT_EQ(abs(-1ll), 1ll);
    ASSERT_EQ(abs(1ll), 1ll);

    ASSERT_FLOAT_EQ(abs(-1.f), 1.f);
    ASSERT_FLOAT_EQ(abs(1.f), 1.f);
    ASSERT_FLOAT_EQ(abs(-0.f), 0.f);
    ASSERT_FLOAT_EQ(abs(0.f), 0.f);

    ASSERT_FLOAT_EQ(abs(-1.), 1.);
    ASSERT_FLOAT_EQ(abs(1.), 1.);
    ASSERT_FLOAT_EQ(abs(-0.), 0.);
    ASSERT_FLOAT_EQ(abs(0.), 0.);

    ASSERT_FLOAT_EQ(abs(-1.l), 1.l);
    ASSERT_FLOAT_EQ(abs(1.l), 1.l);
    ASSERT_FLOAT_EQ(abs(-0.l), 0.l);
    ASSERT_FLOAT_EQ(abs(0.l), 0.l);
}
