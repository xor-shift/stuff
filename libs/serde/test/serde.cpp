#include <gtest/gtest.h>

#include <stuff/intro/introspectors/array.hpp>
#include <stuff/intro/introspectors/intro_builder.hpp>
#include <stuff/intro/introspectors/map.hpp>
#include <stuff/intro/introspectors/set.hpp>
#include <stuff/intro/introspectors/variant.hpp>

#include <stuff/intro.hpp>

#include <stuff/serde/serializables/basic.hpp>
#include <stuff/serde/serializables/intro.hpp>
#include <stuff/serde/serializables/optional.hpp>
#include <stuff/serde/serializables/span.hpp>
#include <stuff/serde/serializables/string_view.hpp>

#include <stuff/serde.hpp>

struct bar_t {
    int a;
    float b;
    double c;
};

constexpr auto _stf_adl_introspector(bar_t const&) { return stf::intro::intro_builder<bar_t>::quick_build<"a", "b", "c">{}; }

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

    using namespace std::string_view_literals;

    stf::serde::serialize(serializer, "Hello, world!"sv);
    ASSERT_EQ(str, "\"Hello, world!\"");
    str.clear();

    stf::serde::concepts::seq_serializer auto seq_ser_0 = *serializer.serialize_seq(0uz);
    seq_ser_0.serialize_element('a');
    seq_ser_0.serialize_element('b');
    seq_ser_0.end();
    stf::serde::serialize(serializer, 'c');
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

    std::map<std::string_view, int> map_0{
      {"a", 0},
      {"b", 1},
      {"c", 2},
    };
    stf::serde::serialize(serializer, map_0);
    ASSERT_EQ(str, "{\"a\":0,\"b\":1,\"c\":2}");
    str.clear();

    bar_t bar{
      .a = 0,
      .b = 3.1415926f,
      .c = 3.1415926,
    };

    stf::serde::serialize(serializer, bar);
    ASSERT_EQ(str, "{\"a\":0,\"b\":3.1415925,\"c\":3.1415926}");
    str.clear();
}

struct full_test_inner_t {
    int a;
    int& b;
    int const& c;
    int&& d;
    int const&& e;
    int f[2];
    int (&g)[3];
    int* h;
    int* i;
};

constexpr auto _stf_adl_introspector(full_test_inner_t const&) {
    return stf::intro::intro_builder<full_test_inner_t>::quick_build<"a", "b", "c", "d", "e", "f", "g", "h", "i">{};  //
}

struct full_test_t {
    std::optional<full_test_inner_t> a[3];
    std::variant<int, float> b;
    std::tuple<int, double> c;
    std::map<std::string_view, int> d;
    // std::set<std::string_view> e;
};

constexpr auto _stf_adl_introspector(full_test_t const&) {
    return stf::intro::intro_builder<full_test_t>::quick_build<"a", "b", "c", "d">{};  //
}

TEST(serde, full_json) {
    int two = 2, three = 3, four = 4, five = 5, eleven = 11;
    int eight_nine_ten[3] = {8, 9, 10};

    full_test_inner_t inner{
      .a = 1,
      .b = two,
      .c = three,
      .d = std::move(four),
      .e = std::move(five),
      .f = {6, 7},
      .g = eight_nine_ten,
      .h = &eleven,
      .i = nullptr,
    };

    full_test_t full{
      .a = {std::move(inner), std::nullopt, std::nullopt},  //
      .b = 3.1415926f,
      .c = {12, 3.1415926},
      .d{{"thirteen", 13}, {"fourteen", 14}},
      //.e {"15", "16"},
    };

    std::string str{};
    stf::serde::json_serializer serializer(back_inserter(str), std::unreachable_sentinel);

    stf::serde::serialize(serializer, full);

    std::string_view expected{
      // clang-format off
      "{"
        "\"a\":["
          "{"
            "\"a\":1,"
            "\"b\":2,"
            "\"c\":3,"
            "\"d\":4,"
            "\"e\":5,"
            "\"f\":[6,7],"
            "\"g\":[8,9,10],"
            "\"h\":11,"
            "\"i\":undefined"
          "},"
          "null,"
          "null"
        "],"
        "\"b\":3.1415925,"
        "\"c\":[12,3.1415926],"
        "\"d\":{"
          "\"fourteen\":14,"
          "\"thirteen\":13"
        "}"
      "}"
      // clang-format on
    };

    ASSERT_EQ(str, expected);
}
