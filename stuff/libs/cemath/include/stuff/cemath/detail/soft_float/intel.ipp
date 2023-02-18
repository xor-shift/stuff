#pragma once

#include <stuff/cemath/detail/soft_float/ieee_esque.ipp>

namespace stf::cemath::detail {

struct intel_efp_soft_float : ieee_esque_float<intel_efp_soft_float> {
    using repr_type = u128;

    constexpr intel_efp_soft_float() = default;
    constexpr intel_efp_soft_float(repr_type repr) : m_repr(repr) {}

    static constexpr auto from_repr(repr_type v) -> intel_efp_soft_float { return {v}; }
    constexpr auto get_repr() const -> repr_type { return m_repr; }

    constexpr auto sign_bit() const -> bool { return get_bit(m_repr, m_exponent_bits + m_mantissa_bits); }

    constexpr auto ceil() -> intel_efp_soft_float& { return *this; /* STUB */ }
    constexpr auto floor() -> intel_efp_soft_float& { return *this; /* STUB */ }
    constexpr auto trunc() -> intel_efp_soft_float& { return *this; /* STUB */ }
    constexpr auto round() -> intel_efp_soft_float& { return *this; /* STUB */ }

private:
    friend struct ieee_esque_float<intel_efp_soft_float>;

    inline static constexpr repr_type m_exponent_bits = 15;
    inline static constexpr repr_type m_mantissa_bits = 64;

    inline static constexpr repr_type m_exponent_mask = ((repr_type)1 << m_exponent_bits) - 1;
    inline static constexpr repr_type m_mantissa_mask = ((repr_type)1 << m_mantissa_bits) - 1;
    inline static constexpr repr_type m_fraction_mask = ((repr_type)1 << (m_mantissa_bits - 1)) - 1;
    inline static constexpr repr_type m_payload_mask = ((repr_type)1 << (m_exponent_bits + m_mantissa_bits)) - 1;

    repr_type m_repr;

    constexpr void set_sign(bool sign) { m_repr = set_bit(m_repr, m_exponent_bits + m_mantissa_bits, sign); }
    constexpr auto exponent() const -> repr_type { return (m_repr >> m_mantissa_bits) & m_exponent_mask; }
    constexpr auto mantissa() const -> repr_type { return m_repr & m_mantissa_mask; }

    constexpr auto fraction() const -> repr_type { return m_repr & m_fraction_mask; }
    constexpr auto int_part() const -> bool { return get_bit(m_repr, m_mantissa_bits - 1); }
};

static_assert(concepts::soft_float<intel_efp_soft_float>);

constexpr auto operator<=>(intel_efp_soft_float lhs, intel_efp_soft_float rhs) -> std::partial_ordering {
    return ieee_esque_float<intel_efp_soft_float>::order(lhs, rhs);
}

}  // namespace stf::cemath::detail

namespace stf::cemath {

template<>
struct native_float<long double> : std::type_identity<detail::intel_efp_soft_float> {};

}  // namespace stf::cemath
