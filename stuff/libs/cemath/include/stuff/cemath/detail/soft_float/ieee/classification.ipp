#pragma once

namespace stf::cemath::detail {

template<typename Desc>
constexpr auto ieee_soft_float<Desc>::classify() const -> classification {
    if (exponent() == 0) {
        return mantissa() == 0 ? classification::zero : classification::subnormal;
    }

    if (exponent() == m_exponent_mask) {
        return mantissa() == 0 ? classification::infinite : classification::nan;
    }

    return classification::normal;
}

}
