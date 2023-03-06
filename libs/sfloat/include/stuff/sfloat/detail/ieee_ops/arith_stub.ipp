#pragma once

namespace stf::sfloat::ieee {

namespace detail {

template<concepts::ieee_float_description Desc>
struct stub_arith_helper;

template<>
struct stub_arith_helper<single_precision> {
    using type = float;
};

template<>
struct stub_arith_helper<double_precision> {
    using type = double;
};

template<concepts::ieee_float_description Desc, typename Op>
constexpr auto stub_arith_impl(soft_float<Desc> lhs, soft_float<Desc> rhs, fenv& env, Op&& op) -> soft_float<Desc> {
    using hard_float_type = typename stub_arith_helper<Desc>::type;

    auto f_lhs = std::bit_cast<hard_float_type>(lhs.repr);
    auto f_rhs = std::bit_cast<hard_float_type>(rhs.repr);

    // update flags if we're not consteval-ing
    hard_float_type f_res = env.template do_guarded_hard_fp_op([=, op = std::forward<Op>(op)] mutable {
        return std::invoke(std::forward<Op>(op), f_lhs, f_rhs);
    });

    // if we're consteval-ing, update the bare minimum of flags
    if consteval {
        // TODO: do the do
    }

    return soft_float<Desc>(std::bit_cast<typename Desc::repr_type>(f_res));
}

}  // namespace detail

// placeholder function, will not update fenv properly
template<concepts::ieee_float_description Desc>
constexpr auto add(soft_float<Desc> lhs, soft_float<Desc> rhs, fenv& env) -> soft_float<Desc> {
    return detail::stub_arith_impl(lhs, rhs, env, std::plus<>{});
}

template<concepts::ieee_float_description Desc>
constexpr auto add(soft_float<Desc> lhs, soft_float<Desc> rhs) -> soft_float<Desc> {
    return STF_SFLOAT_ENVLESS_CALL(add, lhs, rhs);
}

// placeholder function, will not update fenv properly
template<concepts::ieee_float_description Desc>
constexpr auto sub(soft_float<Desc> lhs, soft_float<Desc> rhs, fenv& env) -> soft_float<Desc> {
    return detail::stub_arith_impl(lhs, rhs, env, std::minus<>{});
}

template<concepts::ieee_float_description Desc>
constexpr auto sub(soft_float<Desc> lhs, soft_float<Desc> rhs) -> soft_float<Desc> {
    return STF_SFLOAT_ENVLESS_CALL(sub, lhs, rhs);
}

// placeholder function, will not update fenv properly
template<concepts::ieee_float_description Desc>
constexpr auto mul(soft_float<Desc> lhs, soft_float<Desc> rhs, fenv& env) -> soft_float<Desc> {
    return detail::stub_arith_impl(lhs, rhs, env, std::multiplies<>{});
}

template<concepts::ieee_float_description Desc>
constexpr auto mul(soft_float<Desc> lhs, soft_float<Desc> rhs) -> soft_float<Desc> {
    return STF_SFLOAT_ENVLESS_CALL(mul, lhs, rhs);
}

// placeholder function, will not update fenv properly
template<concepts::ieee_float_description Desc>
constexpr auto div(soft_float<Desc> lhs, soft_float<Desc> rhs, fenv& env) -> soft_float<Desc> {
    if consteval {
        if ((rhs.repr << 1) == 0) {
            env.raise_except(exception::div_by_zero);
        }
    }

    return detail::stub_arith_impl(lhs, rhs, env, std::divides<>{});
}

template<concepts::ieee_float_description Desc>
constexpr auto div(soft_float<Desc> lhs, soft_float<Desc> rhs) -> soft_float<Desc> {
    return STF_SFLOAT_ENVLESS_CALL(div, lhs, rhs);
}

}  // namespace stf::sfloat::ieee
