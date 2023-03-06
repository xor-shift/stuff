#pragma once

#include <stuff/bit.hpp>
#include <stuff/core.hpp>
#include <stuff/sfloat/env.hpp>

#include <concepts>
#include <utility>

namespace stf::sfloat::ieee {

template<concepts::ieee_float_description Desc>
struct soft_float {
    using repr_type = typename Desc::repr_type;
    inline static constexpr usize padding_bytes = Desc::padding_bytes;

    repr_type repr;

    inline static constexpr repr_type fraction_bits = Desc::fraction_bits;
    inline static constexpr repr_type fraction_mask = (static_cast<repr_type>(1) << fraction_bits) - 1;
    inline static constexpr repr_type mantissa_bits = fraction_bits + (Desc::has_integer_bit ? 1 : 0);
    inline static constexpr repr_type mantissa_mask = (static_cast<repr_type>(1) << mantissa_bits) - 1;

    inline static constexpr repr_type exponent_bits = Desc::exponent_bits;
    inline static constexpr repr_type exponent_mask = (static_cast<repr_type>(1) << exponent_bits) - 1;
    inline static constexpr repr_type shifted_exponent_mask = exponent_mask << mantissa_bits;

    constexpr soft_float() = default;
    constexpr soft_float(repr_type v)
        : repr(v) {}

    constexpr soft_float(soft_float const& other) = default;
    constexpr soft_float(soft_float&& other) = default;

    static constexpr auto huge_val() -> soft_float { return {shifted_exponent_mask}; }

    constexpr auto get_repr() const -> repr_type { return repr; }

    constexpr auto sign_bit() const -> bool { return (repr >> (mantissa_bits + exponent_bits)) != 0; }
    constexpr auto exponent() const -> repr_type { return (repr >> mantissa_bits) & exponent_mask; }
    constexpr auto mantissa() const -> repr_type { return repr & mantissa_mask; }
    constexpr auto fraction() const -> repr_type { return mantissa(); }

    constexpr auto real_exponent() const -> std::make_signed_t<repr_type> {
        using signed_repr_type = std::make_signed_t<repr_type>;
        return static_cast<signed_repr_type>(exponent()) - static_cast<signed_repr_type>(exponent_mask / 2);
    }

    constexpr auto set_to_zero(bool preserve_sign) -> soft_float& {
        if (preserve_sign) {
            repr &= ~(mantissa_mask | (exponent_mask << mantissa_bits));
        } else {
            repr = 0;
        }

        return *this;
    }

    constexpr auto set_to_pow_2(int power, bool preserve_sign) -> soft_float& {
        int exponent = power + exponent_mask / 2;

        if (preserve_sign) {
            repr &= ~(mantissa_mask | (exponent_mask << mantissa_bits));
        } else {
            repr = 0;
        }
        repr |= static_cast<repr_type>(exponent) << mantissa_bits;

        return *this;
    }
};

}  // namespace stf::sfloat::ieee

#define STF_SFLOAT_ENVLESS_CALL(_fn, ...) \
    ({                                    \
        fenv env{};                       \
        _fn(__VA_ARGS__, env);            \
    })

#include <stuff/sfloat/detail/ieee_ops/classification.ipp>

#include <stuff/sfloat/detail/ieee_ops/arith.ipp>
#include <stuff/sfloat/detail/ieee_ops/arith_stub.ipp>

#include <stuff/sfloat/detail/ieee_ops/basic.ipp>
#include <stuff/sfloat/detail/ieee_ops/manipulation.ipp>
#include <stuff/sfloat/detail/ieee_ops/nextafter.ipp>
#include <stuff/sfloat/detail/ieee_ops/round.ipp>
#include <stuff/sfloat/detail/ieee_ops/trunc.ipp>
