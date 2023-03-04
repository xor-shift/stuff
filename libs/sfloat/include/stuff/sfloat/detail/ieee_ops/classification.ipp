#pragma once

#include <stuff/sfloat/classification.hpp>

namespace stf::sfloat::ieee {

template<concepts::ieee_float_description Desc>
constexpr auto classify(soft_float<Desc> v) -> classification {
    bool potentially_snorm = (v.repr & v.shifted_exponent_mask) == 0;
    bool potentially_inf = (v.repr & v.shifted_exponent_mask) == v.shifted_exponent_mask;
    bool zero_mantissa = (v.repr & v.mantissa_mask) == 0;

    if (potentially_snorm) {
        return zero_mantissa ? classification::zero : classification::subnormal;
    }

    if (potentially_inf) {
        return zero_mantissa ? classification::infinite : classification::nan;
    }

    return classification::normal;
}

template<concepts::ieee_float_description Desc>
constexpr auto is_finite(soft_float<Desc> v) -> bool {
    return (v.repr & v.shifted_exponent_mask) != v.shifted_exponent_mask;
}

template<concepts::ieee_float_description Desc>
constexpr auto is_infinite(soft_float<Desc> v) -> bool {
    bool potentially_inf = (v.repr & v.shifted_exponent_mask) == v.shifted_exponent_mask;
    bool zero_mantissa = (v.repr & v.mantissa_mask) == 0;
    return zero_mantissa && potentially_inf;
}

template<concepts::ieee_float_description Desc>
constexpr auto is_nan(soft_float<Desc> v) -> bool {
    bool potentially_inf = (v.repr & v.shifted_exponent_mask) == v.shifted_exponent_mask;
    bool zero_mantissa = (v.repr & v.mantissa_mask) == 0;
    return !zero_mantissa && potentially_inf;
}

template<concepts::ieee_float_description Desc>
constexpr auto is_normal(soft_float<Desc> v) -> bool {
    bool potentially_snorm = (v.repr & v.shifted_exponent_mask) == 0;
    bool potentially_inf = (v.repr & v.shifted_exponent_mask) == v.shifted_exponent_mask;
    return !potentially_snorm && !potentially_inf;
}

template<concepts::ieee_float_description Desc>
constexpr auto sign_bit(soft_float<Desc> v) -> bool {
    return (v.repr >> (v.mantissa_bits + v.exponent_bits)) != 0;
}

template<concepts::ieee_float_description Desc>
constexpr auto order(soft_float<Desc> lhs, soft_float<Desc> rhs) -> std::partial_ordering {
    if ((lhs.repr << 1) == 0 && (rhs.repr << 1) == 0) {
        return std::partial_ordering::equivalent;
    }

    if (is_nan(lhs) || is_nan(rhs)) {
        return std::partial_ordering::unordered;
    }

    if (sign_bit(lhs) && !sign_bit(rhs)) {
        return std::partial_ordering::less;
    }

    if (!sign_bit(lhs) && sign_bit(rhs)) {
        return std::partial_ordering::greater;
    }

    bool invert = sign_bit(lhs);

    return invert ? rhs.repr <=> lhs.repr : lhs.repr <=> rhs.repr;
}

}
