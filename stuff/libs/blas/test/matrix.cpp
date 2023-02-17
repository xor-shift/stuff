#include <stuff/blas/matrix.hpp>

#include <gtest/gtest.h>

using namespace stf::integers;

template<template<typename T, usize Rows, usize Cols> class matrix>
    requires(stf::blas::concepts::matrix_backend<matrix<int, 2, 4>>)
void basic_test_matrix_type() {
    using stf::blas::vector;

    matrix<int, 2, 4> mat_0{0, 1, 2, 3, 4, 5, 6, 7};
    matrix<int, 4, 2> mat_1{8, 9, 10, 11, 12, 13, 14, 15};

    ASSERT_EQ(mat_0[0], (vector<int, 4>{0, 1, 2, 3}));
    ASSERT_EQ(mat_0.column(0), (vector<int, 2>{0, 4}));
    ASSERT_EQ(mat_1[0], (vector<int, 2>{8, 9}));
    ASSERT_EQ(mat_1.column(0), (vector<int, 4>{8, 10, 12, 14}));

    ASSERT_EQ(mat_0 <=> mat_1, std::partial_ordering::unordered);
    ASSERT_EQ(mat_0 <=> mat_1, std::partial_ordering::unordered);

    matrix<int, 2, 4> mat_2{8, 9, 10, 11, 12, 13, 14, 15};

    ASSERT_EQ(mat_0 <=> mat_2, std::partial_ordering::less);
    ASSERT_EQ(mat_2 <=> mat_0, std::partial_ordering::greater);

    ASSERT_EQ(-mat_0, (matrix<int, 2, 4>{0, -1, -2, -3, -4, -5, -6, -7}));
    ASSERT_EQ(-mat_1, (matrix<int, 4, 2>{-8, -9, -10, -11, -12, -13, -14, -15}));

    stf::blas::concepts::nd_matrix_of_t<int, 2, 2> auto mat_01 = mat_0 * mat_1;
    matrix<int, 2, 2> expected_mat_01{76, 82, 252, 274};
    ASSERT_EQ(mat_01, expected_mat_01);
}

TEST(blas, matrix_basic) {
    basic_test_matrix_type<stf::blas::matrix>();
}

template<template<typename T, usize Rows, usize Cols> class matrix>
    requires(stf::blas::concepts::matrix_backend<matrix<int, 2, 4>>)
struct vector_matrix_tester {
    template<typename T, usize Size>
    using vector = typename matrix<int, 1, 1>::template preferred_vector_type<T, Size>;

    static void test() {
        namespace concepts = stf::blas::concepts;

        matrix<int, 3, 3> identity{1, 0, 0, 0, 1, 0, 0, 0, 1};
        vector<int, 3> vec_0{1, 2, 3};
        concepts::nd_vector<3> auto res_0 = identity * vec_0;

        ASSERT_EQ(vec_0, res_0);
        ASSERT_FALSE((requires { matrix<int, 3, 4>{} * vector<int, 3>{}; }));
    }
};

TEST(blas, matrix_vector) {
    vector_matrix_tester<stf::blas::matrix>::test();
}
