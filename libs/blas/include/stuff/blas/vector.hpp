#pragma once

#include <stuff/core.hpp>

#include <cmath>
#include <ranges>

namespace stf::blas::concepts {

template<typename T>
concept vector = requires(T const& cv) {
    typename T::value_type;
    typename T::template rebind<float, 3>;

    { cv[0uz] } -> std::convertible_to<typename T::value_type>;
    { T::size } -> std::convertible_to<usize>;

    typename std::integral_constant<usize, T::size>;
};

}  // namespace stf::blas::concepts

namespace stf::blas {

template<typename T, usize N>
struct vector {
    using value_type = T;
    inline static constexpr usize size = N;

    template<typename U, usize M>
    using rebind = vector<U, N>;

    constexpr vector(vector const& other) noexcept = default;
    constexpr vector(vector&& other) noexcept = default;

    constexpr auto operator=(vector const& other) noexcept -> vector& {
        std::ranges::copy(other, m_data.begin());
        return *this;
    }

    constexpr vector() noexcept = default;

    template<typename U, typename... Us>
        requires(!std::is_same_v<std::remove_cvref_t<U>, vector>)
    constexpr vector(U&& arg, Us&&... args) noexcept {
        init_from_mixed_args<U, Us...>(0, std::forward<U>(arg), std::forward<Us>(args)...);
    }

    // constexpr vector(std::initializer_list<T> il) noexcept { std::ranges::copy(il, m_data); }

    template<concepts::vector Vec>
        requires(!std::is_same_v<Vec, vector>) &&  //
                (Vec::size == N) &&                //
                (std::is_constructible_v<T, typename Vec::value_type const&>)
    constexpr vector(Vec const& other) noexcept {
        for (auto const& [i, v] : other | std::views::enumerate) {
            std::construct_at(&m_data[i], v);
        }
    }

    constexpr explicit operator std::array<T, N>() const { return m_data; }

    constexpr operator std::pair<T, T>() const
        requires(N == 2)
    {
        return {m_data[0], m_data[1]};
    }

    constexpr auto begin() const noexcept -> const value_type* { return m_data.begin(); }
    constexpr auto end() const noexcept -> const value_type* { return m_data.end(); }
    constexpr auto begin() noexcept -> value_type* { return m_data.begin(); }
    constexpr auto end() noexcept -> value_type* { return m_data.end(); }

    constexpr auto operator[](usize i) const noexcept -> value_type const& { return m_data[i]; }
    constexpr auto operator[](usize i) noexcept -> value_type& { return m_data[i]; }

    template<typename Fun>
    constexpr auto map(Fun&& fun) const noexcept {
        auto ret = rebind<std::invoke_result_t<Fun, T const&>, N>{};
        for (auto const& [i, v] : *this | std::views::enumerate) {
            ret[i] = std::invoke(std::forward<Fun>(fun), v);
        }
        return ret;
    }

    template<concepts::vector Other, typename Fun>
        requires(Other::size == N) && std::is_invocable_v<Fun, T const&, typename Other::value_type const&>
    constexpr auto zip(Other const& other, Fun&& fun) const {
        auto ret = rebind<std::invoke_result_t<Fun, T const&, typename Other::value_type const&>, N>{};
        for (auto const& [i, v] : *this | std::views::enumerate) {
            ret[i] = std::invoke(std::forward<Fun>(fun), v, other[i]);
        }
        return ret;
    }

    // clang-format off
#define ARITH(sym, oper)                                                                               \
    template<concepts::vector Other>                                                                   \
        requires                                                                                       \
            (Other::size == N) &&                                                                      \
            requires(T const& lhs, typename Other::value_type const& rhs) { lhs sym rhs; }             \
    friend constexpr auto operator sym(vector const& lhs, Other const& rhs) noexcept {                 \
        return lhs.zip(rhs, [](T const& lhs, typename Other::value_type rhs) { return lhs sym rhs; }); \
    }                                                                                                  \
                                                                                                       \
    template<typename U>                                                                               \
        requires                                                                                       \
            (!concepts::vector<U>) &&                                                                  \
            requires(T const& lhs, U const& rhs) { lhs sym rhs; }                                      \
    friend constexpr auto operator sym(vector const& lhs, U const& rhs) noexcept {                     \
        return lhs.map([&rhs](T const& lhs) { return lhs sym rhs; });                                  \
    }                                                                                                  \
                                                                                                       \
    template<typename U>                                                                               \
        requires                                                                                       \
            (!concepts::vector<U>) &&                                                                  \
            requires(U const& lhs, T const& rhs) { lhs sym rhs; }                                      \
    friend constexpr auto operator sym(U const& lhs, vector const& rhs) noexcept {                     \
        return rhs.map([&lhs](T const& rhs) { return lhs sym rhs; });                                  \
    }

    // clang-format on

    ARITH(+, plus);
    ARITH(-, minus);
    ARITH(*, multiplies);
    ARITH(/, divides);

#undef ARITH

private:
    std::array<T, N> m_data;

