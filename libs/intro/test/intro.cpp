#include <stuff/intro/introspectors/intro_builder.hpp>

#include <gtest/gtest.h>

#include <stuff/intro.hpp>
#include <stuff/intro/detail/aggregate/member_types.hpp>
#include <stuff/intro/introspectors/array.hpp>
#include <stuff/intro/introspectors/span.hpp>
#include <stuff/intro/introspectors/tuple.hpp>
#include <stuff/intro/introspectors/unordered_map.hpp>

TEST(intro, basic_array) {
    std::array<int, 3> arr_0{1, 2, 3};
    using intro_0 = stf::intro::introspector_t<decltype(arr_0)>;

    int arr_1[3]{1, 2, 3};
    using intro_1 = stf::intro::introspector_t<decltype(arr_1)>;

    const int arr_2[3]{1, 2, 3};
    using intro_2 = stf::intro::introspector_t<decltype(arr_2)>;

    ASSERT_EQ(intro_0::size(arr_0), 3uz);
    ASSERT_EQ(intro_1::size(arr_1), 3uz);
    ASSERT_EQ(intro_2::size(arr_2), 3uz);

    ASSERT_EQ(intro_0::get<0>(arr_0), 1);
    ASSERT_EQ(intro_1::get<1>(arr_1), 2);
    ASSERT_EQ(intro_2::get<2>(arr_2), 3);

    intro_0::get<0>(arr_0) = 2;
    intro_1::get<1>(arr_1) = 3;

    ASSERT_EQ(intro_0::get<0>(arr_0), 2);
    ASSERT_EQ(intro_1::get<1>(arr_1), 3);
    ASSERT_EQ(intro_2::get<2>(arr_2), 3);

    int sum_0 = 0;
    for (auto it = intro_0::begin(arr_0); it != intro_0::end(arr_0); it++) {
        sum_0 += *it;
    }
    ASSERT_EQ(sum_0, 7);
}

TEST(intro, basic_span) {
    std::array<int, 3> arr_0{1, 2, 3};
    std::span<int> span_0{arr_0};
    using intro_0 = stf::intro::introspector_t<decltype(span_0)>;

    ASSERT_EQ(intro_0::size(span_0), 3);
    ASSERT_EQ(intro_0::index(span_0, 0), 1);
    ASSERT_EQ(intro_0::index(span_0, 1), 2);
    ASSERT_EQ(intro_0::index(span_0, 2), 3);

    int sum_0 = 0;
    for (auto it = intro_0::begin(span_0); it != intro_0::end(span_0); it++) {
        sum_0 += *it;
    }
    ASSERT_EQ(sum_0, 6);
}

TEST(intro, basic_umap) {
    std::unordered_map<std::string_view, int> umap_0{{"a", 1}, {"b", 2}, {"c", 3}};
    using intro_0 = stf::intro::introspector_t<decltype(umap_0)>;

    ASSERT_EQ(intro_0::size(umap_0), 3);
    ASSERT_EQ(intro_0::index(umap_0, "a"), 1);
    ASSERT_EQ(intro_0::index(umap_0, "b"), 2);
    ASSERT_EQ(intro_0::index(umap_0, "c"), 3);

    std::unordered_map<std::string_view, int> umap_1{};
    for (auto it = intro_0::begin(umap_0); it != intro_0::end(umap_0); it++) {
        umap_1.insert(*it);
    }
    ASSERT_EQ(umap_0, umap_1);

    std::unordered_map<std::string_view, int> umap_2{};
    intro_0::iterate(umap_0, [&umap_2]<typename K, typename V>(K&& key, V&& val) {
        static_assert(std::is_same_v<K, std::string_view const&>);
        static_assert(std::is_same_v<V, int&>);

        std::pair<const std::string_view, int> to_insert{key, val};
        umap_2.insert(std::move(to_insert));
    });
    ASSERT_EQ(umap_0, umap_2);

    intro_0::iterate(std::move(umap_0), []<typename K, typename V>(K&&, V&&) {
        static_assert(std::is_same_v<K, std::string_view const&>);
        static_assert(std::is_same_v<V, int>);
    });

    intro_0::iterate(std::move(static_cast<std::remove_cvref_t<decltype(umap_0)> const&>(umap_0)), []<typename K, typename V>(K&&, V&&) {
        static_assert(std::is_same_v<K, std::string_view const&>);
        static_assert(std::is_same_v<V, const int>);
    });
}

