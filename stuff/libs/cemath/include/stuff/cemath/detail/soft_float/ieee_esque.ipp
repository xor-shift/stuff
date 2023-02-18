#pragma once

namespace stf::cemath::detail {

template<typename T>
struct ieee_esque_float {
    constexpr auto classify() const -> classification {
        T const& self = static_cast<T const&>(*this);

        if (self.exponent() == 0) {
            return self.fraction() == 0 ? classification::zero : classification::subnormal;
        }

        if (self.exponent() == self.m_exponent_mask) {
            return self.fraction() == 0 ? classification::infinite : classification::nan;
        }

        return classification::normal;
    }

    constexpr auto abs() -> T& {
        T& self = static_cast<T&>(*this);

        self.set_sign(false);

        return self;
    }

    static constexpr auto order(ieee_esque_float const& lhsf, ieee_esque_float const& rhsf) -> std::partial_ordering {
        T const& lhs = static_cast<T const&>(lhsf);
        T const& rhs = static_cast<T const&>(rhsf);

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

        return invert ? rhs.get_repr() <=> lhs.get_repr() : lhs.get_repr() <=> rhs.get_repr();
    }
};

}  // namespace stf::cemath::detail
