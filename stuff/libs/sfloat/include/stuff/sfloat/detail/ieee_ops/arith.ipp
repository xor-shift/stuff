#pragma once

namespace stf::sfloat::ieee {

template<concepts::ieee_float_description Desc>
constexpr auto operator-(soft_float<Desc> v) -> soft_float<Desc> {
    v.repr ^= static_cast<typename Desc::repr_type>(1) << (v.mantissa_bits + v.exponent_bits);
    return v;
}

}
