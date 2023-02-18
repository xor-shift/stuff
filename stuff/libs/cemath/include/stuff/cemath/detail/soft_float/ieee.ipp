#pragma once

#include <stuff/cemath/detail/soft_float/ieee_esque.ipp>

#include <stuff/cemath/bit.hpp>
#include <stuff/core/integers.hpp>

#include <compare>
#include <utility>

namespace stf::cemath::detail {

struct ieee_float_desc_sp {
    using repr_type = u32;
    inline static constexpr repr_type exponent_bits = 8;
    inline static constexpr repr_type mantissa_bits = 23;
};

struct ieee_float_desc_dp {
    using repr_type = u64;
    inline static constexpr repr_type exponent_bits = 11;
    inline static constexpr repr_type mantissa_bits = 52;
};

template<typename Desc>
struct ieee_soft_float : ieee_esque_float<ieee_soft_float<Desc>> {
    using repr_type = typename Desc::repr_type;

    constexpr ieee_soft_float() noexcept = default;
    constexpr ieee_soft_float(repr_type v) noexcept : m_repr(v) {}

    constexpr static auto from_repr(repr_type v) -> ieee_soft_float { return {v}; }
    constexpr auto get_repr() const -> repr_type { return m_repr; }

    constexpr auto sign_bit() const -> bool { return get_bit(m_repr, m_exponent_bits + m_mantissa_bits); }

    constexpr auto trunc() -> ieee_soft_float&;
    constexpr auto inverse_trunc() -> ieee_soft_float&;
    constexpr auto round() -> ieee_soft_float&;

    constexpr auto ceil() -> ieee_soft_float& { return sign_bit() ? trunc() : inverse_trunc(); }
    constexpr auto floor() -> ieee_soft_float& { return sign_bit() ? inverse_trunc() : trunc(); }

private:
    friend struct ieee_esque_float<ieee_soft_float<Desc>>;

    inline static constexpr repr_type m_exponent_bits = Desc::exponent_bits;
    inline static constexpr repr_type m_exponent_mask = ((repr_type)1 << m_exponent_bits) - 1;
    inline static constexpr repr_type m_mantissa_bits = Desc::mantissa_bits;
    inline static constexpr repr_type m_mantissa_mask = ((repr_type)1 << m_mantissa_bits) - 1;
    inline static constexpr repr_type m_payload_mask = ((repr_type)1 << (m_exponent_bits + m_mantissa_bits)) - 1;

    repr_type m_repr;

    constexpr void set_sign(bool sign) { m_repr = set_bit(m_repr, m_exponent_bits + m_mantissa_bits, sign); }
    constexpr auto exponent() const -> repr_type { return (m_repr >> m_mantissa_bits) & m_exponent_mask; }
    constexpr auto mantissa() const -> repr_type { return m_repr & m_mantissa_mask; }
    constexpr auto fraction() const -> repr_type { return mantissa(); }

    constexpr auto set_to_zero(bool preserve_sign) -> ieee_soft_float& {
        if (preserve_sign) {
            m_repr &= ~m_payload_mask;
        } else {
            m_repr = 0;
        }
        return *this;
    }

    constexpr auto set_to_pow_2(int power, bool preserve_sign) -> ieee_soft_float& {
        int exponent = power + m_exponent_mask / 2;

        if (exponent >= m_exponent_mask || exponent < 0) {
            std::unreachable();
        }

        m_repr &= ~m_payload_mask;
        m_repr |= static_cast<repr_type>(exponent) << m_mantissa_bits;

        return *this;
    }

    constexpr static auto from_parts(bool sign, repr_type exponent, repr_type mantissa) {
        ieee_soft_float ret{};
        ret.m_repr |= exponent;
        ret.m_repr <<= m_mantissa_bits;
        ret.m_repr |= mantissa;
        ret.set_sign(sign);
        return ret;
    }
};

template<typename Desc>
constexpr auto operator<=>(ieee_soft_float<Desc> lhs, ieee_soft_float<Desc> rhs) -> std::partial_ordering {
    return ieee_esque_float<ieee_soft_float<Desc>>::order(lhs, rhs);
}

static_assert(concepts::soft_float<ieee_soft_float<ieee_float_desc_dp>>);

}  // namespace stf::cemath::detail

namespace stf::cemath {

template<>
struct native_float<float> : std::type_identity<detail::ieee_soft_float<detail::ieee_float_desc_sp>> {};

template<>
struct native_float<double> : std::type_identity<detail::ieee_soft_float<detail::ieee_float_desc_dp>> {};

}  // namespace stf::cemath

#include <stuff/cemath/detail/soft_float/ieee/round.ipp>
#include <stuff/cemath/detail/soft_float/ieee/trunc.ipp>
