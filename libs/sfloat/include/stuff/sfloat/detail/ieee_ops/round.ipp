#pragma once

namespace stf::sfloat::ieee {

template<concepts::ieee_float_description Desc>
constexpr auto round(soft_float<Desc> v, fenv&& env = fenv()) -> soft_float<Desc> {
    using signed_type = std::make_signed_t<typename Desc::repr_type>;
    using unsigned_type = typename Desc::repr_type;

    signed_type real_exponent = v.real_exponent();

    if (real_exponent == static_cast<signed_type>(v.fraction_mask / 2)) {  // inf or nan
        return add(v, v, std::move(env));                                  // raise exceptions
    }

    if (real_exponent >= static_cast<signed_type>(v.fraction_bits)) {  // definitely an integer
        return v;
    }

    if (bool mantissa_decides = real_exponent >= 0; !mantissa_decides) {  // 1 > |val|
        if (real_exponent == -1) {                                        // 1 > |val| >= 0.5
            return v.set_to_pow_2(0, true);                               // ±1
        }

        // 0.5 > |val|
        return v.set_to_zero(true);
    }

    unsigned_type fraction_mask = v.mantissa_mask >> real_exponent;
    if (bool fractional = (v.repr & fraction_mask) != 0; !fractional) {
        return v;
    }

    int deciding_bit = v.mantissa_bits - real_exponent - 1;
    unsigned_type increment = static_cast<unsigned_type>(1) << deciding_bit;
    v.repr = (v.repr + increment) & ~fraction_mask;

    return v;
}

template<concepts::ieee_float_description Desc>
constexpr auto round_even(soft_float<Desc> v, fenv&& env = fenv()) -> soft_float<Desc> {
    // stub

    return v;
}

template<concepts::ieee_float_description Desc>
constexpr auto rint(soft_float<Desc> v, fenv&& env = fenv()) -> soft_float<Desc> {
    switch (env.rounding_mode) {
        case rounding::downward: return floor(v, std::move(env));
        case rounding::upward: return ceil(v, std::move(env));
        case rounding::to_nearest: return round_even(v, std::move(env));
        case rounding::toward_zero: return trunc(v, std::move(env));
        default: std::unreachable();
    }
}

template<std::signed_integral T, concepts::ieee_float_description Desc>
constexpr auto lrint(soft_float<Desc> v, fenv&& env = fenv()) -> T {
    if (is_infinite(v) || is_nan(v)) {
        env.raise_except(exception::invalid);
        return (v.sign_bit() ? std::numeric_limits<T>::min : std::numeric_limits<T>::max)();
    }

    soft_float<Desc> res = rint(v, std::move(env));

    std::signed_integral auto exponent = res.real_exponent();
    // the integer result is in [2^e,  2^(e+1))

    if (exponent < 0) {
        return T{0};
    }

    int max_integral_exp = std::numeric_limits<T>::digits;
    // can store from -2^e to (2^e - 1), inclusive
    // assuming -(2^e - 1) to be the lower bound, FIXME
    // assuming e = 63 (int64_t) for explanations

    if (exponent >= std::numeric_limits<T>::digits) {
        env.raise_except(exception::inexact);
        return (res.sign_bit() ? std::numeric_limits<T>::min : std::numeric_limits<T>::max)();
    }

    // e is at most 62 now

    T base = T{1} << exponent;
}

}  // namespace stf::sfloat::ieee
