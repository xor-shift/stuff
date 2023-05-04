#pragma once

#include <stuff/core/integers.hpp>

namespace stf::intro::detail::agg {

template<usize = 0uz>
struct convertible_to_anything {
    constexpr convertible_to_anything() = default;

    template<typename T>
    constexpr operator T&() const noexcept;

    template<typename T>
    constexpr operator T&&() const noexcept;
};

}
