#pragma once

#include <stuff/blas/detail/concepts.hpp>
#include <stuff/core.hpp>

namespace stf::blas {

template<typename T, usize Size>
struct vector {
    using value_type = T;
    inline static constexpr usize size = Size;

    template<typename U>
    using rebind = vector<U, Size>;

    template<typename U, usize N>
    using rebind_n = vector<U, N>;

    T m_data[Size];

    constexpr auto operator[](usize i) const& -> T const& { return m_data[i]; }
    constexpr auto operator[](usize i) & -> T& { return m_data[i]; }
    constexpr auto operator[](usize i) const&& -> T const&& { return std::move(m_data[i]); }
    constexpr auto operator[](usize i) && -> T&& { return std::move(m_data[i]); }
};

template<usize I, typename T, usize Size>
    requires(I < Size)
constexpr auto get(vector<T, Size> const& vec) -> typename vector<T, Size>::value_type {
    return vec[I];
}

static_assert(concepts::vector<vector<int, 3>>);

/// @return
/// The dot product between <code>lhs</code> and <code>rhs</code>.
/// Return type is determined based on promotion rules.
template<concepts::vector T, concepts::vector U>
constexpr auto dot(T const& lhs, U const& rhs)
  -> core::type_promotion_t<typename T::value_type, typename U::value_type>;

/// @return
/// The cross product of <code>lhs</code> and <code>rhs</code> or a
/// vector-expression equivalent to it.
/// Only valid for spaces of R³ and R⁷.
template<concepts::vector T, concepts::vector U>
constexpr auto cross(T const& lhs, U const& rhs) -> concepts::nd_vector<T::size> auto;

/// @return
/// The elementwise addition of <code>lhs</code> and <code>rhs</code> or a
/// vector-expression equivalent to it.
template<concepts::vector T, concepts::vector U>
constexpr auto operator+(T const& lhs, U const& rhs) -> concepts::nd_vector<T::size> auto;

/// @return
/// The negation of <code>v</code> or a vector-expression equivalent to it.
template<concepts::vector T>
constexpr auto operator-(T const& v) -> concepts::nd_vector_of_t<typename T::value_type, T::size> auto;

/// @return
/// The elementwise subtraction of <code>lhs</code> and <code>rhs</code> or a
/// vector-expression equivalent to it.
template<concepts::vector T, concepts::vector U>
constexpr auto operator-(T const& lhs, U const& rhs) -> concepts::nd_vector<T::size> auto;

template<concepts::vector T>
constexpr auto operator/(T const& lhs, typename T::value_type rhs) -> concepts::nd_vector<T::size> auto;

template<concepts::vector T>
constexpr auto operator*(T const& lhs, typename T::value_type rhs) -> concepts::nd_vector<T::size> auto;

template<concepts::vector T>
constexpr auto abs(T const& v) -> typename T::value_type;

template<concepts::vector T>
constexpr auto normalize(T const& v) -> concepts::nd_vector_of_t<typename T::value_type, T::size> auto;

/// @return
/// The elementwise comparison of <code>lhs</code> and <code>rhs</code>.\n
/// If all elements of <code>lhs</code> are in the same relation to the corresponding elements in <code>rhs</code>
/// some std::partial_ordering inequivalent to std::partial_ordering::unordered is returned, otherwise
/// std::partial_ordering::unordered is returned.\n
/// if T::size != U::size, std::partial_ordering::unordered is returned.\n
/// Not recommended for vectors of floating-point value_type.\n
template<concepts::vector T, concepts::vector U>
constexpr auto operator<=>(T const& lhs, U const& rhs) -> std::partial_ordering;

/// Checks the equality of two vectors; <code>lhs</code> and <code>rhs</code>.
/// Not recommended for vectors of floating-point value_type.
constexpr auto operator==(concepts::vector auto const& lhs, concepts::vector auto const& rhs) -> bool {
    return lhs <=> rhs == std::partial_ordering::equivalent;
}

/// GLSL-like vector swizzling.\n
/// x, r, and s correspond to index 0\n
/// y, g, and t correspond to index 1\n
/// z, b, and p correspond to index 2\n
/// w, a, and q correspond to index 3\n
/// @return
/// The swizzled vector.
/// @example
/// <pre>static_assert(swizzle\<"wztsrgzw">(vector\<int, 4>{0,1,2,3}) == vector\<int, 8>{3,2,1,0,0,1,2,3})</pre>
template<string_literal Lit, concepts::vector T>
constexpr auto swizzle(T const& vec) -> concepts::nd_vector_of_t<typename T::value_type, Lit.size()> auto;

}  // namespace stf::blas

#include <stuff/blas/detail/vecops.ipp>