template<typename T0, typename T1>
static void test_tuple_like() {
    T0 tup_0{0, 1.2f, 3.4};
    using intro_0 = stf::intro::introspector_t<decltype(tup_0)>;

    T1 tup_1{5.6f, 7.8};
    using intro_1 = stf::intro::introspector_t<decltype(tup_1)>;

    T1 tup_2{9.10f, 11.12};
    using intro_2 = stf::intro::introspector_t<decltype(tup_2)>;

    ASSERT_EQ(intro_0::template get<0>(tup_0), 0);
    ASSERT_EQ(intro_0::template get<1>(tup_0), 1.2f);
    ASSERT_EQ(intro_0::template get<2>(tup_0), 3.4);

    ASSERT_EQ(intro_1::template get<0>(tup_1), 5.6f);
    ASSERT_EQ(intro_1::template get<1>(tup_1), 7.8);

    ASSERT_EQ(intro_2::template get<0>(tup_2), 9.10f);
    ASSERT_EQ(intro_2::template get<1>(tup_2), 11.12);

    ASSERT_TRUE(std::is_rvalue_reference_v<decltype(intro_0::template get<0>(std::move(tup_0)))>);

    intro_0::iterate(tup_0, [expected_idx = 0uz]<usize I, typename T>(std::integral_constant<usize, I>, T&& v) mutable {
        ASSERT_EQ(expected_idx++, I);

        switch (I) {
            case 0uz: ASSERT_EQ(v, 0); break;
            case 1uz: ASSERT_EQ(v, 1.2f); break;
            case 2uz: ASSERT_EQ(v, 3.4); break;
            default: ASSERT_TRUE(false); break;
        }
    });

    intro_1::iterate(tup_1, [expected_idx = 0uz]<typename T>(usize i, T&& v) mutable {
        ASSERT_EQ(expected_idx++, i);

        switch (i) {
            case 0uz: ASSERT_EQ(v, 5.6f); break;
            case 1uz: ASSERT_EQ(v, 7.8); break;
            default: ASSERT_TRUE(false); break;
        }
    });

    intro_2::iterate(tup_2, [expected_idx = 0uz]<usize I, typename T>(std::integral_constant<usize, I>, T&& v) mutable {
        ASSERT_EQ(expected_idx++, I);

        switch (I) {
            case 0uz: ASSERT_EQ(v, 9.10f); break;
            case 1uz: ASSERT_EQ(v, 11.12); break;
            default: ASSERT_TRUE(false); break;
        }
    });

    intro_2::iterate(tup_2, [expected_idx = 0uz]<typename T>(usize i, T&& v) mutable {
        ASSERT_EQ(expected_idx++, i);

        switch (i) {
            case 0uz: ASSERT_EQ(v, 9.10f); break;
            case 1uz: ASSERT_EQ(v, 11.12); break;
            default: ASSERT_TRUE(false); break;
        }
    });
}

TEST(intro, basic_tuple) { test_tuple_like<std::tuple<int, float, double>, std::tuple<float, double>>(); }

TEST(intro, basic_aggregate) {
    struct s_0 {
        int a;
        float b;
        double c;
    };

    struct s_1 {
        float a;
        double b;
    };

    test_tuple_like<s_0, s_1>();
}

