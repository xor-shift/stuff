#pragma once

namespace stf::qoi::detail {

namespace decoder {

constexpr auto determine_op(u8 leading) -> qoi_op {
    if (leading == 0xFF) {
        return qoi_op::rgba;
    }

    if (leading == 0xFE) {
        return qoi_op::rgb;
    }

    switch (leading >> 6) {
        case 0: return qoi_op::index;
        case 1: return qoi_op::diff;
        case 2: return qoi_op::luma;
        case 3: return qoi_op::run;
        default: std::unreachable();
    }
}

constexpr auto op_size(qoi_op op) -> usize {
    switch (op) {
        case qoi_op::index: [[fallthrough]];
        case qoi_op::diff: [[fallthrough]];
        case qoi_op::run: return 1uz;
        case qoi_op::luma: return 2uz;
        case qoi_op::rgb: return 4uz;
        case qoi_op::rgba: return 5uz;
        default: std::unreachable();
    }
}

struct decoder {
private:
    using err_type = expected::unexpected<std::string_view>;
    static constexpr err_type m_ran_out_err{"ran out of QOI data prematurely"};
    static constexpr err_type m_overrun_err{"QOI data contains more data than the header implies"};

public:
    constexpr decoder(std::span<color> out_pixels, std::span<const u8> in_data) noexcept
        : m_out_pixels(out_pixels)
        , m_in_data(in_data) {
        std::fill(m_hash_table.begin(), m_hash_table.end(), color{0, 0, 0, 0});
    }

    constexpr auto decode() -> expected::expected<usize, std::string_view> {
        usize orig_size = m_in_data.size();

        for (;;) {
            // we can put out, at most, 62 pixels pr iteration
            // and we can consume, at most, 5 bytes per iteration

            bool checked = m_out_pixels.size() < 62 || m_in_data.size() < 5;

            if (!checked) {
                iterate_unchecked();
                continue;
            }

            TRYX(iterate_checked());

            if (m_out_pixels.empty()) {
                if (m_in_data.size() == 8) {
                    break;
                }

                return m_overrun_err;
            }

            if (m_in_data.empty()) {
                return m_ran_out_err;
            }
        }

        return orig_size - m_in_data.size();
    }

private:
    std::span<color> m_out_pixels;
    std::span<const u8> m_in_data;

    std::array<color, 64> m_hash_table;
    color m_prev{0, 0, 0, 255};

    template<bool SetPrev, bool SetIndex>
    constexpr void emit(color c, usize ct = 1) {
        if constexpr (SetPrev) {
            m_prev = c;
        }

        if constexpr (SetIndex) {
            m_hash_table[c.hash()] = c;
        }

        std::fill_n(m_out_pixels.data(), ct, c);
        m_out_pixels = m_out_pixels.subspan(ct);
    }

    constexpr void process_op_unchecked(u8 leading, qoi_op op) {
        usize consumed;

        switch (op) {
            case qoi_op::index:
                emit<true, false>(m_hash_table[leading & 0x3F]);
                consumed = 1;
                break;

            case qoi_op::diff:
                emit<true, true>(m_prev.diff((leading >> 4) & 3, (leading >> 2) & 3, leading & 3));
                consumed = 1;
                break;

            case qoi_op::luma:
                emit<true, true>(m_prev.luma(leading & 0x3F, m_in_data[1] >> 4, m_in_data[1] & 0xF));
                consumed = 2;
                break;

            case qoi_op::run:
                emit<false, true>(m_prev, (leading & 0x3F) + 1);
                consumed = 1;
                break;

            case qoi_op::rgb: [[fallthrough]];
            case qoi_op::rgba: {
                bool alpha = (leading & 1);

                color c;
                c.r = m_in_data[1];
                c.g = m_in_data[2];
                c.b = m_in_data[3];
                c.a = m_in_data[4] * alpha + m_prev.a * !alpha;

                consumed = 4 + alpha;

                emit<true, true>(c);
            } break;

            default: std::unreachable();
        }

        m_in_data = m_in_data.subspan(consumed);
    }

    constexpr void iterate_unchecked() {
        u8 leading = m_in_data.front();
        qoi_op op = determine_op(leading);
        process_op_unchecked(leading, op);
    }

    constexpr auto iterate_checked() -> expected::expected<void, std::string_view> {
        if (m_in_data.empty()) {
            return m_ran_out_err;
        }

        u8 leading = m_in_data.front();
        qoi_op op = determine_op(leading);

        usize expected_to_emit;
        switch (op) {
            case qoi_op::run: expected_to_emit = leading & 0x3F; break;
            default: expected_to_emit = 1;
        }

        if (m_out_pixels.size() < expected_to_emit) {
            return m_overrun_err;
        }

        usize expected_to_consume = op_size(op);

        if (m_in_data.size() < expected_to_consume) {
            return m_ran_out_err;
        }

        process_op_unchecked(leading, op);

        return {};
    }
};

}  // namespace decoder

constexpr auto decode(std::span<color> out_pixels, std::span<const u8> in_data)
  -> expected::expected<void, std::string_view> {
    TRYX(decoder::decoder{out_pixels, in_data}.decode());
    return {};
}

}  // namespace stf::qoi::detail
