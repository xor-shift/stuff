#include <stuff/blas.hpp>

#include <gtest/gtest.h>

template<template<typename T, usize Size> class vector>
void test_vector_type() {
    vector<int, 3> v_0{0, 1, 2};
    vector<int, 3> v_1{3, 4, 5};

    ASSERT_EQ(v_0[0], 0);
    ASSERT_EQ(v_0[1], 1);
    ASSERT_EQ(v_0[2], 2);

    ASSERT_EQ((v_0 + v_1)[0], 3);
    ASSERT_EQ((v_0 + v_1)[1], 5);
    ASSERT_EQ((v_0 + v_1)[2], 7);

    ASSERT_EQ((v_0 * 3)[2], 6);

    /*ASSERT_EQ(dot(v_0, v_1), 14);
    ASSERT_EQ(v_0 <=> v_1, std::partial_ordering::less);
    ASSERT_EQ(v_1 <=> v_0, std::partial_ordering::greater);
    ASSERT_EQ((v_0 + v_1 <=> vector<int, 3>{3, 5, 7}), std::partial_ordering::equivalent);
    ASSERT_EQ((v_0 - v_1 <=> vector<int, 3>{-3, -3, -3}), std::partial_ordering::equivalent);
    ASSERT_EQ((v_0 - v_1 <=> vector<int, 3>{-3, -3, -2}), std::partial_ordering::less);

    ASSERT_EQ(-v_0, (vector<int, 3>{0, -1, -2}));
    ASSERT_EQ(-v_1, (vector<int, 3>{-3, -4, -5}));

    ASSERT_EQ(swizzle<"x">(v_1), (vector<int, 1>{3}));
    ASSERT_EQ(swizzle<"y">(v_1), (vector<int, 1>{4}));
    ASSERT_EQ(swizzle<"z">(v_1), (vector<int, 1>{5}));
    ASSERT_EQ(swizzle<"xx">(v_1), (vector<int, 2>{3, 3}));
    ASSERT_EQ(swizzle<"xz">(v_1), (vector<int, 2>{3, 5}));
    ASSERT_EQ(swizzle<"yz">(v_1), (vector<int, 2>{4, 5}));
    ASSERT_EQ(swizzle<"zx">(v_1), (vector<int, 2>{5, 3}));
    ASSERT_EQ(swizzle<"xxy">(v_1), (vector<int, 3>{3, 3, 4}));
    ASSERT_EQ(swizzle<"zyz">(v_1), (vector<int, 3>{5, 4, 5}));
    ASSERT_EQ(swizzle<"zzx">(v_1), (vector<int, 3>{5, 5, 3}));
    ASSERT_EQ(swizzle<"zzz">(v_1), (vector<int, 3>{5, 5, 5}));*/
}

TEST(blas, vector_basic) {
    test_vector_type<stf::blas::vector>();
}
