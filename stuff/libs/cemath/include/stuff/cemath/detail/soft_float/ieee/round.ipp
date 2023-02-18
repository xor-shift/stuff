#pragma once

namespace stf::cemath::detail {

template<typename Desc>
constexpr auto ieee_soft_float<Desc>::round() -> ieee_soft_float& {
    int real_exponent = static_cast<int>(exponent()) - m_exponent_mask / 2;

    if (bool definitely_an_integer = real_exponent >= static_cast<int>(m_mantissa_bits); definitely_an_integer) {
        return *this;
    }

    if (bool mantissa_decides = real_exponent >= 0; !mantissa_decides) {  // 1 > |val|
        if (real_exponent == -1) {                                        // 1 > |val| >= 0.5
            return set_to_pow_2(0, true);                                 // ±1
        }

        // 0.5 > |val|
        return set_to_zero(true);
    }

    repr_type fraction_mask = m_mantissa_mask >> real_exponent;
    if (bool fractional = (m_repr & fraction_mask) != 0; !fractional) {
        return *this;
    }

    int deciding_bit = m_mantissa_bits - real_exponent - 1;
    repr_type increment = static_cast<repr_type>(1) << deciding_bit;
    m_repr = (m_repr + increment) & ~fraction_mask;

    return *this;
}

}  // namespace stf::cemath::detail
