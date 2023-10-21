#pragma once

#include <stuff/core/integers.hpp>

namespace stf::intro::detail {

template<usize = 0uz>
struct convertible_to_anything {
#pragma GCC diagnostic push

#ifndef __GNUC__
#pragma GCC diagnostic ignored "-Wundefined-inline"
#endif

    template<typename T>
        requires(!std::is_aggregate_v<std::remove_cvref_t<T>> || std::is_array_v<std::remove_cvref_t<T>>)
    constexpr operator T&&() && noexcept;

    template<typename T>
        requires(!std::is_aggregate_v<std::remove_cvref_t<T>> || std::is_array_v<std::remove_cvref_t<T>>)
    constexpr operator T&() const& noexcept;

#pragma GCC diagnostic pop
};

}  // namespace stf::intro::detail
