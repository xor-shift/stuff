#pragma once

#include <stuff/core/integers.hpp>

#include <algorithm>

namespace stf {

template<usize Size>
struct string_literal {
    char data[Size];

    constexpr string_literal() = default;
    constexpr string_literal(const char (&v)[Size]) noexcept { std::copy(std::begin(v), std::end(v), data); }

    constexpr static auto size() -> usize { return Size - 1; }

    constexpr auto begin() & -> char* { return &data[0]; }
    constexpr auto end() & -> char* { return &data[size()]; }
    constexpr auto begin() const& -> const char* { return &data[0]; }
    constexpr auto end() const& -> const char* { return &data[size()]; }

    constexpr auto c_str() const& -> const char* { return data; }

    template<usize OtherSize>
    friend constexpr auto operator==(string_literal const& self, string_literal<OtherSize> const& other) -> bool {
        if constexpr (OtherSize != Size) {
            return false;
        }

        for (usize i = 0; i < Size; i++) {
            if (other.data[i] != self.data[i]) {
                return false;
            }
        }

        return true;
    }

    template<usize Start, usize Len>
        requires(Start < size())
    constexpr auto substr() const -> string_literal<Len + 1> {
        string_literal<Len + 1> ret{};
        std::copy_n(data, Len, ret.data);
        ret.data[Len] = '\0';

        return ret;
    }
};

}
