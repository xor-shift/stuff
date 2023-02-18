#pragma once

namespace stf::cemath {

#pragma push_macro("FACTORY")
#define FACTORY(_name)                                                           \
    template<std::floating_point T>                                              \
    constexpr auto _name(T v)->T {                                               \
        if consteval {                                                           \
            return std::bit_cast<T>(native_to_soft_float(v)._name().get_repr()); \
        } else {                                                                 \
            return std::_name(v);                                                \
        }                                                                        \
    }                                                                            \
                                                                                 \
    constexpr auto _name##f(float v)->float { return _name(v); }                 \
    constexpr auto _name##l(long double v)->long double { return _name(v); }     \
                                                                                 \
    template<std::integral T>                                                    \
    constexpr auto _name(T v)->double {                                          \
        return _name(static_cast<double>(v));                                    \
    }

FACTORY(ceil)
FACTORY(floor)
FACTORY(trunc)

#undef FACTORY
#pragma pop_macro("FACTORY")

}  // namespace stf::cemath
