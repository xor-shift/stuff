#pragma once

#include <stuff/sfloat/env.hpp>

namespace stf::sfloat::ieee {

template<concepts::ieee_float_description Desc>
constexpr auto copysign(soft_float<Desc> mag, soft_float<Desc> sgn) -> soft_float<Desc> {
    mag.repr &= (mag.shifted_exponent_mask | mag.mantissa_mask);
    mag.repr |= sgn.repr & ~(mag.shifted_exponent_mask | mag.mantissa_mask);
    return mag;
}

template<concepts::ieee_float_description Desc>
constexpr auto scalbn(soft_float<Desc> v, fenv&& env = fenv()) -> soft_float<Desc> {

}

}
