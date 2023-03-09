#pragma once

namespace stf::random::detail {

template<std::floating_point T, size_t N, typename Gen>
constexpr auto sample_n_sphere_normal(Gen& gen) -> blas::vector<T, N + 1> {
    normal_distribution_marsaglia<T> dist{};

    blas::vector<T, N + 1> ret;

    for (usize i = 0; i <= N; i++) {
        ret[i] = dist(gen);
    }

    return normalize(ret);
}

template<std::floating_point T, size_t N, typename Gen>
constexpr auto sample_n_ball_rejection(Gen& gen) -> blas::vector<T, N> {
    erand48_distribution<T> dist{};

    blas::vector<T, N> ret;

    do {
        for (usize i = 0; i < N; i++) {
            ret[i] = 2 * dist(gen) - 1;
        }
    } while (abs(ret) > 1);

    return ret;
}

}

// ball_sampler<2> functions

namespace stf::random {

template<std::floating_point T, typename Gen>
constexpr auto ball_sampler<2>::rejection(Gen& gen) -> blas::vector<T, 2> {
    return detail::sample_n_ball_rejection<T, 2>(gen);
}

template<std::floating_point T, typename Gen>
constexpr auto ball_sampler<2>::radius(Gen& gen) -> blas::vector<T, 2> {
    erand48_distribution<T> dist{};
    return sphere_sampler<1>::sample<T>(gen) * std::sqrt(dist(gen));
}

template<std::floating_point T, typename Gen>
constexpr auto ball_sampler<2>::radial(Gen& gen) -> blas::vector<T, 2> {
    erand48_distribution<T> dist{};

    T r = std::sqrt(dist(gen));
    T theta = dist(gen) * 2 * std::numbers::pi_v<T>;

    return {r * std::cos(theta), r * std::sin(theta)};
}

template<std::floating_point T, typename Gen>
constexpr auto ball_sampler<2>::concentric(Gen& gen) -> blas::vector<T, 2> {
    erand48_distribution<T> dist{};

    T x = 2 * dist(gen) - 1;
    T y = 2 * dist(gen) - 1;

    bool xgty = std::abs(x) > std::abs(y);

    T r = xgty ? x : y;
    T theta = xgty ? std::numbers::pi_v<T> / 4 * y / x : std::numbers::pi_v<T> / 2 - std::numbers::pi_v<T> / 4 * x / y;

    return {r * std::cos(theta), r * std::sin(theta)};
}

}

// ball_sampler<3> functions

namespace stf::random {

template<std::floating_point T, typename Gen>
constexpr auto ball_sampler<3>::rejection(Gen& gen) -> blas::vector<T, 3> {
    return detail::sample_n_ball_rejection<T, 3>(gen);
}

template<std::floating_point T, typename Gen>
constexpr auto ball_sampler<3>::radius(Gen& gen) -> blas::vector<T, 3> {
    erand48_distribution<T> dist{};
    return sphere_sampler<2>::sample<T>(gen) * std::cbrt(dist(gen));
}

}

// sphere_sampler<1> functions

namespace stf::random {

template<std::floating_point T, typename Gen>
constexpr auto sphere_sampler<1>::polar(Gen& gen) -> blas::vector<T, 2> {
    erand48_distribution<T> dist{};

    std::floating_point auto theta = dist(gen);
    theta *= 2 * std::numbers::pi;

    return {std::cos(theta), std::sin(theta)};
}

template<std::floating_point T, typename Gen>
constexpr auto sphere_sampler<1>::rejection(Gen& gen) -> blas::vector<T, 2> {
    erand48_distribution<T> dist{};

    T u;
    T v;
    T r2;

    do {
        u = dist(gen) * 2 - 1;
        v = dist(gen) * 2 - 1;
        r2 = u * u + v * v;
    } while (r2 > 1 || r2 == 0);

    return {(u - v) * (u + v) / r2, 2 * u * v / r2};
}

template<std::floating_point T, typename Gen>
constexpr auto sphere_sampler<1>::normal(Gen& gen) -> blas::vector<T, 2> {
    return detail::sample_n_sphere_normal<T, 1>(gen);
}

}

// sphere_sampler<2> functions

namespace stf::random {

template<std::floating_point T, typename Gen>
constexpr auto sphere_sampler<2>::polar_trig(Gen& gen) -> blas::vector<T, 3> {
    erand48_distribution<T> dist{};

    T theta = dist(gen) * 2 * std::numbers::pi_v<T>;
    T phi = std::acos(2 * dist(gen) - 1);

    T st = std::sin(theta);
    T ct = std::cos(theta);
    T sp = std::sin(phi);
    T cp = std::cos(phi);

    return {st * sp, ct * sp, cp};
}

template<std::floating_point T, typename Gen>
constexpr auto sphere_sampler<2>::polar(Gen& gen) -> blas::vector<T, 3> {
    erand48_distribution<T> dist{};

    T theta = dist(gen) * 2 * std::numbers::pi_v<T>;
    T cp = 2 * dist(gen) - 1;

    T sp = std::sqrt(1 - cp * cp);
    T st = std::sin(theta);
    T ct = std::cos(theta);

    return {st * sp, ct * sp, cp};
}

template<std::floating_point T, typename Gen>
constexpr auto sphere_sampler<2>::normal(Gen& gen) -> blas::vector<T, 3> {
    return detail::sample_n_sphere_normal<T, 2>(gen);
}

template<std::floating_point T, typename Gen>
constexpr auto sphere_sampler<2>::rejection_marsaglia(Gen& gen) -> blas::vector<T, 3> {
    erand48_distribution<T> dist{};

    T u;
    T v;
    T r2;

    do {
        u = 2 * dist(gen) - 1;
        v = 2 * dist(gen) - 1;
        r2 = u * u + v * v;
    } while (r2 > 1);

    T x = 2 * u * std::sqrt(1 - r2);
    T y = 2 * v * std::sqrt(1 - r2);
    T z = 1 - 2 * r2;

    return {x, y, z};
}

template<std::floating_point T, typename Gen>
constexpr auto sphere_sampler<2>::rejection_cook(Gen& gen) -> blas::vector<T, 3> {
    erand48_distribution<T> dist{};

    T s;
    T t;
    T u;
    T v;
    T r2;

    do {
        s = 2 * dist(gen) - 1;
        t = 2 * dist(gen) - 1;
        u = 2 * dist(gen) - 1;
        v = 2 * dist(gen) - 1;
        r2 = s * s + t * t + u * u + v * v;
    } while (r2 > 1 || r2 == 0);

    T x = 2 * (s * u + t * v) / r2;
    T y = 2 * (u * v - s * t) / r2;
    T z = (s * s - t * t + u * u - v * v) / r2;

    return {x, y, z};
}

}
