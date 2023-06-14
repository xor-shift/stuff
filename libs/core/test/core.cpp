#include <stuff/core.hpp>

#include <gtest/gtest.h>

using BoT_0 = stf::bunch_of_types<>;
using BoT_1 = stf::bunch_of_types<int>;
using BoT_2 = stf::bunch_of_types<int, char, long>;

TEST(core, bunch_of_types_nth_type) {
    ASSERT_EQ(BoT_0::size, 0);

    ASSERT_EQ(BoT_1::size, 1);
    ASSERT_TRUE((std::is_same_v<BoT_1::nth_type<0>, int>));

    ASSERT_EQ(BoT_2::size, 3);
    ASSERT_TRUE((std::is_same_v<BoT_2::nth_type<0>, int>));
    ASSERT_TRUE((std::is_same_v<BoT_2::nth_type<1>, char>));
    ASSERT_TRUE((std::is_same_v<BoT_2::nth_type<2>, long>));
}

template<typename T>
struct predicate_is_same_v {
    template<typename U>
    struct predicate : std::is_same<T, U> {};
};

TEST(core, bunch_of_types_find_type) {
    ASSERT_EQ(BoT_1::find_type<predicate_is_same_v<int>::predicate>::value, 0);
    ASSERT_EQ(BoT_1::find_type<predicate_is_same_v<char>::predicate>::value, 1);
    ASSERT_EQ(BoT_1::find_type<predicate_is_same_v<long>::predicate>::value, 1);

    ASSERT_EQ(BoT_2::find_type<predicate_is_same_v<int>::predicate>::value, 0);
    ASSERT_EQ(BoT_2::find_type<predicate_is_same_v<char>::predicate>::value, 1);
    ASSERT_EQ(BoT_2::find_type<predicate_is_same_v<long>::predicate>::value, 2);
    ASSERT_EQ(BoT_2::find_type<predicate_is_same_v<void>::predicate>::value, 3);
    ASSERT_EQ(BoT_2::find_type<predicate_is_same_v<char*>::predicate>::value, 3);
    ASSERT_EQ(BoT_2::find_type<predicate_is_same_v<std::string>::predicate>::value, 3);
}
