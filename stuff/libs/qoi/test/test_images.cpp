#include "./test_images.hpp"

namespace {

#include "./test_images/headers/dice.qoi.h"
#include "./test_images/headers/dice.data.h"
#include "./test_images/headers/edgecase.qoi.h"
#include "./test_images/headers/edgecase.data.h"
#include "./test_images/headers/kodim10.qoi.h"
#include "./test_images/headers/kodim10.data.h"
#include "./test_images/headers/kodim23.qoi.h"
#include "./test_images/headers/kodim23.data.h"
#include "./test_images/headers/qoi_logo.qoi.h"
#include "./test_images/headers/qoi_logo.data.h"
#include "./test_images/headers/testcard.qoi.h"
#include "./test_images/headers/testcard.data.h"
#include "./test_images/headers/testcard_rgba.qoi.h"
#include "./test_images/headers/testcard_rgba.data.h"
#include "./test_images/headers/wikipedia_008.qoi.h"
#include "./test_images/headers/wikipedia_008.data.h"

}

namespace stf::qoi::test_images {

#pragma PUSH_MACRO("FACTORY")
#define FACTORY(_name)                                \
std::span<const uint8_t> _name##_qoi(::_name##_qoi);  \
std::span<const uint8_t> _name##_data(::_name##_data) \

FACTORY(dice);
FACTORY(edgecase);
FACTORY(kodim10);
FACTORY(kodim23);
FACTORY(qoi_logo);
FACTORY(testcard);
FACTORY(testcard_rgba);
FACTORY(wikipedia_008);

#undef FACTORY
#pragma POP_MACRO("FACTORY")

}
