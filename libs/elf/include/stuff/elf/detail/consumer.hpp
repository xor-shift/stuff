#pragma once

#include <stuff/bit.hpp>
#include <stuff/core.hpp>

#include <span>

namespace stf::elf::detail {

struct data_consumer {
    std::span<const u8> bytes;
    usize read_head;

    bool is_64;
    std::endian endianness;

    template<usize N>
    constexpr auto consume_bytes(std::integral_constant<usize, N> = {}) -> std::array<u8, N> {
        auto buf = std::array<u8, N>{};
        std::copy_n(bytes.begin() + (read_head += N) - N, N, buf.begin());
        return buf;
    };

    template<std::unsigned_integral T>
    constexpr auto consume(std::type_identity<T> = {}) -> T {
        alignas(alignof(T)) const auto buf = consume_bytes(std::integral_constant<usize, sizeof(T)>{});
        return stf::bit::convert_endian(std::bit_cast<T>(buf), endianness, std::endian::native);
    };

    constexpr auto consume_address() -> u64 {
        u64 v;
        if (is_64) {
            v = consume(std::type_identity<u64>{});
        } else {
            v = consume(std::type_identity<u32>{});
        }
        return v;
    };
};

}  // namespace detail