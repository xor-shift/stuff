#pragma once

namespace stf::sfloat::ieee {

template<concepts::ieee_float_description Desc>
constexpr auto abs(soft_float<Desc> v, fenv&& env = fenv()) -> soft_float<Desc> {
    if (is_nan(v)) {
        return add(v, v, env);
    }

    v.repr &= (static_cast<typename Desc::repr_type>(1) << (v.mantissa_bits + v.exponent_bits)) - 1;
    return v;
}

}  // namespace stf::sfloat::ieee
