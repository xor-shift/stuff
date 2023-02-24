#pragma once

#include <cmath>

namespace stf::sfloat {

enum class classification : int {
    nan = FP_NAN,
    infinite = FP_INFINITE,
    zero = FP_ZERO,
    subnormal = FP_SUBNORMAL,
    normal = FP_NORMAL,
};

}
