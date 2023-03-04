#include <stuff/scope/scope_guard.hpp>

#include <gtest/gtest.h>

TEST(scope_guard, normal_exit) {
    bool exit_test = false;
    bool success_test = false;
    bool fail_test = false;

    stf::scope::detail::simple_guard actual_test{[&] noexcept {
        ASSERT_TRUE(exit_test);
        ASSERT_TRUE(success_test);
        ASSERT_FALSE(fail_test);
    }};

    stf::scope::scope_exit exit_guard{[&] { exit_test = true; }};
    stf::scope::scope_success success_guard{[&] { success_test = true; }};
    stf::scope::scope_fail fail_guard{[&] { fail_test = true; }};

    constexpr stf::scope::scope_exit constexpr_test{[] {}};
}

TEST(scope_guard, unwinding) try {
    bool exit_test = false;
    bool success_test = false;
    bool fail_test = false;

    stf::scope::detail::simple_guard actual_test{[&] noexcept {
        ASSERT_TRUE(exit_test);
        ASSERT_FALSE(success_test);
        ASSERT_TRUE(fail_test);
    }};

    stf::scope::scope_exit exit_guard{[&] { exit_test = true; }};
    stf::scope::scope_success success_guard{[&] { success_test = true; }};
    stf::scope::scope_fail fail_guard{[&] { fail_test = true; }};

    throw 0;
} catch (...) {
}

TEST(scope_guard, moved_guard) {
    int exit_test = 0;
    int success_test = 0;
    int fail_test = 0;

    stf::scope::detail::simple_guard actual_test{[&] noexcept {
        ASSERT_EQ(exit_test, 1);
        ASSERT_EQ(success_test, 1);
        ASSERT_EQ(fail_test, 0);
    }};

    stf::scope::scope_exit exit_guard_0{[&] { exit_test += 1; }};
    stf::scope::scope_success success_guard_0{[&] { success_test += 1; }};
    stf::scope::scope_fail fail_guard_0{[&] { fail_test += 1; }};

    stf::scope::scope_exit exit_guard_1{std::move(exit_guard_0)};
    stf::scope::scope_success success_guard_1{std::move(success_guard_0)};
    stf::scope::scope_fail fail_guard_1{std::move(fail_guard_0)};
}
