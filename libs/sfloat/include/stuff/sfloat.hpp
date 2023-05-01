#pragma once

#include <stuff/core/integers.hpp>
#include <stuff/sfloat/ieee.hpp>
#include <stuff/sfloat/soft_float.hpp>

#include <concepts>

namespace stf::sfloat {

template<std::floating_point T>
struct native_float;

template<std::floating_point T>
using native_float_t = typename native_float<T>::type;

template<std::floating_point T>
constexpr auto make_soft(T v) -> concepts::soft_float auto{
    using sf_type = native_float_t<T>;

    auto temp = std::bit_cast<std::array<unsigned char, sizeof(T)>>(v);

    if constexpr (std::endian::native == std::endian::big) {
        std::fill_n(temp.begin(), sf_type::padding_bytes, 0);
    } else {
        std::fill_n(temp.rbegin(), sf_type::padding_bytes, 0);
    }

    return sf_type(std::bit_cast<typename sf_type::repr_type>(temp));
}

#pragma push_macro("SFLOAT_STDCALL_UNARY")
#define SFLOAT_STDCALL_UNARY(_name)                            \
    namespace detail {                                         \
                                                               \
    struct _name##_fn {                                        \
        template<std::floating_point T>                        \
        constexpr auto operator()(T v) const -> T {            \
            if !consteval {                                    \
                return std::_name(v);                          \
            } else {                                           \
                concepts::soft_float auto soft = make_soft(v); \
                concepts::soft_float auto res = _name(soft);   \
                return std::bit_cast<T>(res);                  \
            }                                                  \
        }                                                      \
                                                               \
        template<std::integral T>                              \
        constexpr auto operator()(T v) const -> double {       \
            return (*this)(static_cast<double>(v));            \
        }                                                      \
    };                                                         \
    }                                                          \
                                                               \
    inline static constexpr detail::_name##_fn _name = detail::_name##_fn{};

SFLOAT_STDCALL_UNARY(abs);
SFLOAT_STDCALL_UNARY(ceil);
SFLOAT_STDCALL_UNARY(floor);
SFLOAT_STDCALL_UNARY(round);
SFLOAT_STDCALL_UNARY(trunc);

#undef SFLOAT_STDCALL_UNARY
#pragma pop_macro("SFLOAT_STDCALL_UNARY")

}  // namespace stf::sfloat
