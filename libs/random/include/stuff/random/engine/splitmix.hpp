#pragma once

#include <stuff/core.hpp>
#include <stuff/random/concepts.hpp>

namespace stf::random {

namespace detail {

template<std::unsigned_integral T, T Inc, T M0, int S0, T M1, int S1, T M2, int S2>
struct splitmix_generator {
    using result_type = T;

    constexpr splitmix_generator() = default;

    constexpr splitmix_generator(splitmix_generator const&) = default;
    constexpr splitmix_generator(splitmix_generator&&) = default;

    constexpr splitmix_generator(result_type seed) noexcept
        : m_state(seed) {}

    // SeedSequence requirements

    template<typename IIt>
    constexpr splitmix_generator(IIt beg, IIt end) noexcept(noexcept(*beg) && noexcept(beg == end))
        : m_state(beg != end ? *beg : 0) {}

    constexpr splitmix_generator(std::initializer_list<result_type> il)  //
      noexcept(noexcept(splitmix_generator(il.begin(), il.end())))
        : splitmix_generator(il.begin(), il.end()) {}

    // RandomNumberEngine requirements

    template<concepts::seed_sequence Seq>
        requires(!std::is_same_v<Seq, splitmix_generator>)
    constexpr splitmix_generator(Seq& seq) noexcept(noexcept(splitmix_generator{}.seed(seq))) {
        seed(seq);
    }

    // UniformRandomBitGenerator requirements

    static constexpr auto min() -> result_type { return 0; }
    static constexpr auto max() -> result_type { return -1; }

    constexpr auto operator()() -> result_type {
        m_state += Inc;
        m_state *= M0;

        result_type ret = m_state;
        ret = (ret ^ (ret >> S0)) * M1;
        ret = (ret ^ (ret >> S1)) * M2;
        return ret ^ (ret >> S2);
    }

    // SeedSequence requirements

    template<typename OIt>
    constexpr void generate(OIt ob, OIt oe) {
        std::generate(ob, oe, *this);
    }

    constexpr auto size() const -> usize { return 1; }

    template<typename OIt>
    constexpr void param(OIt it) const {
        *it++ = m_state;
    }

    // RandomNumberEngine requirements

    constexpr void seed() { m_state = 0; }
    constexpr void seed(T s) { m_state = s; }

    template<concepts::seed_sequence Seq>
    constexpr void seed(Seq& q) {
        q.generate(&m_state, &m_state + 1);
    }

    constexpr void discard(unsigned long long z) {
        for (auto i = 0ull; i < z; i++) {
            (*this)();
        }
    }

private:
    result_type m_state = 0;
};

}  // namespace detail

using splitmix_32_generator =
  detail::splitmix_generator<u32, 314159265u, 1664525u, 15, 0x5ce4e5b9u, 13, 0x1331c1ebu, 15>;

static_assert(concepts::uniform_random_bit_generator<splitmix_32_generator>);
static_assert(concepts::random_number_engine<splitmix_32_generator>);
static_assert(concepts::seed_sequence<splitmix_32_generator>);

using splitmix_64_generator =
  detail::splitmix_generator<u64, 0x9e3779b97f4a7c15ul, 1ul, 30, 0xbf58476d1ce4e5b9ul, 27, 0x94d049bb133111ebul, 31>;

static_assert(concepts::uniform_random_bit_generator<splitmix_64_generator>);
static_assert(concepts::random_number_engine<splitmix_64_generator>);
static_assert(concepts::seed_sequence<splitmix_64_generator>);

}  // namespace stf::random
