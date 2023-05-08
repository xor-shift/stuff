#include <stuff/intro/introspectors/array.hpp>
#include <stuff/intro/introspectors/intro_builder.hpp>

#include <stuff/serde.hpp>

#include <gtest/gtest.h>

struct bar_t {
    int a;
    float b;
    double c;
};

constexpr auto _stf_adl_introspector() { return stf::intro::intro_builder<bar_t>::quick_build<"a", "b", "c">{}; }

TEST(serde, sandbox_0) {
    std::string str{};
    stf::serde::json_serializer serializer(back_inserter(str), std::unreachable_sentinel);

    stf::serde::serialize(serializer, true);
    ASSERT_EQ(str, "true");
    str.clear();

    stf::serde::serialize(serializer, -1234);
    ASSERT_EQ(str, "-1234");
    str.clear();

    stf::serde::serialize(serializer, 3.1415926f);
    ASSERT_EQ(str, "3.1415925");
    str.clear();

    stf::serde::serialize(serializer, 3.1415926);
    ASSERT_EQ(str, "3.1415926");
    str.clear();

    stf::serde::serialize(serializer, 'a');
    ASSERT_EQ(str, "'a'");
    str.clear();

    stf::serde::serialize(serializer, "Hello, world!");
    ASSERT_EQ(str, "\"Hello, world!\"");
    str.clear();

    stf::serde::concepts::seq_serializer auto seq_ser_0 = *serializer.serialize_seq(0uz);
    seq_ser_0.serialize_element('a');
    seq_ser_0.serialize_element('b');
    seq_ser_0.end();
    _stf_adl_serialize(serializer, 'c');
    ASSERT_EQ(str, "['a','b']'c'");
    str.clear();

    std::array<int, 3> arr{0, 1, 2};
    stf::serde::serialize(serializer, arr);
    ASSERT_EQ(str, "[0,1,2]");
    str.clear();

    struct foo_t {
        int a;
        float b;
        double c;
    } foo{
      .a = 0,
      .b = 3.1415926f,
      .c = 3.1415926,
    };

    stf::serde::serialize(serializer, foo);
    ASSERT_EQ(str, "[0,3.1415925,3.1415926]");
    str.clear();

    bar_t bar{
      .a = 0,
      .b = 3.1415926f,
      .c = 3.1415926,
    };

    stf::serde::serialize(serializer, bar);
    ASSERT_EQ(str, "[0,3.1415925,3.1415926]");
    str.clear();
}
