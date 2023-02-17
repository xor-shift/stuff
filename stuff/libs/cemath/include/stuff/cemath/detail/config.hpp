#pragma once

#include <stuff/core.hpp>

#include <concepts>
#include <type_traits>

namespace stf::cemath::detail::config {

inline static constexpr bool intmax_is_unique =  //
  !std::is_same_v<::std::intmax_t, char> &&      //
  !std::is_same_v<::std::intmax_t, short> &&     //
  !std::is_same_v<::std::intmax_t, int> &&       //
  !std::is_same_v<::std::intmax_t, long> &&      //
  !std::is_same_v<::std::intmax_t, long long>;

template<typename T>
consteval auto have_cexpr_thing_helper(T) -> bool {
    return true;
}

#define HAVE_CEXPR_THING(_thing, ...) requires { have_cexpr_thing_helper(_thing(__VA_ARGS__)); }

inline static constexpr bool have_builtin_fabsf = STF_FUNC_EXISTS(__builtin_fabsf, 0.F);
inline static constexpr bool have_constexpr_fabsf = HAVE_CEXPR_THING(__builtin_fabsf, 0.F);
inline static constexpr bool have_builtin_fabs = STF_FUNC_EXISTS(__builtin_fabs, 0.);
inline static constexpr bool have_constexpr_fabs = HAVE_CEXPR_THING(__builtin_fabs, 0.);
inline static constexpr bool have_builtin_fabsl = STF_FUNC_EXISTS(__builtin_fabsl, 0.L);
inline static constexpr bool have_constexpr_fabsl = HAVE_CEXPR_THING(__builtin_fabsl, 0.L);

#undef HAVE_CEXPR_THING

}  // namespace stf::cemath::detail
