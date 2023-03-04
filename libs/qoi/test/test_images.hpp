#pragma once

#include <cstdint>
#include <span>

namespace stf::qoi::test_images {

#pragma push_macro("DEF_FACTORY")
#define DEF_FACTORY(_name)                       \
    extern std::span<const uint8_t> _name##_qoi; \
    extern std::span<const uint8_t> _name##_data

DEF_FACTORY(dice);
DEF_FACTORY(edgecase);
DEF_FACTORY(kodim10);
DEF_FACTORY(kodim23);
DEF_FACTORY(qoi_logo);
DEF_FACTORY(testcard);
DEF_FACTORY(testcard_rgba);
DEF_FACTORY(wikipedia_008);

#undef DEF_FACTORY
#pragma pop_macro("DEF_FACTORY")

}  // namespace stf::qoi::test_images
