#include <stuff/medium_int/detail/constexpr_overflow.hpp>

#include <stuff/core.hpp>

#include <gtest/gtest.h>

namespace {

template<std::unsigned_integral T>
constexpr auto add_overflow_wrapped(T a, T b) -> std::pair<T, bool> {
    auto result = T{};
    const auto carry = stf::detail::add_sub_overflow<T>::add_overflow(a, b, result);

    return {result, carry};
}

template<std::unsigned_integral T>
constexpr auto sub_overflow_wrapped(T a, T b) -> std::pair<T, bool> {
    auto result = T{};
    const auto borrow = stf::detail::add_sub_overflow<T>::sub_overflow(a, b, result);

    return {result, borrow};
}

}

// here, "above" or "below" refers to the size of the types being tested
// "above" actually means "above or equal to"
// unsigned, unsigned long, unsigned long long fall into "above", the rest are "below"

TEST(medium_int, carry_above) {
  ASSERT_EQ(add_overflow_wrapped(0u, 0u), std::pair(0u, false));
  ASSERT_EQ(add_overflow_wrapped(0u, 1u), std::pair(1u, false));
  ASSERT_EQ(add_overflow_wrapped(1u, 0u), std::pair(1u, false));
  ASSERT_EQ(add_overflow_wrapped(1u, 1u), std::pair(2u, false));
  ASSERT_EQ(add_overflow_wrapped(0xFFFF'FFFFu, 1u), std::pair(0u, true));
  ASSERT_EQ(add_overflow_wrapped(0xFFFF'FFFFu, 0xFFFF'FFFFu), std::pair(0xFFFF'FFFEu, true));
  ASSERT_EQ(add_overflow_wrapped(0xFFFF'FFFF'FFFF'FFFFull, 1ull), std::pair(0ull, true));
  ASSERT_EQ(add_overflow_wrapped(0xFFFF'FFFF'FFFF'FFFFull, 0xFFFF'FFFF'FFFF'FFFFull), std::pair(0xFFFF'FFFF'FFFF'FFFEull, true));
}

TEST(medium_int, carry_below) {
  ASSERT_EQ(add_overflow_wrapped<u16>(u16{0xFFFFu}, u16{1u}), std::pair(u16{0u}, true));
  ASSERT_EQ(add_overflow_wrapped<u16>(u16{0xFFFFu}, u16{0xFFFFu}), std::pair(u16{0xFFFEu}, true));
  ASSERT_EQ(add_overflow_wrapped<u8>(u8{0xFFu}, u8{1u}), std::pair(u8{0u}, true));
  ASSERT_EQ(add_overflow_wrapped<u8>(u8{0xFFu}, u8{0xFFu}), std::pair(u8{0xFEu}, true));
}

TEST(medium_int, borrow_above) {
  ASSERT_EQ(sub_overflow_wrapped(0u, 0u), std::pair(0u, false));
  ASSERT_EQ(sub_overflow_wrapped(0u, 1u), std::pair(0xFFFF'FFFFu, true));
  ASSERT_EQ(sub_overflow_wrapped(1u, 0u), std::pair(1u, false));
  ASSERT_EQ(sub_overflow_wrapped(1u, 1u), std::pair(0u, false));
  ASSERT_EQ(sub_overflow_wrapped(0ull, 1ull), std::pair(0xFFFF'FFFF'FFFF'FFFFull, true));
}

TEST(medium_int, borrow_below) {
  ASSERT_EQ(sub_overflow_wrapped(u16{0u}, u16{1u}), std::pair(u16{0xFFFFu}, true));
  ASSERT_EQ(sub_overflow_wrapped(u8{0u}, u8{1u}), std::pair(u8{0xFFu}, true));
}
