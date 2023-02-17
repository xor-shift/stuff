#pragma once

namespace stf::cemath::detail {

template<typename Desc>
constexpr auto ieee_soft_float<Desc>::trunc() const -> ieee_soft_float {
    int real_exponent = static_cast<int>(exponent()) - m_exponent_mask / 2;

    // ± 2^e * (1 + 2^-m_b * m)
    //        ^^^^^^^^^^^^^^^^
    //             [1, 2)

    if (real_exponent < 0) {
        // 2^e * (1 + 2^-m_b * m)
        // ^^^
        // (0, 0.5]
        // the whole expression is (0, 1) normal or (0, 0.5) if subnormal
        // return ±0 in any case

        return from_parts(sign_bit(), 0, 0);
    }

    // actual truncation here
    if (real_exponent < m_mantissa_bits) {
        // the most significant `real_exponent` bits of the mantissa contribute to the integer part of the
        // represented number, the rest contribute to the fractional part. we can discard anything besides the
        // most significant `real_exponent` bits

        repr_type new_mantissa = mantissa() & ~(m_mantissa_mask >> real_exponent);
        return from_parts(sign_bit(), exponent(), new_mantissa);
    }

    if (real_exponent == m_exponent_mask / 2 + 1) [[unlikely]] {
        // do something special with nan or inf?
    }

    // value is not fractional
    return *this;
}

template<typename Desc>
constexpr auto ieee_soft_float<Desc>::inverse_trunc() const -> ieee_soft_float {
    int real_exponent = static_cast<int>(exponent()) - m_exponent_mask / 2;

    if (m_repr << 1 == 0) {
        return from_parts(sign_bit(), 0, 0);
    }

    if (real_exponent < 0) {                                     // |v| ∈ (0, 1) if norm or (0, 0.5) if snorm
        return from_parts(sign_bit(), m_exponent_mask >> 1, 0);  // ±1
    }

    if (real_exponent < m_mantissa_bits) {  // v >= 1
        repr_type fraction_mask = m_mantissa_mask >> real_exponent;

        if (repr_type fraction = mantissa() & fraction_mask; fraction == 0) {  // integer value
            return *this;
        }

        repr_type increment = static_cast<repr_type>(1) << (m_mantissa_bits - real_exponent);

        ieee_soft_float ret = *this;
        ret.m_repr &= ~fraction_mask;
        ret.m_repr += increment;
        if (bool overflown = ret.sign_bit() != sign_bit(); overflown) {  // is this possible?
            return from_parts(sign_bit(), m_exponent_mask, 0);           // inf
        }

        return ret;
    }

    if (real_exponent == m_exponent_mask / 2 + 1) [[unlikely]] {
        // do something special?
    }

    return *this;
}

}  // namespace stf::cemath::detail
