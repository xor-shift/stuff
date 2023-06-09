#pragma once

#include <stuff/random/concepts.hpp>
#include <stuff/random/dist/erand48.hpp>

#include <cmath>
#include <numbers>

namespace stf::random {

namespace detail {

template<std::floating_point T = double, bool Polar = false>
    requires(std::is_same_v<T, float> || std::is_same_v<T, double>)
struct box_muller_base {
    struct parameters {
        T mu = 0;
        T sigma = 1;

        friend constexpr auto operator==(parameters const& lhs, parameters const& rhs) -> bool {
            return lhs.mu == rhs.mu && lhs.sigma == rhs.sigma;
        }
    };

    using result_type = T;
    using param_type = parameters;

    constexpr box_muller_base() = default;
    constexpr box_muller_base(param_type const& params) noexcept
        : m_params(params) {}

    constexpr void reset() noexcept(box_muller_base{}.m_fp_dist.reset()) {
        m_have_cache = false;
        m_fp_dist.reset();
    }

    constexpr auto param() const -> param_type { return m_params; }
    constexpr auto param(param_type const& params) { m_params = params; }

    template<concepts::uniform_random_bit_generator G>
    constexpr auto generate_two_bm(G& gen) -> std::pair<T, T> {
        T x_0 = T(1) - m_fp_dist(gen);  // (0, 1]
        T x_1 = T(1) - m_fp_dist(gen);  // (0, 1]

        constexpr T sigma = 1;
        constexpr T mu = 0;

        T r = sigma * std::sqrt(T(-2) * std::log(x_0));

        T z_0 = mu + r * std::cos(std::numbers::pi_v<T> * 2 * x_1);
        T z_1 = mu + r * std::sin(std::numbers::pi_v<T> * 2 * x_1);

        return {z_0, z_1};
    }

    template<concepts::uniform_random_bit_generator G>
    constexpr auto generate_two_marsaglia(G& gen) -> std::pair<T, T> {
        T x;
        T y;
        T s;

        do {
            x = m_fp_dist(gen) * 2 - 1;
            y = m_fp_dist(gen) * 2 - 1;
            s = x * x + y * y;
        } while(s >= 1 || s == 0);

        T r = std::sqrt(std::log(s) * -2 / s);

        constexpr T sigma = 1;
        constexpr T mu = 0;

        T z_0 = mu + sigma * x * r;
        T z_1 = mu + sigma * y * r;

        return {z_0, z_1};
    }

    template<concepts::uniform_random_bit_generator G>
    constexpr auto operator()(G& gen) -> result_type {
        T ret;

        if (!m_have_cache) {
            auto [z_0, z_1] = Polar ? generate_two_marsaglia(gen) : generate_two_bm(gen);
            ret = z_0;
            m_cache = z_1;
        } else {
            ret = m_cache;
        }

        m_have_cache ^= true;

        return ret;
    }

    template<concepts::uniform_random_bit_generator G>
    constexpr auto operator()(G& generator, param_type) -> result_type {
        return (*this)(generator);
    }

    constexpr auto min() -> result_type { return std::numeric_limits<T>::min(); }

    constexpr auto max() -> result_type { return std::numeric_limits<T>::max(); }

    constexpr auto operator==(box_muller_base const& other) const -> bool {
        return other.param() == param();
    }

private:
    param_type m_params{};

    T m_cache;
    bool m_have_cache = false;

    erand48_distribution<T> m_fp_dist{{true}};
};

}

template<std::floating_point T = double>
using normal_distribution_box_muller = detail::box_muller_base<T, false>;

template<std::floating_point T = double>
using normal_distribution_marsaglia = detail::box_muller_base<T, true>;


}  // namespace stf::random
