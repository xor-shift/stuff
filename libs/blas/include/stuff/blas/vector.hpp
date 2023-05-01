#pragma once

#include <stuff/blas/detail/concepts.hpp>
#include <stuff/core.hpp>

#include <cmath>
#include <functional>

#ifndef STF_BLAS_INCLUDE_FUNCTIONS_FOR_PLAIN_VECTOR
# define STF_BLAS_INCLUDE_FUNCTIONS_FOR_PLAIN_VECTOR 0
#endif

namespace stf::blas {

template<typename T, usize Size>
struct vector {
    using value_type = T;
    inline static constexpr usize size = Size;

    template<typename U = T, usize N = Size>
    using rebind = vector<U, N>;

    constexpr vector() = default;

    template<typename V>
        requires(std::is_arithmetic_v<V>)
    constexpr vector(V v) noexcept {
        std::fill_n(m_data, Size, static_cast<T>(v));
    }

    /*template<typename V, typename... Vs>
        requires(std::is_arithmetic_v<V>) && (std::is_arithmetic_v<Vs> && ...)
    constexpr vector(V v, Vs... vs) noexcept {
        T temp[] = {static_cast<T>(v), static_cast<T>(vs)...};
        std::copy_n(temp, std::min(std::size(temp), Size), m_data);
    }*/

    template<typename V, typename... Vs>
        requires(std::is_arithmetic_v<V> || concepts::vector<V>) &&             //
                ((std::is_arithmetic_v<Vs> || concepts::vector<Vs>) && ...) &&  //
                (sizeof...(Vs) != 0 || concepts::vector<V>)
    constexpr vector(V const& v, Vs const&... vs) noexcept {
        init_from_mixed_args(0, v, vs...);
    }

    template<typename V>
        requires std::is_arithmetic_v<V>
    constexpr vector(std::initializer_list<V> il) noexcept {
        std::copy_n(il.begin(), std::min(il.size(), Size), m_data);
    }

    constexpr auto operator[](usize i) const& -> T const& { return m_data[i]; }
    constexpr auto operator[](usize i) & -> T& { return m_data[i]; }
    constexpr auto operator[](usize i) const&& -> T const&& { return std::move(m_data[i]); }
    constexpr auto operator[](usize i) && -> T&& { return std::move(m_data[i]); }
    constexpr auto data() -> T* { return m_data; }
    constexpr auto data() const -> const T* { return m_data; }
    constexpr auto begin() -> T* { return data(); }
    constexpr auto begin() const -> const T* { return data(); }
    constexpr auto end() -> T* { return data() + size; }
    constexpr auto end() const -> const T* { return data() + size; }

    friend constexpr void swap(vector& lhs, vector& rhs) {
        for (usize i = 0; i < Size; i++) {
            using std::swap;
            swap(lhs.m_data[i], rhs.m_data[i]);
        }
    }

private:
    T m_data[Size];

    template<typename V, typename... Vs>
    constexpr void init_from_mixed_args(usize i, V const& v, Vs const&... vs) {
        if constexpr (std::is_arithmetic_v<V>) {
            m_data[i] = v;
            ++i;
        } else {
            usize to_emit = std::min(V::size, Size - i);
            for (usize j = 0; j < to_emit; j++) {
                m_data[i + j] = static_cast<T>(v[j]);
            }
            i += to_emit;
        }

        if constexpr (sizeof...(Vs) == 0) {
            return;
        } else {
            if (i >= Size) {
                return;
            } else {
                return init_from_mixed_args<Vs...>(i, vs...);
            }
        }
    }
};

template<usize I, typename T, usize Size>
    requires(I < Size)
constexpr auto get(vector<T, Size> const& vec) -> typename vector<T, Size>::value_type {
    return vec[I];
}

static_assert(concepts::generic_vector<vector<int, 3>, int, 3>);
static_assert(concepts::vector_backend<vector<int, 3>>);

}  // namespace stf::blas

# include <stuff/blas/detail/generic_vec/generic_vec_ops.hpp>

#if STF_BLAS_INCLUDE_FUNCTIONS_FOR_PLAIN_VECTOR
# include <stuff/blas/detail/plain_vec/plain_vec_ops.hpp>
#endif
