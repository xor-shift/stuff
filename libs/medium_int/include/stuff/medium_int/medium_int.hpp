#pragma once

#include <stuff/medium_int/detail/carry.hpp>
#include <stuff/medium_int/detail/repr.hpp>

#include <stuff/bit.hpp>
#include <stuff/core.hpp>

#include <functional>
#include <limits>
#include <ranges>
#include <span>

namespace stf {

template<usize Bits, std::unsigned_integral Repr = typename detail::decide_medium_int_repr<Bits>::type>
struct medium_int {
    using repr_type = Repr;
    inline static constexpr auto m_word_count = (Bits / std::numeric_limits<repr_type>::digits) + ((Bits % std::numeric_limits<repr_type>::digits) != 0 ? 1 : 0);
    inline static constexpr auto m_bits_per_word = std::numeric_limits<repr_type>::digits;
    inline static constexpr auto m_excess_bits = std::numeric_limits<repr_type>::digits * m_word_count - Bits;
    static_assert(m_excess_bits < std::numeric_limits<repr_type>::digits);

    medium_int() = default;
    medium_int(medium_int const&) = default;
    medium_int(medium_int&&) = default;

    template<std::integral T>
    constexpr medium_int(T v)
        : medium_int() {
        set(v);
    }

    constexpr void reset() { std::ranges::fill(m_repr, 0); }

    constexpr auto get_repr() const -> std::span<repr_type, m_word_count> { return m_repr; }

    /**
     * returns <code>Bytes</code> bytes representing this medium_int, little endian, sign extended
     * @tparam Bytes How many bytes do ya want?
     * @return a forward range of fixed size containing <code>u8</code>s. in the future, when <code>std::views::concat</code> is a thing: a forward range of indefinite size.
     */
    template<usize Bytes>
    constexpr auto get_bytes_zext() const -> std::ranges::range auto {
        // TODO: return range in the future, when std::views::concat is standardised and implemented (C++26, if LEWG accepts the paper given by the Ranges SG)
        // leaving the return type to be auto pre-emptively

        return get_bytes_impl<Bytes, false>();
    }

    /**
     * returns <code>Bytes</code> bytes representing this medium_int, little endian, sign extended
     * @tparam Bytes How many bytes do ya want?
     * @return a forward range of fixed size containing <code>u8</code>s. in the future, when <code>std::views::concat</code> is a thing: a forward range of indefinite size.
     */
    template<usize Bytes>
    constexpr auto get_bytes_sext() const -> std::ranges::range auto {
        return get_bytes_impl<Bytes, true>();
    }

    template<std::unsigned_integral OtherT>
    constexpr void set(OtherT val) {
        const auto le_val = stf::bit::convert_endian(val, std::endian::native, std::endian::little);
        const auto arr = std::bit_cast<std::array<u8, sizeof(OtherT)>>(le_val);
        return set_from_unsigned_bytes(std::span<const u8>(arr), std::endian::little);
    }

    template<std::signed_integral OtherT>
    constexpr void set(OtherT val) {
        using unsigned_type = std::make_unsigned_t<OtherT>;
        if (val >= 0) {
            return set<unsigned_type>(static_cast<unsigned_type>(val));
        }

        if (val == -1) {
            set(1u);
            negate();
            return;
        }

        if (val == std::numeric_limits<OtherT>::min()) {
            reset();

            const auto sign_bit_pos = ((Bits % std::numeric_limits<repr_type>::digits) ?: std::numeric_limits<repr_type>::digits) - 1;

            m_repr.back() = static_cast<repr_type>(1) << sign_bit_pos;

            return;
        }

        set<unsigned_type>(static_cast<unsigned_type>(-val));
        negate();
    }

    template<std::unsigned_integral OtherT>
    constexpr auto get() const -> OtherT {
        auto little_endian_bytes =                                                                                                   //
          m_repr                                                                                                                     //
          | std::views::transform([](auto val) { return stf::bit::convert_endian(val, std::endian::native, std::endian::little); })  //
          | std::views::transform(std::bit_cast<std::array<u8, sizeof(repr_type)>, repr_type>)                                       //
          | std::views::join                                                                                                         //
          | std::views::take(sizeof(OtherT));

        // TODO: use std::ranges::to when it is available
        auto ret_bytes_le = std::array<u8, sizeof(OtherT)>{};
        std::ranges::copy(little_endian_bytes, ret_bytes_le.begin());

        const auto ret_val_le = std::bit_cast<OtherT>(ret_bytes_le);
        const auto ret_val = stf::bit::convert_endian(ret_val_le, std::endian::little, std::endian::native);

        return ret_val;
    }

    template<std::signed_integral OtherT>
        requires(std::numeric_limits<std::make_unsigned_t<OtherT>>::digits <= Bits)
    constexpr auto get() const -> OtherT {
        return std::bit_cast<OtherT>(get<std::make_unsigned_t<OtherT>>());
    }

    template<std::signed_integral OtherT>
        requires(std::numeric_limits<std::make_unsigned_t<OtherT>>::digits > Bits)
    constexpr auto get() const -> OtherT {
        const auto empty_sign_bits = std::numeric_limits<std::make_unsigned_t<OtherT>>::digits - Bits;

        // cheap trick to sign extend
        // two's complement was standardised, right?
        const auto temp = get<std::make_unsigned_t<OtherT>>() << empty_sign_bits;
        const std::signed_integral auto ret = static_cast<OtherT>(temp) >> empty_sign_bits;

        return ret;
    }

    constexpr auto operator~() -> medium_int {
        auto ret = medium_int{*this};
        ret.invert();
        return ret;
    }

