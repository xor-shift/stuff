#pragma once

#include <stuff/cemath/detail/config.hpp>
#include <stuff/core/integers.hpp>

#include <cinttypes>
#include <concepts>

namespace stf::cemath {

#define ABS_MACRO(_v) ((_v) < 0 ? -(_v) : (_v))
#define ABS_FACTORY(_name, _type) \
    constexpr auto _name(_type arg)->_type { return ABS_MACRO(arg); }

ABS_FACTORY(abs, int)
ABS_FACTORY(abs, long)
ABS_FACTORY(abs, long long)
ABS_FACTORY(abs, float)
ABS_FACTORY(abs, double)
ABS_FACTORY(abs, long double)

ABS_FACTORY(imaxabs, imax)
ABS_FACTORY(labs, long)
ABS_FACTORY(llabs, long long)
ABS_FACTORY(fabsf, float)
ABS_FACTORY(fabs, float)
ABS_FACTORY(fabs, double)
ABS_FACTORY(fabs, long double)
ABS_FACTORY(fabsl, long double)

// TODO: add an `abs` overload that takes in an imax iff. imax config::imax_is_unique

template<std::integral T>
constexpr double fabs(T arg) {
    return fabs(static_cast<double>(arg));
}

#undef ABS_FACTORY
#undef ABS_MACRO

}  // namespace stf::cemath::detail
