#include <stuff/expected.hpp>

#include <gtest/gtest.h>

TEST(expected, unexpected) {}

struct nontrivial_settings {
    bool noexcept_constructor = false;
    bool noexcept_destructor = false;
};

template<nontrivial_settings Settings>
struct nontrivial {
    nontrivial() noexcept(Settings.noexcept_constructor) {}
    ~nontrivial() noexcept(Settings.noexcept_destructor) {}

    nontrivial(nontrivial&& other)
        : move_constructed(true) {
        other.moved = true;
    }

    nontrivial(nontrivial const& other)
        : copy_constructed(true) {
        other.copied = true;
    }

    auto operator=(nontrivial&& other) -> nontrivial& {
        other.moved_in_assignment = true;
        move_assigned = true;
        return *this;
    }

    auto operator=(nontrivial const& other) -> nontrivial& {
        other.copied_in_assignment = true;
        copy_assigned = true;
        return *this;
    }

    bool copy_constructed = false;
    bool copy_assigned = false;
    bool move_constructed = false;
    bool move_assigned = false;
    mutable bool copied = false;
    mutable bool copied_in_assignment = false;
    bool moved = false;
    bool moved_in_assignment = false;
};

struct no_default_constructor {
    no_default_constructor() = delete;
    no_default_constructor(int) {}
};

TEST(expected, expected) {
    using stf::expected;
    using stf::unexpected;

    expected<int, float> exp_0{};
    ASSERT_TRUE(noexcept(decltype(exp_0){}));
    ASSERT_TRUE(exp_0.has_value());
    ASSERT_NO_THROW(exp_0.value());
    ASSERT_EQ(*exp_0, 0);

    expected<int, float> exp_0_copy{exp_0};
    ASSERT_TRUE(exp_0_copy.has_value());
    ASSERT_NO_THROW(exp_0_copy.value());
    ASSERT_EQ(*exp_0_copy, 0);

    expected<int, float> exp_0_move{exp_0};
    ASSERT_TRUE(exp_0_move.has_value());
    ASSERT_NO_THROW(exp_0_move.value());
    ASSERT_EQ(*exp_0_move, 0);

    expected<nontrivial<nontrivial_settings{false, false}>, float> exp_1{};
    ASSERT_FALSE(noexcept(decltype(exp_1){}));
    ASSERT_TRUE(exp_1.has_value());
    ASSERT_NO_THROW(exp_1.value());

    expected<nontrivial<nontrivial_settings{true, true}>, float> exp_2{};
    ASSERT_TRUE(noexcept(decltype(exp_2){}));
    ASSERT_TRUE(exp_2.has_value());
    ASSERT_NO_THROW(exp_2.value());

    ASSERT_FALSE(exp_1->copied);
    expected exp_1_copy(exp_1);
    ASSERT_TRUE(exp_1->copied);
    ASSERT_TRUE(exp_1_copy->copy_constructed);

    ASSERT_FALSE(exp_1->moved);
    expected exp_1_move(std::move(exp_1));
    ASSERT_TRUE(exp_1->moved);
    ASSERT_TRUE(exp_1_move->move_constructed);

    ASSERT_FALSE(exp_1->copied_in_assignment);
    decltype(exp_1) exp_1_copy_assign{};
    exp_1_copy_assign = exp_1;
    ASSERT_TRUE(exp_1->copied_in_assignment);
    ASSERT_TRUE(exp_1_copy_assign->copy_assigned);

    ASSERT_FALSE(exp_1->moved_in_assignment);
    decltype(exp_1) exp_1_move_assign{};
    exp_1_move_assign = std::move(exp_1);
    ASSERT_TRUE(exp_1->moved_in_assignment);
    ASSERT_TRUE(exp_1_move_assign->move_assigned);

    expected<char, float> exp_3{};
    ASSERT_EQ(exp_0, 0);
    ASSERT_EQ(exp_0, exp_3);

    expected<char, float> exp_4{unexpected{4}};
    ASSERT_NE(exp_4, 0);
    ASSERT_NE(exp_4, exp_3);
    ASSERT_THROW(exp_4.value(), stf::bad_expected_access<float>);
    ASSERT_EQ(exp_4.error(), 4);

    expected<no_default_constructor, float> exp_5{unexpected{5}};
    expected<no_default_constructor, float> exp_6{6};
}

TEST(expected, expected_void_t) {
    using stf::expected;
    using stf::unexpected;

    expected<void, int> exp_0{};
    ASSERT_TRUE(exp_0.has_value());
    ASSERT_NO_THROW(exp_0.value());

    expected<void, int> exp_1{unexpected{5}};
    ASSERT_FALSE(exp_1.has_value());
    ASSERT_THROW(exp_1.value(), stf::bad_expected_access<int>);
    ASSERT_EQ(exp_1.error(), 5);
}

TEST(expected, tryx) {
    using stf::expected;
    using stf::unexpected;

    auto fn_0 = [](int i) -> expected<int, std::string_view> {
        if (i % 2) {
            return i * 2;
        }

        return unexpected{"aeiou"};
    };

    auto fn_1 = [&fn_0](int from, int to, int stride) -> expected<int, std::string_view> {
        int sum = 0;
        for (int i = from; i < to; i += stride) {
            sum += TRYX(fn_0(i));
        }
        return sum;
    };

    auto res_0 = fn_1(0, 10, 2);
    ASSERT_FALSE(res_0);
    ASSERT_EQ(res_0.error(), "aeiou");

    auto res_1 = fn_1(1, 10, 2);
    ASSERT_TRUE(res_1);
    ASSERT_EQ(res_1.value(), 1 * 2 + 3 * 2 + 5 * 2 + 7 * 2 + 9 * 2);
}
