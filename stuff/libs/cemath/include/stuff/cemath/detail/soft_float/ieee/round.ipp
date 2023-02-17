#pragma once

namespace stf::cemath::detail {

template<typename Desc>
constexpr auto ieee_soft_float<Desc>::round() const -> ieee_soft_float {
    int real_exponent = static_cast<int>(exponent()) - m_exponent_mask / 2;

    if (bool definitely_an_integer = real_exponent >= static_cast<int>(m_mantissa_bits); definitely_an_integer) {
        if (real_exponent == m_exponent_mask / 2 + 1) [[unlikely]] {
            // do something special with nan or inf?
        }

        return *this;
    }

    if (bool mantissa_decides = real_exponent >= 0; !mantissa_decides) {  // 1 > |val|
        if (real_exponent == -1) {                                        // 1 > |val| >= 0.5
            return from_parts(sign_bit(), m_exponent_mask >> 1, 0);       // ±1
        }

        // 0.5 > |val|
        return from_parts(sign_bit(), 0, 0);  // ±0
    }

    repr_type fraction_mask = m_mantissa_mask >> real_exponent;
    if (bool fractional = (m_repr & fraction_mask) != 0; !fractional) {
        return *this;
    }

    int deciding_bit = m_mantissa_bits - real_exponent - 1;
    repr_type increment = static_cast<repr_type>(1) << deciding_bit;
    repr_type new_repr = (m_repr + increment) & ~fraction_mask;

    return from_repr(new_repr);
}

}  // namespace stf::cemath::detail