    constexpr auto operator-() -> medium_int {
        auto ret = medium_int{*this};
        ret.negate();
        return ret;
    }

    constexpr auto operator++() -> medium_int& {
        detail::carry_context<repr_type> context{};

        m_repr[0] = context.addc(m_repr[0], repr_type{1});

        for (auto& elem : m_repr | std::views::drop(1)) {
            elem = context.addc(elem, repr_type{0});
        }

        mask_excess_bits_out();

        return *this;
    }

    constexpr auto operator--() -> medium_int& {
        detail::carry_context<repr_type> context{};

        m_repr[0] = context.subc(m_repr[0], repr_type{1});

        for (auto& elem : m_repr | std::views::drop(1)) {
            elem = context.subc(elem, repr_type{0});
        }

        mask_excess_bits_out();

        return *this;
    }

    constexpr auto operator++() const -> medium_int {
        auto ret = *this;
        this->operator++();
        return ret;
    }

    //friend constexpr auto operator==(medium_int const& lhs, medium_int const& rhs) -> bool { return std::ranges::equal(lhs.m_repr, rhs.m_repr); }

    friend constexpr auto operator<=>(medium_int const& lhs, medium_int const& rhs) -> std::strong_ordering {
        for (auto [lhs_v, rhs_v] : std::views::zip(lhs.m_repr | std::views::reverse, rhs.m_repr | std::views::reverse)) {
            const auto tmp = lhs_v <=> rhs_v;

            if (tmp == std::strong_ordering::equivalent) {
                continue;
            }

            return tmp;
        }

        return std::strong_ordering::equivalent;
    }

    friend constexpr auto operator==(medium_int const& lhs, medium_int const& rhs) -> bool { return (lhs <=> rhs) == std::strong_ordering::equivalent; }

    constexpr auto operator+=(medium_int const& other) -> medium_int& {
        auto context = detail::carry_context<repr_type>{};

        for (auto [lhs, rhs] : std::views::zip(m_repr, other.m_repr) | std::views::reverse) {
            lhs = context.addc(lhs, rhs);
        }

        mask_excess_bits_out();

        return *this;
    }

    friend constexpr auto operator+(medium_int const& lhs, medium_int const& rhs) -> medium_int {
        auto ret = lhs;
        ret += rhs;
        return ret;
    }

    constexpr auto operator-=(medium_int const& other) -> medium_int& {
        auto context = detail::carry_context<repr_type>{};

        for (auto& [lhs, rhs] : std::views::zip(m_repr, other.m_repr) | std::views::reverse) {
            lhs = context.addc(lhs, rhs);
        }

        mask_excess_bits_out();

        return *this;
    }

    friend constexpr auto operator-(medium_int const& lhs, medium_int const& rhs) -> medium_int {
        auto ret = lhs;
        ret -= rhs;
        return ret;
    }

private:
    std::array<repr_type, m_word_count> m_repr{};

    constexpr auto is_negative() const -> bool {
        const auto last_word = m_repr.back();
        return ((last_word >> (m_bits_per_word - m_excess_bits)) & 1) != 0;
    }

    template<usize Bytes, bool SignExtend>
    constexpr auto get_bytes_impl() const -> std::ranges::range auto {
        auto repr_bytes = m_repr | std::views::transform([](auto v) { return stf::bit::convert_endian(v, std::endian::native, std::endian::little); }) |
                          std::views::transform([](auto v) { return std::bit_cast<std::array<u8, sizeof(v)>>(v); }) | std::views::join | std::views::take(Bytes);

        auto ret = std::array<u8, Bytes>{};

        auto it = std::ranges::copy(repr_bytes, ret.begin());
        std::fill(it, ret.end(), SignExtend ? std::numeric_limits<u8>::max() : 0);

        if (SignExtend && m_excess_bits != 0 && is_negative()) {
            *it |= std::numeric_limits<u8>::max() << (std::numeric_limits<u8>::digits - m_excess_bits);
        }

        return ret;
    }

    constexpr void set_from_unsigned_bytes(std::span<const u8> bytes, std::endian endian = std::endian::little) {
        std::ranges::fill(m_repr, 0);

        auto repr_range =                         //
          bytes                                   //
          | std::views::chunk(sizeof(repr_type))  //
          | std::views::transform([](auto chunk) {
                // TODO: use std::ranges::to when it is available
                auto arr = std::array<u8, sizeof(repr_type)>{};
                std::ranges::copy(chunk, arr.begin());
                return arr;
            })                                                                                                            //
          | std::views::transform(std::bit_cast<repr_type, std::array<u8, sizeof(repr_type)>>)                            //
          | std::views::transform([endian](auto v) { return stf::bit::convert_endian(v, endian, std::endian::native); })  //
          | std::views::take(m_word_count);

        std::ranges::copy(repr_range, m_repr.begin());

        mask_excess_bits_out();
    }

    constexpr void mask_excess_bits_out() {
        // the upper bits of m_repr.back() that are not a part of the representation

        if constexpr (m_excess_bits == 0) {
            return;
        } else {
            constexpr auto mask = ((repr_type{1} << (m_excess_bits)) - 1) << (std::numeric_limits<repr_type>::digits - m_excess_bits);
            static_assert(m_excess_bits == 0 || mask != 0);
            static_assert(m_excess_bits == 0 || mask < std::numeric_limits<repr_type>::max());

            m_repr.back() &= ~mask;
        }
    }

    constexpr void invert() {
        for (auto& v : m_repr) {
            v = ~v;
        }

        mask_excess_bits_out();
    }

    constexpr void negate() {
        invert();
        ++*this;
    }
};

}  // namespace stf
