#pragma once

namespace stf::cemath {

template<std::floating_point T>
constexpr auto fpclassify(T v) -> int {
    if consteval {
        switch (native_to_soft_float(v).classify()) {
            case classification::infinite: return FP_INFINITE;
            case classification::nan: return FP_NAN;
            case classification::normal: return FP_NORMAL;
            case classification::subnormal: return FP_SUBNORMAL;
            case classification::zero: return FP_ZERO;
            default: std::unreachable();
        }
    } else {
        return std::fpclassify(v);
    }
}

template<std::integral T>
constexpr auto fpclassify(T v) -> int {
    return fpclassify(static_cast<double>(v));
}

#pragma push_macro("AUX_CLASS_FACTORY")
#define AUX_CLASS_FACTORY(_name, _predicate)                \
    template<typename T>                                    \
        requires std::integral<T> || std::floating_point<T> \
    constexpr auto _name(T v) {                             \
        int res = fpclassify(v);                            \
        return _predicate;                                  \
    }

AUX_CLASS_FACTORY(isfinite, res != FP_INFINITE);
AUX_CLASS_FACTORY(isinf, res == FP_INFINITE);
AUX_CLASS_FACTORY(isnan, res == FP_NAN);
AUX_CLASS_FACTORY(isnormal, res == FP_NORMAL);

#undef AUX_CLASS_FACTORY
#pragma pop_macro("AUX_CLASS_FACTORY")

#pragma push_macro("ORDERING_FACTORY")
#define ORDERING_FACTORY(_name, _predicate)                                                       \
    template<std::floating_point T>                                                               \
    constexpr auto _name(T x, T y)->bool {                                                        \
        if consteval {                                                                            \
            std::partial_ordering ordering = native_to_soft_float(x) <=> native_to_soft_float(y); \
            return _predicate;                                                                    \
        } else {                                                                                  \
            return std::_name(x, y);                                                              \
        }                                                                                         \
    }                                                                                             \
                                                                                                  \
    template<std::integral T>                                                                     \
    constexpr auto _name(T x, T y)->bool {                                                        \
        return _name(static_cast<double>(x), static_cast<double>(y));                             \
    }

ORDERING_FACTORY(isgreater, ordering == std::partial_ordering::greater)
ORDERING_FACTORY(
  isgreaterequal,
  ordering == std::partial_ordering::greater || ordering == std::partial_ordering::equivalent
)
ORDERING_FACTORY(isless, ordering == std::partial_ordering::less)
ORDERING_FACTORY(islessequal, ordering == std::partial_ordering::less || ordering == std::partial_ordering::equivalent)
ORDERING_FACTORY(islessgreater, ordering == std::partial_ordering::less || ordering == std::partial_ordering::greater)
ORDERING_FACTORY(isunordered, ordering == std::partial_ordering::unordered)

#undef ORDERING_FACTORY
#pragma pop_macro("ORDERING_FACTORY")

}  // namespace stf::cemath
