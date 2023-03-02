#pragma once

namespace stf::qoi::detail {

namespace encoder {

template<typename It>
struct lossless_encoder {
    constexpr lossless_encoder(It it)
        : m_it(it) {
        std::fill(m_hash_table.begin(), m_hash_table.end(), color{0, 0, 0, 0});
    }

    constexpr void pixel(color c) {
        if (m_prev_color == c) {
            ++m_run_length;

            if (m_run_length == 62) {
                emit_run(m_run_length);
                m_run_length = 0;
            }

            return;
        }

        stf::scope::scope_exit scope_exit{[this, c] {
            m_prev_color = c;
            m_hash_table[c.hash()] = c;
        }};

        if (m_run_length != 0) {
            emit_run(m_run_length);
            m_run_length = 0;
        }

        if (u8 hash = c.hash(); m_hash_table[hash] == c) {
            emit_index(hash);
            return;
        }

        if (c.a == m_prev_color.a && (emit_diff(m_prev_color, c) || emit_luma(m_prev_color, c))) {
            return;
        }

        if (m_prev_color.a == c.a) {
            emit_rgb(c);
            return;
        }

        emit_rgba(c);
    }

    constexpr auto finish() -> It {
        if (m_run_length != 0) {
            emit_run(m_run_length);
            m_run_length = 0;
        }

        return m_it;
    }

private:
    It m_it;

    std::array<color, 64> m_hash_table;
    color m_prev_color{0, 0, 0, 255};

    usize m_run_length = 0;

    constexpr void emit_byte(u8 b) {
        // might do stuff here later
        *m_it++ = b;
    }

    constexpr void emit_rgba(color c) {
        emit_byte(0xFF);
        emit_byte(c.r);
        emit_byte(c.g);
        emit_byte(c.b);
        emit_byte(c.a);
    }

    constexpr void emit_rgb(color c) {
        emit_byte(static_cast<u8>(0xFE));
        emit_byte(c.r);
        emit_byte(c.g);
        emit_byte(c.b);
    }

    constexpr void emit_index(u8 index) {
        if (index & 0xC0) {
            std::unreachable();
        }

        emit_byte(index);
    }

    constexpr auto emit_diff(color prev, color cur) -> bool {
        int dr = cur.r - prev.r + 2;
        int dg = cur.g - prev.g + 2;
        int db = cur.b - prev.b + 2;

        auto cond = [](int d) -> bool { return (d & ~0b11) == 0; };

        if (!cond(dr) || !cond(dg) || !cond(db)) {
            return false;
        }

        emit_byte(0x40 | (dr << 4) | (dg << 2) | db);
        return true;
    }

    constexpr auto emit_luma(color prev, color cur) -> bool {
        int dg = cur.g - prev.g;
        int dr_dg = (cur.r - prev.r) - dg;
        int db_dg = (cur.b - prev.b) - dg;

        dr_dg += 8;
        dg += 32;
        db_dg += 8;

        if ((dr_dg & ~0xF) || (db_dg & ~0xF) || (dg & ~0x3F)) {
            return false;
        }

        emit_byte(0x80 | dg);
        emit_byte((dr_dg << 4) | db_dg);
        return true;
    }

    constexpr void emit_run(usize run_length) {
        if (run_length >= 62 || run_length == 0) {
            std::unreachable();
        }

        emit_byte(static_cast<u8>(0xC0 | (run_length - 1)));
    }
};

}  // namespace encoder

template<typename It>
constexpr auto encode_lossless(It out_data, std::span<const color> in_pixels)
  -> expected::expected<It, std::string_view> {
    encoder::lossless_encoder encoder{out_data};
    for (color c : in_pixels) {
        encoder.pixel(c);
    }
    return encoder.finish();
}

}  // namespace stf::qoi::detail
