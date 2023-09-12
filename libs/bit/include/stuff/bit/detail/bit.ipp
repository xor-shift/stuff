#pragma once

#include <stuff/core/integers.hpp>

#include <array>
#include <bit>
#include <concepts>
#include <string_view>

namespace stf::bit {

namespace detail {

template<std::unsigned_integral T, usize nth_pattern>
constexpr auto bit_reversal_pattern() -> T {
    //                  inc shift val
    // 1010101010101010   2    2    1
    // 1100110011001100   4    4    3
    // 1111000011110000   8    8    15
    // 1111111100000000   16   16   255

    T ret{};

    usize bits = sizeof(T) * 8;

    usize shift_amount = 1 << (nth_pattern + 1);
    T or_val = (T{1} << (shift_amount / 2)) - 1;
    usize final_shift = shift_amount / 2;

    for (usize i = 0; i < bits; i += shift_amount) {
        ret <<= (shift_amount >= bits) ? bits - 1 : shift_amount;
        ret |= or_val;
    }

    return ret << final_shift;
}

template<std::unsigned_integral T>
struct reversal_steps;

template<>
struct reversal_steps<u8> : std::integral_constant<usize, 3> {};

template<>
struct reversal_steps<u16> : std::integral_constant<usize, 4> {};

template<>
struct reversal_steps<u32> : std::integral_constant<usize, 5> {};

template<>
struct reversal_steps<u64> : std::integral_constant<usize, 6> {};

template<std::unsigned_integral T>
inline static constexpr usize reversal_steps_v = reversal_steps<T>::value;

template<std::unsigned_integral T, size_t current_step = 0>
constexpr auto reverse_bits_patterns(T v) -> T {
    const size_t pattern = bit_reversal_pattern<T, current_step>();

    const auto shift = 1 << current_step;

    const auto left = v & pattern;
    const auto right = v & ~pattern;

    const auto temp = (left >> shift) | (right << shift);

    if constexpr (current_step + 1 < reversal_steps_v<T>)
        return reverse_bits_patterns<T, current_step + 1>(temp);
    return temp;
}

template<std::unsigned_integral T>
constexpr auto reverse_bits_naive(T v) -> T {
    T out{};

    for (size_t i = 0; i < sizeof(T) * 8; i++) {
        const T lsb = v & T{1};
        v >>= 1;
        out |= lsb << (sizeof(T) * 8 - i - 1);
    }

    return out;
}

}  // namespace detail

template<typename T>
constexpr auto reverse_bits(T v) -> T {
    return detail::reverse_bits_patterns(v);
}

template<typename T>
constexpr auto reverse_bytes(T v) -> T {
    if consteval {
        auto arr = std::bit_cast<std::array<char, sizeof(T)>>(v);
        for (usize i = 0; i < sizeof(T) / 2; i++) {
            char& a = arr[i];
            char& b = arr[sizeof(T) - i - 1];

            auto t = a;
            a = b;
            b = t;
        }
        return std::bit_cast<T>(arr);
    } else {
        auto* p = reinterpret_cast<char*>(&v);
        for (usize i = 0; i < sizeof(T) / 2; i++) {
            char& a = p[i];
            char& b = p[sizeof(T) - i - 1];

            std::swap(a, b);
        }
        return v;
    }
}

namespace detail {

template<typename T>
constexpr auto compound_shift(T v, int left_shift, int right_shift) -> T {
    v <<= left_shift;
    v >>= right_shift;
    return v;
};

template<typename T, int StartBitMSB, stf::string_literal Pattern>
consteval auto generate_extraction_info() {
    auto str = std::string_view{Pattern.c_str(), Pattern.size()};

    auto extract_int = [](auto&& str) -> int {
#ifdef __cpp_lib_constexpr_charconv
        int v;
        const auto res = std::from_chars(str.begin(), str.end(), v);

        if (res.ec != std::errc()) {
            throw "invalid extraction pattern";
        }

        str = str.substr(std::distance(str.begin(), res.ptr));

        return v;
#else

        usize digit_ct = 0;
        int ret = 0;

        for (char c : str) {
            if (c < '0' || c > '9') {
                break;
            }

            ret *= 10;
            ret += c - '0';
            digit_ct++;
        }

        str = str.substr(digit_ct);

        return ret;
#endif
    };

    constexpr auto amt_segments = std::count(Pattern.begin(), Pattern.end(), '|') + 1;
    constexpr auto amt_skips = std::count(Pattern.begin(), Pattern.end(), '~');
    auto ret = std::array<std::tuple<T, int, int>, amt_segments - amt_skips> {};

    int bit_index = StartBitMSB;
    for (usize i = 0; !str.empty(); i++) {
        //const auto segment_sz = std::min<std::string_view::size_type>(str.find_first_of('|'), str.size());
        const auto segment_sz = std::distance(str.begin(), std::find(str.begin(), str.end(), '|'));
        auto segment = str.substr(0, segment_sz);
        str = segment.size() == str.size() ? str.substr(segment.size()) : str.substr(segment.size() + 1);

        const auto from = extract_int(segment);
        const auto skip = !segment.empty() && segment.front() == '~';
        const auto down_to = !segment.empty() ? extract_int(segment.substr(1)) : from;

        if (down_to > from) {
            throw "invalid extraction pattern, b > a in a:b";
        }

        const auto segment_width = from - down_to + 1;
        const auto raw_left_shift_amt = from - bit_index;
        const auto left_shift_amt = raw_left_shift_amt < 0 ? 0 : raw_left_shift_amt;
        const auto right_shift_amt = raw_left_shift_amt < 0 ? -raw_left_shift_amt : 0;

        const T mask = (((T)1 << (T)segment_width) - (T)1) << (T)down_to;

        if (!skip) {
            std::get<0>(ret[i]) = mask;
            std::get<1>(ret[i]) = left_shift_amt;
            std::get<2>(ret[i]) = right_shift_amt;
        } else {
            i -= 1;
        }

        bit_index -= segment_width;
    }

    return ret;
}

}

template<typename T, usize StartBitMSB, stf::string_literal Pattern>
constexpr auto extract(T v) -> T {
    constexpr auto extraction_info = detail::generate_extraction_info<T, StartBitMSB, Pattern>();

    u32 ret = 0;
    for (auto& [mask, left_shift, right_shift] : extraction_info) {
        ret |= detail::compound_shift<u32>(v, left_shift, right_shift) & mask;
    }

    return ret;
}

}  // namespace stf::bit
