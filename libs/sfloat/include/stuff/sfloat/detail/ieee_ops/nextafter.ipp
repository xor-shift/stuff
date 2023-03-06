#pragma once

namespace stf::sfloat::ieee {

template<concepts::ieee_float_description Desc>
constexpr auto nextafter(soft_float<Desc> from, soft_float<Desc> to, fenv& env) -> soft_float<Desc> {
    if (is_nan(from) || is_nan(to)) {
        return from;
    }

    if (from.repr == to.repr) {
        return from;
    }

    soft_float<Desc> abs_from(from.repr & (from.shifted_exponent_mask | from.mantissa_mask));
    soft_float<Desc> abs_to(to.repr & (to.shifted_exponent_mask | to.mantissa_mask));

    if (abs_from.repr == 0) {
        env.raise_except(exception::underflow);
        env.raise_except(exception::inexact);

        return copysign(soft_float<Desc>(1), to);
    }

    // assumptions for clarity
    assume(from != to);          // from is not to
    assume(abs_from.repr != 0);  // from is not ±0

    soft_float<Desc> ret(from);

    if (from.sign_bit() == to.sign_bit()) {
        ret.repr += abs_to.repr > abs_from.repr ? 1 : -1;
    } else {
        ret.repr += !to.sign_bit() ? 1 : -1;
    }

    // assumptions cont'd
    assume(!is_nan(ret));  // if from is ±inf, it won't get bumped into a nan (from would have to eq to)

    classification cls = classify(ret);

    if (cls == classification::subnormal || cls == classification::zero) {
        env.raise_except(exception::underflow);
        env.raise_except(exception::inexact);
    } else if (cls == classification::infinite) {
        env.raise_except(exception::overflow);
        env.raise_except(exception::inexact);
    }

    return ret;
}

template<concepts::ieee_float_description Desc>
constexpr auto nextafter(soft_float<Desc> from, soft_float<Desc> to) -> soft_float<Desc> {
    fenv env {};
    return nextafter(from, to, env);
}

}  // namespace stf::sfloat::ieee
