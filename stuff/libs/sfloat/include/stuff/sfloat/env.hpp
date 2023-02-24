#pragma once

#include <cfenv>
#include <functional>

namespace stf::sfloat {

enum class rounding : int {
    downward = FE_DOWNWARD,
    to_nearest = FE_TONEAREST,
    toward_zero = FE_TOWARDZERO,
    upward = FE_UPWARD,
};

enum class exception : int {
    div_by_zero = FE_DIVBYZERO,
    inexact = FE_INEXACT,
    invalid = FE_INVALID,
    overflow = FE_OVERFLOW,
    underflow = FE_UNDERFLOW,
};

struct fenv {
    rounding rounding_mode = rounding();
    int exceptions;

    constexpr void raise_except(exception e) { exceptions |= static_cast<int>(e); }
    constexpr bool test_except(exception e) const { return (exceptions & static_cast<int>(e)) != 0; }
    constexpr void clear_except(exception e) { exceptions &= ~static_cast<int>(e); }

    void inject_to_std_fenv() {
#pragma STDC FENV_ACCESS ON
        std::fesetround(static_cast<int>(rounding_mode));

#pragma push_macro("UPDATE_EXCEPT")
#define UPDATE_EXCEPT(_e) (test_except(_e) ? std::feraiseexcept : std::feclearexcept)(static_cast<int>(_e))
        UPDATE_EXCEPT(exception::div_by_zero);
        UPDATE_EXCEPT(exception::inexact);
        UPDATE_EXCEPT(exception::invalid);
        UPDATE_EXCEPT(exception::overflow);
        UPDATE_EXCEPT(exception::underflow);
#undef UPDATE_EXCEPT
#pragma pop_macro("UPDATE_EXCEPT")
    }

    void gather_from_std_fenv() {
#pragma STDC FENV_ACCESS ON
        exceptions = std::fegetround();

#pragma push_macro("UPDATE_EXCEPT")
#define UPDATE_EXCEPT(_e) \
    (this->*(std::fetestexcept(static_cast<int>(_e)) ? &fenv::raise_except : &fenv::clear_except))(_e)
        UPDATE_EXCEPT(exception::div_by_zero);
        UPDATE_EXCEPT(exception::inexact);
        UPDATE_EXCEPT(exception::invalid);
        UPDATE_EXCEPT(exception::overflow);
        UPDATE_EXCEPT(exception::underflow);
#undef UPDATE_EXCEPT
#pragma pop_macro("UPDATE_EXCEPT")
    }

    template<typename Func>
    constexpr auto do_guarded_hard_fp_op(Func&& fn) {
        std::fenv_t old_fenv;
        if !consteval {
            std::fegetenv(&old_fenv);
            inject_to_std_fenv();
        }

        auto res = std::invoke(std::forward<Func>(fn));

        if !consteval {
            gather_from_std_fenv();
            std::fesetenv(&old_fenv);
        }

        return res;
    }
};

}  // namespace stf::sfloat
