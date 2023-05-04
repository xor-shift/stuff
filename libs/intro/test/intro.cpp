#include <stuff/intro/detail/aggregate/tie.hpp>

#include <stuff/intro.hpp>

#include <gtest/gtest.h>

#include <stuff/intro/introspectors/array.hpp>

TEST(intro, basic_array) {
    std::array<int, 3> arr_0{1, 2, 3};
    auto intro_0 = _stf_adl_introspector(arr_0);

    int arr_1[3]{1, 2, 3};
    auto intro_1 = _stf_adl_introspector(arr_1);

    const int arr_2[3]{1, 2, 3};
    auto intro_2 = _stf_adl_introspector(arr_2);

    ASSERT_EQ(intro_0.size(arr_0), 3uz);
    ASSERT_EQ(intro_1.size(arr_1), 3uz);
    ASSERT_EQ(intro_2.size(arr_2), 3uz);

    ASSERT_EQ(intro_0.get<0>(arr_0), 1);
    ASSERT_EQ(intro_1.get<1>(arr_1), 2);
    ASSERT_EQ(intro_2.get<2>(arr_2), 3);

    intro_0.get<0>(arr_0) = 2;
    intro_1.get<1>(arr_1) = 3;

    ASSERT_EQ(intro_0.get<0>(arr_0), 2);
    ASSERT_EQ(intro_1.get<1>(arr_1), 3);
    ASSERT_EQ(intro_2.get<2>(arr_2), 3);

    int sum_0 = 0;
    for (auto it = intro_0.begin(arr_0); it != intro_0.end(arr_0); it++) {
        sum_0 += *it;
    }
    ASSERT_EQ(sum_0, 7);
}

#include <stuff/intro/introspectors/tuple.hpp>

TEST(intro, basic_tuple) {
    std::tuple<int, float, double> tup_0{0, 1.2f, 3.4};
    auto intro_0 = _stf_adl_introspector(tup_0);

    std::tuple<float, double> tup_1{5.6f, 7.8};
    auto intro_1 = _stf_adl_introspector(tup_1);

    std::pair<float, double> tup_2{9.10f, 11.12};
    auto intro_2 = _stf_adl_introspector(tup_2);

    ASSERT_EQ(intro_0.get<0>(tup_0), 0);
    ASSERT_EQ(intro_0.get<1>(tup_0), 1.2f);
    ASSERT_EQ(intro_0.get<2>(tup_0), 3.4);

    ASSERT_EQ(intro_1.get<0>(tup_1), 5.6f);
    ASSERT_EQ(intro_1.get<1>(tup_1), 7.8);

    ASSERT_EQ(intro_2.get<0>(tup_2), 9.10f);
    ASSERT_EQ(intro_2.get<1>(tup_2), 11.12);

    ASSERT_TRUE(std::is_rvalue_reference_v<decltype(intro_0.get<0>(std::move(tup_0)))>);

    intro_0.iterate(tup_0, [expected_idx = 0uz]<usize I, typename T>(std::integral_constant<usize, I>, T&& v) mutable {
        ASSERT_EQ(expected_idx++, I);

        switch (I) {
            case 0uz: ASSERT_EQ(v, 0); break;
            case 1uz: ASSERT_EQ(v, 1.2f); break;
            case 2uz: ASSERT_EQ(v, 3.4); break;
            default: ASSERT_TRUE(false); break;
        }
    });

    intro_1.iterate(tup_1, [expected_idx = 0uz]<typename T>(usize i, T&& v) mutable {
        ASSERT_EQ(expected_idx++, i);

        switch (i) {
            case 0uz: ASSERT_EQ(v, 5.6f); break;
            case 1uz: ASSERT_EQ(v, 7.8); break;
            default: ASSERT_TRUE(false); break;
        }
    });

    intro_2.iterate(tup_2, [expected_idx = 0uz]<usize I, typename T>(std::integral_constant<usize, I>, T&& v) mutable {
        ASSERT_EQ(expected_idx++, I);

        switch (I) {
            case 0uz: ASSERT_EQ(v, 9.10f); break;
            case 1uz: ASSERT_EQ(v, 11.12); break;
            default: ASSERT_TRUE(false); break;
        }
    });

    intro_2.iterate(tup_2, [expected_idx = 0uz]<typename T>(usize i, T&& v) mutable {
        ASSERT_EQ(expected_idx++, i);

        switch (i) {
            case 0uz: ASSERT_EQ(v, 9.10f); break;
            case 1uz: ASSERT_EQ(v, 11.12); break;
            default: ASSERT_TRUE(false); break;
        }
    });
}

#include <stuff/intro/introspectors/span.hpp>

TEST(intro, basic_span) {
    std::array<int, 3> arr_0{1, 2, 3};
    std::span<int> span_0{arr_0};
    auto intro_0 = _stf_adl_introspector(span_0);

    ASSERT_EQ(intro_0.size(span_0), 3);
    ASSERT_EQ(intro_0.index(span_0, 0), 1);
    ASSERT_EQ(intro_0.index(span_0, 1), 2);
    ASSERT_EQ(intro_0.index(span_0, 2), 3);

    int sum_0 = 0;
    for (auto it = intro_0.begin(span_0); it != intro_0.end(span_0); it++) {
        sum_0 += *it;
    }
    ASSERT_EQ(sum_0, 6);
}

#include <stuff/intro/detail/aggregate/member_types.hpp>
#include <stuff/intro/introspectors/unordered_map.hpp>

TEST(intro, basic_umap) {
    std::unordered_map<std::string_view, int> umap_0{{"a", 1}, {"b", 2}, {"c", 3}};
    auto intro_0 = _stf_adl_introspector(umap_0);

    ASSERT_EQ(intro_0.size(umap_0), 3);
    ASSERT_EQ(intro_0.index(umap_0, "a"), 1);
    ASSERT_EQ(intro_0.index(umap_0, "b"), 2);
    ASSERT_EQ(intro_0.index(umap_0, "c"), 3);

    std::unordered_map<std::string_view, int> umap_1{};
    for (auto it = intro_0.begin(umap_0); it != intro_0.end(umap_0); it++) {
        umap_1.insert(*it);
    }
    ASSERT_EQ(umap_0, umap_1);
}

using stf::intro::detail::agg::make_reference_like;
using stf::intro::arity_v;


template<typename T>
    requires (arity_v<std::remove_cvref_t<T>> == 5)
inline auto tie_members(T&& v) {
    auto&& [a, b, c, d, e] = v;
    return std::forward_as_tuple(static_cast<typename make_reference_like<T, decltype(a)>::type>(a), static_cast<typename make_reference_like<T, decltype(b)>::type>(b), static_cast<typename make_reference_like<T, decltype(c)>::type>(c), static_cast<typename make_reference_like<T, decltype(d)>::type>(d), static_cast<typename make_reference_like<T, decltype(e)>::type>(e));
}

TEST(intro, basic_sagg) {
    struct test_struct_0 {
        int a;
        int& b;
        int const& c;
        int&& d;
        int const&& e;
    };
    int a = 0;
    test_struct_0 foo{a, a, a, std::move(a), std::move(a)};
    auto tied = tie_members(foo);
    static_assert(std::is_same_v<decltype(tied), std::tuple<int&, int&, int const&, int&&, int const&&>>);
}
