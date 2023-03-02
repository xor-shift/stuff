#pragma once

#include <stuff/core/integers.hpp>

namespace stf::qoi::detail {

enum class qoi_op : u8 {
    index,
    diff,
    luma,
    run,
    rgb,
    rgba,
};

}
