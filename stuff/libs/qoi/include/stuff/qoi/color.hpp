#pragma once

#include <stuff/bit.hpp>
#include <stuff/core/integers.hpp>

namespace stf::qoi {

struct color {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 255;

    constexpr auto value() const -> u32 {
        auto v = std::bit_cast<u32>(*this);
        return bit::convert_endian(v, std::endian::native, std::endian::big);
    }

    constexpr auto slow_hash() const -> u8 {
        return (r * 3 + g * 5 + b * 7 + a * 11) % 64;
    }

    constexpr auto fast_hash() const -> u8 {
        // this function is reverse-engineered from:
        // https://github.com/zakarumych/rapid-qoi/blob/c5359a53476001d8d170c3733e6ab22e8173f40f/src/lib.rs#L337

        /*
         *                   0300'0700'0005'000B
         *                   AA00'GG00'00BB'00RR
         *                   ------------------- val shift
         *                   RR00'RR00'00RR'00RR 3   0
         *              BB00'BB00'00BB'00BB      7   16 (+16)
         *      GG'00GG'0000'GG00'GG             5   40 (+24)
         * AA'00AA'0000'AA00'AA                  11  56 (+16)
         *                   ^^
         *
         * val is the multiplier affecting the upper nibble
         */

        u64 v = std::bit_cast<u32>(*this);
        v |= v << 32;
        v &= 0xFF00'FF00'00FF'00FF;
        // LE: AA00'GG00'00BB'00RR
        // BE: RR00'BB00'00GG'00AA

        u64 multiplier;

        if constexpr (std::endian::native == std::endian::little) {
            multiplier = 0x0300'0700'0005'000Bull;
        } else {
            multiplier = 0x0B00'0500'0007'0003ull;
        }

        return ((v * multiplier) >> 56) % 64;
    }

    constexpr auto hash() const -> u8 { return fast_hash(); }

    friend constexpr auto operator==(color lhs, color rhs) -> bool {
        return std::bit_cast<u32>(lhs) == std::bit_cast<u32>(rhs);
    }

    constexpr auto diff(u8 dr, u8 dg, u8 db) -> color {
        auto ret = *this;
        ret.r = ret.r + (dr - 2);
        ret.g = ret.g + (dg - 2);
        ret.b = ret.b + (db - 2);
        return ret;
    }

    constexpr auto luma(u8 dg, u8 drdg, u8 dbdg) -> color {
        int real_dg = dg - 32;
        int real_dr = (drdg - 8) + real_dg;
        int real_db = (dbdg - 8) + real_dg;

        auto ret = *this;
        ret.r = ret.r + real_dr;
        ret.g = ret.g + real_dg;
        ret.b = ret.b + real_db;
        return ret;
    }
};

}  // namespace stf::qoi
