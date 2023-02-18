#pragma once

#include <stuff/cemath.hpp>

#include <gtest/gtest.h>

#define SASSERT_EQ(val0, val1)       \
    static_assert((val0) == (val1)); \
    ASSERT_EQ((val0), (val1))

#define SASSERT_TRUE(val) \
    static_assert(val);   \
    ASSERT_TRUE(val)

#define SASSERT_FALSE(val) \
    static_assert(!(val)); \
    ASSERT_FALSE(val)
