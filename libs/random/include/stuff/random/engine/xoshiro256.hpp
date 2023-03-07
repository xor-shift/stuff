#pragma once

#include <stuff/random/engine/splitmix.hpp>
#include <stuff/random/util.hpp>

#include <span>

namespace stf::random {

namespace detail {

struct xoshiro256_base {
    using result_type = u64;

    constexpr xoshiro256_base()
        : xoshiro256_base(0xDEADBEEFCAFEBABEull) {}

    constexpr xoshiro256_base(xoshiro256_base const&) = default;
    constexpr xoshiro256_base(xoshiro256_base&&) = default;

    constexpr xoshiro256_base(u64 s) noexcept(noexcept(xoshiro256_base{}.seed(s))) { seed(s); }

    template<concepts::seed_sequence S>
    constexpr xoshiro256_base(S& q) noexcept(noexcept(xoshiro256_base{}.seed(q))) {
        seed(q);
    }

    static constexpr auto min() -> result_type { return 0; }
    static constexpr auto max() -> result_type { return -1; }

    constexpr auto discard(unsigned long long z) {
        for (auto i = 0ull; i < z; i++) {
            permute_state();
        }
    }

    constexpr void seed() noexcept { std::fill_n(m_state, 4, 0); }

    constexpr void seed(u64 s) noexcept {
        splitmix_64_generator gen{s};
        m_state[0] = gen();
        m_state[1] = gen();
        m_state[2] = gen();
        m_state[3] = gen();
    }

    template<concepts::seed_sequence S>
    constexpr void seed(S& seq) {
        std::array<u32, 8> gen_data{};
        seq.generate(gen_data.begin(), gen_data.end());
        m_state[0] = (static_cast<u64>(gen_data[0]) << 32) | gen_data[1];
        m_state[1] = (static_cast<u64>(gen_data[2]) << 32) | gen_data[3];
        m_state[2] = (static_cast<u64>(gen_data[4]) << 32) | gen_data[5];
        m_state[3] = (static_cast<u64>(gen_data[6]) << 32) | gen_data[7];
    }

    friend constexpr auto operator==(xoshiro256_base const& lhs, xoshiro256_base const& rhs) -> bool {
        return std::equal(std::begin(lhs.m_state), std::end(lhs.m_state), std::begin(rhs.m_state));
    }

    constexpr auto state() -> std::span<u64, 4> { return std::span(m_state); }

    constexpr void jump_128() {
        return jump({0x180ec6d33cfd0abaull, 0xd5a61266f0c9392cull, 0xa9582618e03fc9aaull, 0x39abdc4529b1661cull});
    }

    constexpr void jump_192() {
        return jump({0x76e15d3efefdcbbfull, 0xc5004e441c522fb3ull, 0x77710069854ee241ull, 0x39109bb02acbe635ull});
    }

protected:
    constexpr void permute_state() {
        u64 t = m_state[1] << 17;

        m_state[2] ^= m_state[0];
        m_state[3] ^= m_state[1];
        m_state[1] ^= m_state[2];
        m_state[0] ^= m_state[3];

        m_state[2] ^= t;

        m_state[3] = std::rotl(m_state[3], 45);
    }

    constexpr void jump(const u64 (&poly)[4]) {
        return jump_f2_linear(std::span(m_state), std::span(poly), [this] { permute_state(); });
    }

    u64 m_state[4];
};

}  // namespace detail

struct xoshiro_256p : detail::xoshiro256_base {
    using base_type = xoshiro256_base;

    constexpr xoshiro_256p(u64 s) noexcept(noexcept(base_type(s)))
        : base_type(s) {}

    template<concepts::seed_sequence S>
    constexpr xoshiro_256p(S& q) noexcept(noexcept(base_type(q)))
        : base_type(q) {}

    constexpr xoshiro_256p() = default;
    constexpr xoshiro_256p(xoshiro_256p const&) = default;
    constexpr xoshiro_256p(xoshiro_256p&&) = default;

    constexpr auto operator()() -> u64 {
        uint64_t ret = m_state[0] + m_state[3];

        permute_state();

        return ret;
    }
};

struct xoshiro_256pp : detail::xoshiro256_base {
    using base_type = xoshiro256_base;

    constexpr xoshiro_256pp(u64 s) noexcept(noexcept(base_type(s)))
        : base_type(s) {}

    template<concepts::seed_sequence S>
    constexpr xoshiro_256pp(S& q) noexcept(noexcept(base_type(q)))
        : base_type(q) {}

    constexpr xoshiro_256pp() = default;
    constexpr xoshiro_256pp(xoshiro_256pp const&) = default;
    constexpr xoshiro_256pp(xoshiro_256pp&&) = default;

    constexpr auto operator()() -> u64 {
        uint64_t ret = std::rotl(m_state[0] + m_state[3], 23) + m_state[0];

        permute_state();

        return ret;
    }
};

struct xoshiro_256ss : detail::xoshiro256_base {
    using base_type = xoshiro256_base;

    constexpr xoshiro_256ss(u64 s) noexcept(noexcept(base_type(s)))
        : base_type(s) {}

    template<concepts::seed_sequence S>
    constexpr xoshiro_256ss(S& q) noexcept(noexcept(base_type(q)))
        : base_type(q) {}

    constexpr xoshiro_256ss() = default;
    constexpr xoshiro_256ss(xoshiro_256ss const&) = default;
    constexpr xoshiro_256ss(xoshiro_256ss&&) = default;

    constexpr auto operator()() -> u64 {
        uint64_t ret = std::rotl(m_state[1] * 5, 7) * 9;

        permute_state();

        return ret;
    }
};

}  // namespace stf::random
