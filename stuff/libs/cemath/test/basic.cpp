#include "./common.hpp"

TEST(cemath, abs) {
    using stf::cemath::abs;

    SASSERT_EQ(abs(-1), 1);
    SASSERT_EQ(abs(1), 1);
    SASSERT_EQ(abs(-1l), 1l);
    SASSERT_EQ(abs(1l), 1l);
    SASSERT_EQ(abs(-1ll), 1ll);
    SASSERT_EQ(abs(1ll), 1ll);

    SASSERT_EQ(abs(-1.f), 1.f);
    SASSERT_EQ(abs(1.f), 1.f);
    SASSERT_EQ(abs(-0.f), 0.f);
    SASSERT_EQ(abs(0.f), 0.f);

    SASSERT_EQ(abs(-1.), 1.);
    SASSERT_EQ(abs(1.), 1.);
    SASSERT_EQ(abs(-0.), 0.);
    SASSERT_EQ(abs(0.), 0.);

    SASSERT_EQ(abs(-1.l), 1.l);
    SASSERT_EQ(abs(1.l), 1.l);
    SASSERT_EQ(abs(-0.l), 0.l);
    SASSERT_EQ(abs(0.l), 0.l);
}
