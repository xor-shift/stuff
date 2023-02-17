#pragma once

namespace stf::cemath::detail {

template<std::floating_point T>
struct float_parts;

template<>
struct float_parts<long double> {
    using value_type = long double;
    using repr_type = u128;

    inline static constexpr repr_type exponent_bits = 15;
    inline static constexpr repr_type mantissa_bits = 64;

    repr_type repr;

    constexpr static auto from_float(value_type v) -> float_parts { return {std::bit_cast<repr_type>(v)}; }
    constexpr auto to_float() const -> value_type { return std::bit_cast<value_type>(repr); }

    constexpr auto sign_bit() const -> bool { return (repr >> 79) != 0; }
    constexpr auto exponent() const -> repr_type { return (repr >> 64) & 0x7FFFull; }
    constexpr auto mantissa() const -> repr_type { return repr & 0xFFFF'FFFF'FFFF'FFFFull; }
    constexpr auto fraction() const -> repr_type { return repr & 0x7FFF'FFFF'FFFF'FFFFull; }
    constexpr auto int_part() const -> bool { return ((repr >> 63) & 1) != 0; }

    constexpr auto classify() const -> classification {
        if (exponent() == 0) {
            return fraction() == 0 ? classification::zero : classification::subnormal;
        }

        if (exponent() == 0x7FFFull) {
            if (mantissa() == 0 || (fraction() == 0 && int_part()))
                return classification::infinite;
            return classification::nan;
        }

        return classification::normal;
    }
};

constexpr auto operator<=>(float_parts<long double> lhs, float_parts<long double> rhs) -> std::partial_ordering {
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

    return invert ? rhs.repr <=> lhs.repr : lhs.repr <=> rhs.repr;
}

}  // namespace stf::cemath::detail
