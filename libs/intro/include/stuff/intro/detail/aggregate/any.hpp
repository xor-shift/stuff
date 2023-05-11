#pragma once

#include <stuff/core/integers.hpp>

namespace stf::intro::detail {

template<usize = 0uz>
struct convertible_to_anything {
    template<typename T>
        requires(!std::is_aggregate_v<std::remove_cvref_t<T>> || std::is_array_v<std::remove_cvref_t<T>>)
    constexpr operator T&&() && noexcept;

    template<typename T>
        requires(!std::is_aggregate_v<std::remove_cvref_t<T>> || std::is_array_v<std::remove_cvref_t<T>>)
    constexpr operator T&() const& noexcept;
};

}  // namespace stf::intro::detail
