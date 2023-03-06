#pragma once

namespace stf::sfloat::ieee {

template<concepts::ieee_float_description Desc>
constexpr auto abs(soft_float<Desc> v, fenv& env) -> soft_float<Desc> {
    if (is_nan(v)) {
        return add(v, v, env);
    }

    v.repr &= (static_cast<typename Desc::repr_type>(1) << (v.mantissa_bits + v.exponent_bits)) - 1;
    return v;
}

template<concepts::ieee_float_description Desc>
constexpr auto abs(soft_float<Desc> v) -> soft_float<Desc> {
    return STF_SFLOAT_ENVLESS_CALL(abs, v);
}

}  // namespace stf::sfloat::ieee
