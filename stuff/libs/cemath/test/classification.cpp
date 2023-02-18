#include "./common.hpp"

template<std::floating_point T>
static void test_classification() {
    constexpr T qnan = std::numeric_limits<T>::quiet_NaN();
    constexpr T snan = std::numeric_limits<T>::signaling_NaN();
    constexpr T inf = std::numeric_limits<T>::infinity();

    constexpr T min_norm = std::numeric_limits<T>::min();
    constexpr T min_snorm = std::numeric_limits<T>::denorm_min();

    SASSERT_TRUE(stf::cemath::isnormal(min_norm));
    SASSERT_FALSE(stf::cemath::isnormal(min_snorm));
    //
    SASSERT_TRUE(stf::cemath::isnan(qnan));
    SASSERT_TRUE(stf::cemath::isnan(snan));
    SASSERT_TRUE(stf::cemath::isinf(inf));
    //

    SASSERT_TRUE(stf::cemath::isgreater(min_norm, min_snorm));
    SASSERT_FALSE(stf::cemath::islessequal(min_norm, min_snorm));
    SASSERT_FALSE(stf::cemath::isless(min_norm, min_snorm));
    SASSERT_TRUE(stf::cemath::isgreaterequal(min_norm, min_snorm));

    //
    SASSERT_TRUE(stf::cemath::isunordered(qnan, min_norm));
    SASSERT_TRUE(stf::cemath::isunordered(qnan, snan));
}

TEST(cemath, classification) {
    test_classification<float>();
    test_classification<double>();
    test_classification<long double>();
}
