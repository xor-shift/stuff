#pragma once

namespace stf::cemath::detail {

template<typename Desc>
constexpr auto operator<=>(ieee_soft_float<Desc> lhs, ieee_soft_float<Desc> rhs) -> std::partial_ordering {
    classification lhs_class = lhs.classify();
    classification rhs_class = rhs.classify();

    if (lhs_class == classification::zero && rhs_class == classification::zero) {
        return std::partial_ordering::equivalent;
    }

    if (lhs_class == classification::nan || rhs_class == classification::nan) {
        return std::partial_ordering::unordered;
    }

    if (lhs.sign_bit() && !rhs.sign_bit()) {
        return std::partial_ordering::less;
    }

    if (!lhs.sign_bit() && rhs.sign_bit()) {
        return std::partial_ordering::greater;
    }

    bool invert = lhs.sign_bit();

    return invert ? rhs.m_repr <=> lhs.m_repr : lhs.m_repr <=> rhs.m_repr;
}

}  // namespace stf::cemath::detail
