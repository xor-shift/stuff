#pragma once

#include <stuff/medium_int/detail/constexpr_overflow.hpp>

#include <stuff/core.hpp>

#include <concepts>

namespace stf::detail {

template<std::unsigned_integral T>
struct carry_context {
    using impl_type = stf::detail::add_sub_overflow<T>;

    constexpr auto addc(T lhs, T rhs) -> T {
        auto result = T{};

        const auto carry = impl_type::add_overflow(lhs, rhs, result);
        result += m_carry;

        m_carry = carry;

        return result;
    }

private:
    bool m_carry = 0;
};

}  // namespace stf::detail
