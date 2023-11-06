#include <stuff/core/stow.hpp>

#include <gtest/gtest.h>

namespace {

struct foo {
    constexpr auto get_hidden() const -> int { return hidden; }

private:
    int hidden = 42;
};

}

template struct stf::detail::stow_private<foo, &foo::hidden, "cool identifier">;

TEST(stf_core, stow) {
    auto bar = foo{};
    ASSERT_EQ(bar.get_hidden(), 42);

    (bar.*stf::detail::get_stown_member_ptr<foo, int foo::*, "cool identifier">::m_ptr) = 1337;
    ASSERT_EQ(bar.get_hidden(), 1337);
}
