#pragma once

#include <stuff/medium_int/detail/overflow_intrinsics.hpp>

#include <limits>

namespace stf::detail {

template<std::unsigned_integral T>
struct add_sub_overflow;

template<std::unsigned_integral T>
    requires (sizeof(T) >= sizeof(unsigned))
struct add_sub_overflow<T> {
    static constexpr auto add_overflow(T a, T b, T& result) -> bool {
        if consteval {
            const std::unsigned_integral /* guaranteed, see requirement */ auto res_temp = a + b;

            /* 1111 + 1111 = 1110 (res_temp) (overflow)
             * 1110 - 1111 = 0001 (res_temp - a) (underflow)
             */

            const auto carry_out = ((res_temp - a) != b || (res_temp - b) != a);

            result = res_temp;
            return carry_out;
        } else {
            return intrinsic_add_sub_overflow<T>::add_overflow(a, b, result);
        }
    }

    static constexpr auto sub_overflow(T a, T b, T& result) -> bool {
        if consteval {
            const std::unsigned_integral auto res_temp = a - b;

            /* 0000 - 1111 = 0001 (res_temp) (overflow)
             * 0001 + 0000 = 0001 (res_temp - a)
             * 0001 + 1111 = 1110 (res_temp - b) (underflow)
             */

            const auto borrow_out = ((res_temp + a) != b || (res_temp + b) != a);

            result = res_temp;
            return borrow_out;
        } else {
            return intrinsic_add_sub_overflow<T>::sub_overflow(a, b, result);
        }
    }
};

template<std::unsigned_integral T>
    requires (sizeof(T) < sizeof(unsigned))
struct add_sub_overflow<T> {
    static constexpr auto add_overflow(T a, T b, T& result) -> bool {
        const std::signed_integral auto v = a + b;
        result = static_cast<T>(v);
        return v > std::numeric_limits<T>::max();
    }

    static constexpr auto sub_overflow(T a, T b, T& result) -> bool {
        const std::signed_integral auto v = a - b;
        result = static_cast<T>(v);
        return v < 0;
    }
};

}
