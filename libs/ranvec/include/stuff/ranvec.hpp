#pragma once

#include <stuff/blas.hpp>
#include <stuff/random.hpp>

namespace stf::random {

template<size_t N>
struct sphere_sampler;

template<> struct sphere_sampler<1>;
template<> struct sphere_sampler<2>;

template<size_t N>
struct ball_sampler;

template<> struct ball_sampler<2>;
template<> struct ball_sampler<3>;

template<>
struct ball_sampler<2> {
    template<std::floating_point T, typename Gen>
    static constexpr auto rejection(Gen& gen) -> blas::vector<T, 2>;

    template<std::floating_point T, typename Gen>
    static constexpr auto radius(Gen& gen) -> blas::vector<T, 2>;

    template<std::floating_point T, typename Gen>
    static constexpr auto radial(Gen& gen) -> blas::vector<T, 2>;

    template<std::floating_point T, typename Gen>
    static constexpr auto concentric(Gen& gen) -> blas::vector<T, 2>;

    template<std::floating_point T, typename Gen>
    static constexpr auto sample(Gen& gen) -> blas::vector<T, 2> { return rejection<T>(gen); }
};

template<>
struct ball_sampler<3> {
    template<std::floating_point T, typename Gen>
    static constexpr auto rejection(Gen& gen) -> blas::vector<T, 3>;

    template<std::floating_point T, typename Gen>
    static constexpr auto radius(Gen& gen) -> blas::vector<T, 3>;

    template<std::floating_point T, typename Gen>
    static constexpr auto sample(Gen& gen) -> blas::vector<T, 3> { return radius<T>(gen); }
};

template<>
struct sphere_sampler<1> {
    template<std::floating_point T, typename Gen>
    static constexpr auto polar(Gen& gen) -> blas::vector<T, 2>;

    template<std::floating_point T, typename Gen>
    static constexpr auto rejection(Gen& gen) -> blas::vector<T, 2>;

    template<std::floating_point T, typename Gen>
    static constexpr auto normal(Gen& gen) -> blas::vector<T, 2>;

    template<std::floating_point T, typename Gen>
    static constexpr auto sample(Gen& gen) -> blas::vector<T, 2> { return rejection<T>(gen); }
};

template<>
struct sphere_sampler<2> {
    template<std::floating_point T, typename Gen>
    static constexpr auto polar_trig(Gen& gen) -> blas::vector<T, 3>;

    template<std::floating_point T, typename Gen>
    static constexpr auto polar(Gen& gen) -> blas::vector<T, 3>;

    template<std::floating_point T, typename Gen>
    static constexpr auto normal(Gen& gen) -> blas::vector<T, 3>;

    template<std::floating_point T, typename Gen>
    static constexpr auto rejection_marsaglia(Gen& gen) -> blas::vector<T, 3>;

    template<std::floating_point T, typename Gen>
    static constexpr auto rejection_cook(Gen& gen) -> blas::vector<T, 3>;

    template<std::floating_point T, typename Gen>
    static constexpr auto sample(Gen& gen) -> blas::vector<T, 3> { return rejection_marsaglia<T>(gen); }
};

}  // namespace stf::random

#include <stuff/ranvec/detail/ranvec.ipp>
