#pragma once

namespace stf::sfloat::ieee {

template<concepts::ieee_float_description Desc>
constexpr auto trunc(soft_float<Desc> v, fenv&& env = fenv()) -> soft_float<Desc> {
    using signed_type = std::make_signed_t<typename Desc::repr_type>;
    using unsigned_type = typename Desc::repr_type;

    signed_type real_exponent = v.real_exponent();

    if (real_exponent == static_cast<signed_type>(v.fraction_mask / 2)) {  // inf or nan
        return add(v, v, std::move(env));                                             // raise exceptions
    }

    if (real_exponent >= static_cast<signed_type>(v.fraction_bits)) {  // definitely an integer
        return v;
    }

    // ± 2^e * (1 + 2^-m_b * m)
    //        ^^^^^^^^^^^^^^^^
    //             [1, 2)

    if (real_exponent < 0) {
        // 2^e * (1 + 2^-m_b * m)
        // ^^^
        // (0, 0.5]
        // the whole expression is (0, 1) normal or (0, 0.5) if subnormal
        // return ±0 in any case

        return v.set_to_zero(true);
    }

    // the most significant `real_exponent` bits of the mantissa contribute to the integer part of the
    // represented number, the rest contribute to the fractional part. we can discard anything besides the
    // most significant `real_exponent` bits

    v.repr &= ~(v.fraction_mask >> real_exponent);

    return v;
}

template<concepts::ieee_float_description Desc>
constexpr auto inverse_trunc(soft_float<Desc> v, fenv&& env = fenv()) -> soft_float<Desc> {
    using signed_type = std::make_signed_t<typename Desc::repr_type>;
    using unsigned_type = typename Desc::repr_type;

    signed_type real_exponent = v.real_exponent();

    if (real_exponent == static_cast<signed_type>(v.fraction_mask / 2)) {  // inf or nan
        return add(v, v, std::move(env));                                  // raise exceptions
    }

    if (real_exponent >= static_cast<signed_type>(v.fraction_bits)) {  // definitely an integer
        return v;
    }

    if (v.repr << 1 == 0) {
        return v.set_to_zero(true);
    }

    if (real_exponent < 0) {             // |v| ∈ (0, 1) if norm or (0, 0.5) if snorm
        return v.set_to_pow_2(0, true);  // ±1
    }

    unsigned_type fraction_mask = v.mantissa_mask >> real_exponent;

    if (unsigned_type fraction = v.mantissa() & fraction_mask; fraction == 0) {  // integer value
        return v;
    }

    unsigned_type increment = static_cast<unsigned_type>(1) << (v.fraction_bits - real_exponent);

    v.repr &= ~fraction_mask;
    v.repr += increment;

    return v;
}

template<concepts::ieee_float_description Desc>
constexpr auto ceil(soft_float<Desc> v, fenv&& env = fenv()) -> soft_float<Desc> {
    return v.sign_bit() ? trunc(v, std::move(env)) : inverse_trunc(v, std::move(env));
}

template<concepts::ieee_float_description Desc>
constexpr auto floor(soft_float<Desc> v, fenv&& env = fenv()) -> soft_float<Desc> {
    return v.sign_bit() ? inverse_trunc(v, std::move(env)) : trunc(v, std::move(env));
}

}  // namespace stf::sfloat::ieee
