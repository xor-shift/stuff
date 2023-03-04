#pragma once

#include <stuff/bit.hpp>
#include <stuff/core/integers.hpp>

namespace stf::qoi {

namespace detail {

// BE: RG LE: GR
static constexpr std::array<u8, 65536> hash_table_0 = ([] {
    std::array<u8, 65536> ret;

    for (int r = 0; r <= 255; r++) {
        for (int g = 0; g <= 255; g++) {
            int idx = std::endian::native == std::endian::big ? r * 256 + g : g * 256 + r;
            ret[idx] = r * 3 + g * 5;
        }
    }

    return ret;
})();

// BE: BA LE: AB
static constexpr std::array<u8, 65536> hash_table_1 = ([] {
    std::array<u8, 65536> ret;

    for (int b = 0; b <= 255; b++) {
        for (int a = 0; a <= 255; a++) {
            int idx = std::endian::native == std::endian::big ? b * 256 + a : a * 256 + b;
            ret[idx] = b * 7 + a * 11;
        }
    }

    return ret;
})();

static constexpr std::array<int, 64> diff_hash_delta_table = ([] {
    // notice that the hash function is linear

    std::array<int, 64> ret;

    for (int i = 0; i < 64; i++) {
        int dr = ((i / 16) % 4) - 2;
        int dg = ((i / 4) % 4) - 2;
        int db = ((i / 1) % 4) - 2;

        ret[i] = dr * 3 + dg * 5 + db * 7;
    }

    return ret;
})();

static constexpr std::array<int, 256 * 64> luma_hash_delta_table = ([] {
    std::array<int, 256 * 64> ret;

    for (int i = 0; i < 256 * 64; i++) {
        int dg = ((i / 256) % 64) - 32;

        int drdg = ((i / 16) % 16) - 8;
        int dbdg = ((i / 1) % 16) - 8;

        ret[i] = drdg * 3 + dg * 5 * 3 + dbdg * 7;
    }

    return ret;
})();

}  // namespace detail

struct color {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 255;

    constexpr auto value() const -> u32 {
        auto v = std::bit_cast<u32>(*this);
        return bit::convert_endian(v, std::endian::native, std::endian::big);
    }

    constexpr auto hash_slow() const -> u8 { return r * 3 + g * 5 + b * 7 + a * 11; }

    constexpr auto hash_lookup() const -> u8 {
        u32 t = std::bit_cast<u32>(*this);

        u32 idx_0 = t >> 16;     // LE: AB BE: RG
        u32 idx_1 = t & 0xFFFF;  // LE: GR BE: BA

        u8 hash_0 = (std::endian::native == std::endian::little ? detail::hash_table_1 : detail::hash_table_0)[idx_0];
        u8 hash_1 = (std::endian::native == std::endian::little ? detail::hash_table_0 : detail::hash_table_1)[idx_1];

        return hash_0 + hash_1;
    }

    constexpr auto hash_fast() const -> u8 {
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

        return (v * multiplier) >> 56;
    }

    constexpr auto hash() const -> u8 { return hash_fast(); }

    static constexpr auto hash_delta(u8 delta) -> int {
        return detail::diff_hash_delta_table[delta];
    }
    static constexpr auto hash_luma(u8 lead, u8 follow) -> int {
        return detail::luma_hash_delta_table[(((int)lead) << 8) | follow];
    }

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
