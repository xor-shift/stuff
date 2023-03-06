#pragma once

#include <stuff/random/concepts.hpp>
#include <stuff/random/util.hpp>
#include <stuff/sfloat.hpp>

#include <cmath>

namespace stf::random {

template<std::floating_point T = double>
    requires(std::is_same_v<T, float> || std::is_same_v<T, double>)
struct erand48_distribution {
    struct parameters {
        bool include_zero = true;

        friend constexpr auto operator==(parameters const& lhs, parameters const& rhs) {
            return lhs.include_zero == rhs.include_zero;
        }
    };

    using result_type = T;
    using param_type = parameters;

    constexpr erand48_distribution() = default;
    constexpr erand48_distribution(param_type const& params) noexcept
        : m_params(params) {}

    constexpr void reset() noexcept {}

    constexpr auto param() const -> param_type { return m_params; }
    constexpr auto param(param_type const& params) { m_params = params; }

    template<concepts::uniform_random_bit_generator G>
    constexpr auto operator()(G& generator) -> result_type {
        constexpr int bits_needed = std::is_same_v<T, double> ? 48 : 24;

        u64 res = get_entropy<u64, G>(generator, bits_needed);

        T temp;

        if constexpr (std::is_same_v<T, double>) {
            temp = std::ldexp(res, -bits_needed);
        } else {
            temp = std::ldexp(static_cast<T>(res), -bits_needed);
        }

        if (!m_params.include_zero) {
            temp = std::bit_cast<T>(nextafter(sfloat::make_soft<T>(temp), sfloat::make_soft<T>(-1)).get_repr());
        }

        return temp;
    }

    template<concepts::uniform_random_bit_generator G>
    constexpr auto operator()(G& generator, param_type) -> result_type {
        return (*this)(generator);
    }

    constexpr auto min() -> result_type {
        if (m_params.include_zero) {
            return 0;
        }

        return std::bit_cast<T>(nextafter(sfloat::make_soft<T>(0), sfloat::make_soft<T>(1)).get_repr());
    }

    constexpr auto max() -> result_type {
        if (m_params.include_zero) {
            return std::bit_cast<T>(nextafter(sfloat::make_soft<T>(1), sfloat::make_soft<T>(0)).get_repr());
        }

        return 1;
    }

    constexpr auto operator==(erand48_distribution const& other) const -> bool { return other.param() == param(); }

private:
    parameters m_params {};
};

}
