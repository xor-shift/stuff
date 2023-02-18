#pragma once

namespace stf::cemath::detail {

template<typename Desc>
constexpr auto ieee_soft_float<Desc>::trunc() -> ieee_soft_float& {
    int real_exponent = static_cast<int>(exponent()) - m_exponent_mask / 2;

    if (real_exponent >= static_cast<int>(m_mantissa_bits)) {
        return *this;
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

        return set_to_zero(true);
    }

    // the most significant `real_exponent` bits of the mantissa contribute to the integer part of the
    // represented number, the rest contribute to the fractional part. we can discard anything besides the
    // most significant `real_exponent` bits

    m_repr &= ~(m_mantissa_mask >> real_exponent);
    return *this;
}

template<typename Desc>
constexpr auto ieee_soft_float<Desc>::inverse_trunc() -> ieee_soft_float& {
    int real_exponent = static_cast<int>(exponent()) - m_exponent_mask / 2;

    if (real_exponent >= static_cast<int>(m_mantissa_bits)) {
        return *this;
    }

    if (m_repr << 1 == 0) {
        return set_to_zero(true);
    }

    if (real_exponent < 0) {           // |v| ∈ (0, 1) if norm or (0, 0.5) if snorm
        return set_to_pow_2(0, true);  // ±1
    }

    repr_type fraction_mask = m_mantissa_mask >> real_exponent;

    if (repr_type fraction = mantissa() & fraction_mask; fraction == 0) {  // integer value
        return *this;
    }

    repr_type increment = static_cast<repr_type>(1) << (m_mantissa_bits - real_exponent);

    m_repr &= ~fraction_mask;
    m_repr += increment;

    return *this;
}

}  // namespace stf::cemath::detail
