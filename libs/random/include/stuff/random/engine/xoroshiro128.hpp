#pragma once

#include <stuff/random/engine/splitmix.hpp>
#include <stuff/random/util.hpp>

#include <span>

namespace stf::random {

namespace detail {

template<int A, int B, int C>
struct xoroshiro_128_base {
    using result_type = u64;

    constexpr xoroshiro_128_base(u64 s) noexcept(noexcept(xoroshiro_128_base{}.seed(s))) { seed(s); }

    template<concepts::seed_sequence S>
    constexpr xoroshiro_128_base(S& q) noexcept(noexcept(xoroshiro_128_base{}.seed(q))) {
        seed(q);
    }

    constexpr xoroshiro_128_base() = default;
    constexpr xoroshiro_128_base(xoroshiro_128_base const&) = default;
    constexpr xoroshiro_128_base(xoroshiro_128_base&&) = default;

    static constexpr auto min() -> result_type { return 0; }
    static constexpr auto max() -> result_type { return -1; }

    constexpr auto discard(unsigned long long z) {
        for (auto i = 0ull; i < z; i++) {
            permute_state();
        }
    }

    constexpr void seed() noexcept { std::fill_n(m_state, 2, 0); }

    constexpr void seed(u64 s) noexcept {
        splitmix_64_generator gen{s};
        m_state[0] = gen();
        m_state[1] = gen();
    }

    template<concepts::seed_sequence S>
    constexpr void seed(S& seq) {
        std::array<u32, 4> gen_data{};
        seq.generate(gen_data.begin(), gen_data.end());
        m_state[0] = (static_cast<u64>(gen_data[0]) << 32) | gen_data[1];
        m_state[1] = (static_cast<u64>(gen_data[2]) << 32) | gen_data[3];
    }

    friend constexpr auto operator==(xoroshiro_128_base const& lhs, xoroshiro_128_base const& rhs) -> bool {
        return std::equal(std::begin(lhs.m_state), std::end(lhs.m_state), std::begin(rhs.m_state));
    }

    constexpr auto state() -> std::span<u64, 2> { return m_state; }

protected:
    constexpr void permute_state() {
        uint64_t s0 = m_state[0];
        uint64_t s1 = m_state[1] ^ m_state[0];

        m_state[0] = std::rotl(s0, A) ^ s1 ^ (s1 << B);
        m_state[1] = std::rotl(s1, C);
    }

    constexpr void jump(const u64 (&poly)[2]) {
        return jump_f2_linear(std::span(m_state), std::span(poly), [this] { permute_state(); });
    }

    u64 m_state[2]{0};
};

}  // namespace detail

struct xoroshiro_128p : detail::xoroshiro_128_base<24, 16, 37> {
    using base_type = xoroshiro_128_base<24, 16, 37>;

    constexpr xoroshiro_128p(u64 s) noexcept(noexcept(base_type(s)))
        : base_type(s) {}

    template<concepts::seed_sequence S>
    constexpr xoroshiro_128p(S& q) noexcept(noexcept(base_type(q)))
        : base_type(q) {}

    constexpr xoroshiro_128p() = default;
    constexpr xoroshiro_128p(xoroshiro_128p const&) = default;
    constexpr xoroshiro_128p(xoroshiro_128p&&) = default;

    constexpr auto operator()() -> u64 {
        uint64_t ret = m_state[0] + m_state[1];

        permute_state();

        return ret;
    }

    constexpr void jump_64() { jump({0xdf900294d8f554a5, 0x170865df4b3201fc}); }
    constexpr void jump_96() { jump({0xd2a98b26625eee7b, 0xdddf9b1090aa7ac1}); }
};

struct xoroshiro_128pp : detail::xoroshiro_128_base<49, 21, 28> {
    using base_type = xoroshiro_128_base<49, 21, 28>;

    constexpr xoroshiro_128pp(u64 s) noexcept(noexcept(base_type(s)))
        : base_type(s) {}

    template<concepts::seed_sequence S>
    constexpr xoroshiro_128pp(S& q) noexcept(noexcept(base_type(q)))
        : base_type(q) {}

    constexpr xoroshiro_128pp() = default;
    constexpr xoroshiro_128pp(xoroshiro_128pp const&) = default;
    constexpr xoroshiro_128pp(xoroshiro_128pp&&) = default;

    constexpr auto operator()() -> u64 {
        uint64_t ret = std::rotl(m_state[0] + m_state[1], 17) + m_state[0];

        permute_state();

        return ret;
    }

    constexpr void jump_64() { jump({0x2bd7a6a6e99c2ddc, 0x0992ccaf6a6fca05}); }
    constexpr void jump_96() { jump({0x360fd5f2cf8d5d99, 0x9c6e6877736c46e3}); }
};

struct xoroshiro_128ss : detail::xoroshiro_128_base<24, 16, 37> {
    using base_type = xoroshiro_128_base<24, 16, 37>;

    constexpr xoroshiro_128ss(u64 s) noexcept(noexcept(base_type(s)))
        : base_type(s) {}

    template<concepts::seed_sequence S>
    constexpr xoroshiro_128ss(S& q) noexcept(noexcept(base_type(q)))
        : base_type(q) {}

    constexpr xoroshiro_128ss() = default;
    constexpr xoroshiro_128ss(xoroshiro_128ss const&) = default;
    constexpr xoroshiro_128ss(xoroshiro_128ss&&) = default;

    constexpr auto operator()() -> u64 {
        uint64_t ret = std::rotl(m_state[0] * 5, 7) * 9;

        permute_state();

        return ret;
    }

    constexpr void jump_64() { jump({0xdf900294d8f554a5, 0x170865df4b3201fc}); }
    constexpr void jump_96() { jump({0xd2a98b26625eee7b, 0xdddf9b1090aa7ac1}); }
};

static_assert(concepts::uniform_random_bit_generator<xoroshiro_128p>);
static_assert(concepts::random_number_engine<xoroshiro_128p>);

}  // namespace stf::random
