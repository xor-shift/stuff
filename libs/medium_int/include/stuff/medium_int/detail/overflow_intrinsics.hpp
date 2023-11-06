#pragma once

#include <concepts>

namespace stf::detail {

template<std::unsigned_integral T>
struct intrinsic_add_sub_overflow;

// just use the fact that + promotes to int for things below this
template<>
struct intrinsic_add_sub_overflow<unsigned> {
    static auto add_overflow(unsigned a, unsigned b, unsigned& result) -> bool { return __builtin_uadd_overflow(a, b, &result); }
    static auto sub_overflow(unsigned a, unsigned b, unsigned& result) -> bool { return __builtin_usub_overflow(a, b, &result); }
};

template<>
struct intrinsic_add_sub_overflow<unsigned long> {
    static auto add_overflow(unsigned long a, unsigned long b, unsigned long& result) -> bool { return __builtin_uaddl_overflow(a, b, &result); }
    static auto sub_overflow(unsigned long a, unsigned long b, unsigned long& result) -> bool { return __builtin_usubl_overflow(a, b, &result); }
};

template<>
struct intrinsic_add_sub_overflow<unsigned long long> {
    static auto add_overflow(unsigned long long a, unsigned long long b, unsigned long long& result) -> bool { return __builtin_uaddll_overflow(a, b, &result); }
    static auto sub_overflow(unsigned long long a, unsigned long long b, unsigned long long& result) -> bool { return __builtin_usubll_overflow(a, b, &result); }
};

}  // namespace stf::detail