    template<typename U, typename... Us>
    constexpr void init_from_mixed_args(usize i, U&& v, Us&&... rest) {
        auto increment = 0uz;
        if constexpr (requires(U const& v) { v.size(); }) {
            // using V = typename U::value_type;
            for (auto&& [j, v] : v | std::views::take(N) | std::views::enumerate) {
                increment += 1;
                std::construct_at(&m_data[i + j], std::move(v));
            }
        } else {
            std::construct_at(&m_data[i], std::forward<U>(v));
            increment = 1;
        }

        if constexpr (sizeof...(Us) != 0) {
            return init_from_mixed_args<Us...>(i + increment, std::forward<Us>(rest)...);
        }
    }
};

template<concepts::vector Lhs, concepts::vector Rhs>
    requires(Lhs::size == Rhs::size) && std::is_same_v<typename Lhs::value_type, typename Rhs::value_type> && (Lhs::size != 0)
constexpr auto operator<=>(Lhs const& lhs, Rhs const& rhs) {
    auto ret = lhs[0] <=> rhs[0];

    for (auto i : std::views::iota(1uz, Lhs::size)) {
        if constexpr (std::is_same_v<typeof(ret), std::partial_ordering>) {
            if (ret == std::partial_ordering::unordered || ret != std::partial_ordering::equivalent) {
                break;
            }
        } else {
            if (ret != std::strong_ordering::equivalent) {
                break;
            }
        }

        ret = lhs[i] <=> rhs[i];
    }

    return ret;
}

template<concepts::vector Lhs, concepts::vector Rhs>
    requires(Lhs::size == Rhs::size) && std::is_same_v<typename Lhs::value_type, typename Rhs::value_type> && (Lhs::size != 0)
constexpr auto operator==(Lhs const& lhs, Rhs const& rhs) {
    for (auto i : std::views::iota(0uz, Lhs::size)) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

template<concepts::vector Lhs, concepts::vector Rhs>
    requires(Lhs::size == Rhs::size)
constexpr auto dot(Lhs const& lhs, Rhs const& rhs) {
    auto ret = typeof(lhs[0] * rhs[0]){};

    for (auto i : std::views::iota(0uz, Lhs::size)) {
        ret += lhs[i] * rhs[i];
    }

    return ret;
}

template<concepts::vector Vec>
constexpr auto abs(Vec const& vec) {
    return vec.map([](auto const& v) { return std::abs(v); });
}

template<concepts::vector Vec>
constexpr auto length(Vec const& vec) {
    return std::sqrt(dot(vec, vec));
}

template<concepts::vector Vec>
constexpr auto normalize(Vec const& vec) {
    return vec / length(vec);
}

template<concepts::vector Lhs, concepts::vector Rhs>
    requires(Lhs::size == 3 && Rhs::size == 3)
constexpr auto cross(Lhs const& lhs, Rhs const& rhs) {
    using res_type = typeof(std::declval<typename Lhs::value_type>() * std::declval<typename Rhs::value_type>());

    // clang-format off
    return typename Lhs::template rebind<res_type, 3> {
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0],
    };
    // clang-format on
}

template<string_literal SwizzleString, concepts::vector Vec>
constexpr auto swizzle(Vec const& vec) {
    static_assert(
      ([]() {
          constexpr auto acceptable = std::string_view{"xyzwrgba"};
          for (char c : SwizzleString) {
              if (acceptable.find(c) == std::string_view::npos) {
                  return false;
              }
          }
          return true;
      })(),
      "a swizzling string must be made up of the characters within \"rgbaxyzw\""
    );

    auto ret = typename Vec::template rebind<typename Vec::value_type, SwizzleString.size() - 1>{};

    for (const auto& [i, c] : SwizzleString | std::views::enumerate) {
        switch (c) {
            case 'x': [[fallthrough]];
            case 'r': ret[i] = vec[0]; break;

            case 'y': [[fallthrough]];
            case 'g': ret[i] = vec[1]; break;

            case 'z': [[fallthrough]];
            case 'b': ret[i] = vec[2]; break;

            case 'w': [[fallthrough]];
            case 'a': ret[i] = vec[3]; break;

            default: std::unreachable();
        }
    }

    return ret;
}

namespace detail {

template<typename... Ts>
struct mixed_arg_helper;

template<typename T>
    requires(!requires { typename T::value_type; })
struct mixed_arg_helper<T> {
    inline static constexpr auto size = 1uz;
    using value_type = std::remove_cvref_t<T>;
};

template<typename T>
    requires requires { typename T::value_type; }
struct mixed_arg_helper<T> {
    inline static constexpr auto size = T::size();
    using value_type = T::value_type;
};

template<typename T, typename... Ts>
struct mixed_arg_helper<T, Ts...> {
    inline static constexpr auto size = mixed_arg_helper<T>::size + mixed_arg_helper<Ts...>::size;
    using value_type = std::common_type_t<typename mixed_arg_helper<T>::value_type, typename mixed_arg_helper<Ts...>::value_type>;
};

}  // namespace detail

template<typename... Ts>
vector(Ts&&...) -> vector<typename detail::mixed_arg_helper<Ts...>::value_type, detail::mixed_arg_helper<Ts...>::size>;

}  // namespace stf::blas

template<typename... Ts>
constexpr auto vec(Ts&&... args) {
    return stf::blas::vector{std::forward<Ts>(args)...};
}
