#include <stuff/scope/unique_resource.hpp>

#include <gtest/gtest.h>

TEST(unique_resource, normal_exit) {
    int dummy_resource = 123;

    stf::detail::simple_guard tester{[&] noexcept { ASSERT_EQ(dummy_resource, 0); }};

    auto deleter = [&, i = 0](int* v) mutable {
        ASSERT_EQ(v, &dummy_resource);
        *v = 0;
    };

    stf::unique_resource resource{&dummy_resource, deleter};
    ASSERT_EQ(dummy_resource, 123);
}