TEST(intro, custom_introspector) {
    struct test_struct {
        int data[5];

        struct introspector_type {
            using intro_type = test_struct;
            using value_type = int;

            static constexpr auto size() -> usize { return 5; }
            static constexpr auto size(test_struct const&) -> usize { return size(); }

            static constexpr auto begin(intro_type const& v) -> const int* { return v.data; }
            static constexpr auto begin(intro_type& v) -> int* { return v.data; }
            static constexpr auto begin(intro_type const&& v) -> const int* { return v.data; }
            static constexpr auto begin(intro_type&& v) -> int* { return v.data; }

            static constexpr auto end(intro_type const& v) -> const int* { return v.data + size(); }
            static constexpr auto end(intro_type& v) -> int* { return v.data + size(); }
            static constexpr auto end(intro_type const&& v) -> const int* { return v.data + size(); }
            static constexpr auto end(intro_type&& v) -> int* { return v.data + size(); }

            static constexpr auto index(intro_type const& v, usize i) -> const int& { return v.data[i]; }
            static constexpr auto index(intro_type& v, usize i) -> int& { return v.data[i]; }
            static constexpr auto index(intro_type const&& v, usize i) -> const int&& { return std::move(v.data[i]); }
            static constexpr auto index(intro_type&& v, usize i) -> int&& { return std::move(v.data[i]); }
        };

        static_assert(stf::intro::concepts::span_introspector<introspector_type>);
    };

    using intro = stf::intro::introspector_t<test_struct>;

    test_struct foo{{1, 2, 3, 4, 5}};

    ASSERT_EQ(intro::size(), 5);
    ASSERT_EQ(intro::size(foo), 5);
    ASSERT_EQ(intro::index(foo, 0), 1);
    ASSERT_EQ(intro::index(foo, 1), 2);
    ASSERT_EQ(intro::index(foo, 2), 3);
    ASSERT_EQ(intro::index(foo, 3), 4);
    ASSERT_EQ(intro::index(foo, 4), 5);
}

TEST(intro, intro_builder) {
    struct square {
        int sl;
    };

    using intro_0 = stf::intro::intro_builder<square>                                               //
      ::add_field<int, "side_length", decltype([]<typename T>(T && v) -> auto&& { return v.sl; })>  //
      ::add_field<const int, "area", decltype([]<typename T>(T&& v) -> float { return v.sl * v.sl; })>;

    static_assert(stf::intro::concepts::tuple_introspector<intro_0>);

    square sq{
      .sl = 2,
    };

    ASSERT_EQ(intro_0::get<0>(sq), 2);
    ASSERT_EQ(intro_0::get<"side_length">(sq), 2);
    ASSERT_EQ(intro_0::get<1>(sq), 4);
    ASSERT_EQ(intro_0::get<"area">(sq), 4);

    intro_0::get<0>(sq) = 3;

    ASSERT_EQ(intro_0::get<0>(sq), 3);
    ASSERT_EQ(intro_0::get<"side_length">(sq), 3);
    ASSERT_EQ(intro_0::get<1>(sq), 9);
    ASSERT_EQ(intro_0::get<"area">(sq), 9);

    ASSERT_EQ(intro_0::get<0>(square{4}), 4);                               // const lvalue
    ASSERT_EQ(intro_0::get<0>(square{4}) = 4, 4);                           // rvalue
    ASSERT_EQ(intro_0::get<0>(static_cast<const square&&>(square{4})), 4);  // const rvalue
    ASSERT_EQ(intro_0::get<"side_length">(square{4}), 4);

    ASSERT_EQ(intro_0::get<1>(square{4}), 16);
    // ASSERT_EQ(intro_0::get<1>(square{4}) = 4, 16); // won't work, read-only field
    ASSERT_EQ(intro_0::get<1>(static_cast<const square&&>(square{4})), 16);
    ASSERT_EQ(intro_0::get<"area">(square{4}), 16);
}

TEST(intro, intro_builder_quick_build) {
    struct test_struct {
        int a;
        float b;
        double c;
    };

    using intro_0 = stf::intro::intro_builder<test_struct>::quick_build<"a", "b", "c">;
    static_assert(stf::intro::concepts::tuple_introspector<intro_0>);

    static_assert(stf::intro::arity_v<test_struct> == 3);

    test_struct s{
      .a = 1,
      .b = 2.3f,
      .c = 4.5,
    };

    ASSERT_EQ(intro_0::get<0>(s), 1);
    ASSERT_EQ(intro_0::get<"a">(s), 1);
    ASSERT_EQ(intro_0::get<1>(s), 2.3f);
    ASSERT_EQ(intro_0::get<"b">(s), 2.3f);
    ASSERT_EQ(intro_0::get<2>(s), 4.5);
    ASSERT_EQ(intro_0::get<"c">(s), 4.5);
}
