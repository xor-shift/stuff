#include <gtest/gtest.h>

#define STF_INTRO_RUN_CT_TESTS 1

#include <stuff/intro/introspectors/function.hpp>

#include <stuff/intro.hpp>

TEST(intro, overloadless_function) {
    void _definitely_not_defined_0(void);
    void _definitely_not_defined_1(int);
    int _definitely_not_defined_2(void);
    int _definitely_not_defined_3(int);
    int _definitely_not_defined_4(int, float, char);
    int (*_definitely_not_defined_5())(float);

    ASSERT_EQ(stf::intro::function_introspector<decltype(_definitely_not_defined_0)>::arg_count, 0);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_0)>::return_type, void>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_0)>::nth_argument<0>, void>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(_definitely_not_defined_1)>::arg_count, 1);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_1)>::return_type, void>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_1)>::nth_argument<0>, int>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(_definitely_not_defined_2)>::arg_count, 0);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_2)>::return_type, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_2)>::nth_argument<0>, void>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(_definitely_not_defined_3)>::arg_count, 1);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_3)>::return_type, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_3)>::nth_argument<0>, int>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(_definitely_not_defined_4)>::arg_count, 3);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_4)>::return_type, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_4)>::nth_argument<0>, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_4)>::nth_argument<1>, float>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_4)>::nth_argument<2>, char>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(_definitely_not_defined_5)>::arg_count, 0);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_5)>::return_type, int (*)(float)>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(_definitely_not_defined_5)>::nth_argument<0>, void>));
}

TEST(intro, overloadless_call_operator) {
    auto lambda_0 = [] {};
    auto lambda_1 = [](int) {};
    auto lambda_2 = []() { return 1; };
    auto lambda_3 = [](int) { return 1; };
    auto lambda_4 = [](int, float, char) { return 1; };
    auto lambda_5 = []() { return static_cast<int (*)(float)>(nullptr); };

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_0)>::arg_count, 0);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_0)>::return_type, void>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_0)>::nth_argument<0>, void>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_1)>::arg_count, 1);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_1)>::return_type, void>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_1)>::nth_argument<0>, int>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_2)>::arg_count, 0);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_2)>::return_type, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_2)>::nth_argument<0>, void>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_3)>::arg_count, 1);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_3)>::return_type, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_3)>::nth_argument<0>, int>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_4)>::arg_count, 3);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_4)>::return_type, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_4)>::nth_argument<0>, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_4)>::nth_argument<1>, float>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_4)>::nth_argument<2>, char>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_5)>::arg_count, 0);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_5)>::return_type, int (*)(float)>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_5)>::nth_argument<0>, void>));
}

TEST(intro, overloadless_static_call_operator) {
    struct { static auto operator()() {} } lambda_0 {};
    struct { static auto operator()(int) {} } lambda_1 {};
    struct { static auto operator()() { return 1; } } lambda_2 {};
    struct { static auto operator()(int) { return 1; } } lambda_3 {};
    struct { static auto operator()(int, float, char) { return 1; } } lambda_4 {};
    struct { static auto operator()() { return static_cast<int (*)(float)>(nullptr); } } lambda_5 {};

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_0)>::arg_count, 0);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_0)>::return_type, void>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_0)>::nth_argument<0>, void>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_1)>::arg_count, 1);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_1)>::return_type, void>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_1)>::nth_argument<0>, int>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_2)>::arg_count, 0);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_2)>::return_type, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_2)>::nth_argument<0>, void>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_3)>::arg_count, 1);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_3)>::return_type, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_3)>::nth_argument<0>, int>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_4)>::arg_count, 3);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_4)>::return_type, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_4)>::nth_argument<0>, int>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_4)>::nth_argument<1>, float>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_4)>::nth_argument<2>, char>));

    ASSERT_EQ(stf::intro::function_introspector<decltype(lambda_5)>::arg_count, 0);
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_5)>::return_type, int (*)(float)>));
    ASSERT_TRUE((std::is_same_v<stf::intro::function_introspector<decltype(lambda_5)>::nth_argument<0>, void>));
}
