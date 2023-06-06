#pragma once

#include <stuff/blas/vector.hpp>

#include <cmath>
#include <span>

namespace stf::blas {

template<typename T, usize Rows, usize Cols>
struct matrix;

namespace detail {

template<typename T, usize Cols>
struct matrix_row {
    using value_type = std::remove_const_t<T>;
    inline static constexpr usize size = Cols;

    template<typename U>
    using rebind = vector<U, size>;

    template<typename U, usize N>
    using rebind_n = vector<U, N>;

    std::span<T, Cols> m_data;

    constexpr auto operator[](usize i) const -> T const& { return m_data[i]; }
    constexpr auto operator[](usize i) -> T& { return m_data[i]; }
};

template<usize I, typename T, usize Cols>
    requires(I < Cols)
constexpr auto get(matrix_row<T, Cols> const& v) -> typename matrix_row<T, Cols>::value_type {
    return v[I];
}

static_assert(concepts::vector<matrix_row<int, 3>>);

template<typename T, usize Rows, usize Cols>
struct matrix_column {
    using value_type = std::remove_const_t<T>;
    inline static constexpr usize size = Rows;

    template<typename U>
    using rebind = vector<U, size>;

    template<typename U, usize N>
    using rebind_n = vector<U, N>;

    // contains the entire matrix offset by a stride
    std::span<T> m_data;

    constexpr auto operator[](usize i) const -> T const& { return m_data[index(i)]; }
    constexpr auto operator[](usize i) -> T& { return m_data[index(i)]; }

    constexpr auto index(usize row) const -> usize { return row * Cols; }
};

template<usize I, typename T, usize Rows, usize Cols>
    requires(I < Rows)
constexpr auto get(matrix_column<T, Rows, Cols> const& v) -> typename matrix_row<T, Cols>::value_type {
    return v[I];
}

static_assert(concepts::vector<matrix_column<int, 3, 4>>);

template<concepts::vector Vector>
struct vector_matrix_adapter {
    using value_type = typename Vector::value_type;
    inline static constexpr usize rows = Vector::size;
    inline static constexpr usize cols = 1;

    template<typename U>
    using rebind = matrix<U, rows, cols>;

    template<typename U, usize NRows, usize NCols>
    using rebind_n = matrix<U, NRows, NCols>;

    template<typename U, usize Size>
    using preferred_vector_type = vector<U, Size>;

    Vector const& m_vec;

    constexpr auto operator[](usize i) const -> concepts::generic_vector<value_type, 1> auto{
        return typename Vector::template rebind<value_type, 1>{m_vec[i]};
    }

    constexpr auto column(usize i) const -> concepts::generic_vector<value_type, rows> auto{ return m_vec; }
};

static_assert(concepts::matrix<vector_matrix_adapter<vector<int, 3>>>);

}  // namespace detail

/// A simple row-major matrix.
template<typename T, usize Rows, usize Cols>
struct matrix {
    using value_type = T;
    inline static constexpr usize rows = Rows;
    inline static constexpr usize cols = Cols;

    template<typename U>
    using rebind = matrix<U, Rows, Cols>;

    template<typename U, usize NRows, usize NCols>
    using rebind_n = matrix<U, NRows, NCols>;

    template<typename U, usize Size>
    using preferred_vector_type = vector<U, Size>;

    T m_data[Rows * Cols];

    constexpr auto operator[](usize i) const -> detail::matrix_row<const T, Cols> {
        return {std::span<const T, Cols>(m_data + Cols * i, Cols)};
    }

    constexpr auto operator[](usize i) -> detail::matrix_row<T, Cols> {
        return {std::span<T, Cols>(m_data + Cols * i, Cols)};
    }

    constexpr auto column(usize i) const -> detail::matrix_column<const T, Rows, Cols> {
        return {std::span<const T>(m_data + i, Rows * Cols - i)};
    }

    constexpr auto column(usize i) -> detail::matrix_column<T, Rows, Cols> {
        return {std::span<T>(m_data + i, Rows * Cols - i)};
    }

    static constexpr auto identity() -> matrix requires(Rows == Cols);

    static constexpr auto rotate(T theta) -> matrix requires(Rows == Cols && Rows == 2);

    /// Rotation matrix in 3 dimensions.
    /// For a vector [0, 0, 1]; x is pitch, y is yaw, z is roll
    static constexpr auto rotate(T x, T y, T z) -> matrix requires(Rows == Cols && Rows == 3);

    /// Homogenous rotation matrix in 3 dimensions.
    /// For a vector [0, 0, 1]; x is pitch, y is yaw, z is roll
    static constexpr auto rotate(T x, T y, T z) -> matrix requires(Rows == Cols && Rows == 4);

    static constexpr auto scale(T x, T y) -> matrix requires(Rows == Cols && Rows == 2);
    static constexpr auto scale(T x, T y) -> matrix requires(Rows == Cols && Rows == 3);
    static constexpr auto scale(T x, T y, T z) -> matrix requires(Rows == Cols && Rows == 3);
    static constexpr auto scale(T x, T y, T z) -> matrix requires(Rows == Cols && Rows == 4);

    static constexpr auto translate(T x, T y) -> matrix requires(Rows == Cols && Rows == 3);
    static constexpr auto translate(T x, T y, T z) -> matrix requires(Rows == Cols && Rows == 4);
};

static_assert(concepts::matrix<matrix<int, 3, 4>>);
static_assert(concepts::matrix_backend<matrix<int, 3, 4>>);

/*template<typename T, usize Size, template<typename U, usize Rows, usize Cols> class matrix_type = matrix>
    requires concepts::matrix_backend<matrix_type<T, Size, Size>>
constexpr auto identity_matrix() -> matrix_type<T, Size, Size>;

template<typename T, template<typename U, usize Rows, usize Cols> class matrix_type = matrix>
    requires concepts::matrix_backend<matrix_type<T, 2, 2>>
constexpr auto rotation_matrix(T theta) -> matrix_type<T, 2, 2>;

/// Rotation matrix in 3 dimensions.
/// For a vector [0, 0, 1]; x is pitch, y is yaw, z is roll
template<typename T, template<typename U, usize Rows, usize Cols> class matrix_type = matrix>
    requires concepts::matrix_backend<matrix_type<T, 3, 3>>
constexpr auto rotation_matrix(T x, T y, T z) -> matrix_type<T, 3, 3>;*/

/// @return
/// The elementwise addition of <code>lhs</code> and <code>rhs</code> or a
/// matrix-expression equivalent to it.
template<concepts::matrix T, concepts::matrix U>
    requires(T::rows == U::rows && T::cols == U::cols)
constexpr auto operator+(T const& lhs, U const& rhs) -> concepts::nd_matrix<T::cols, T::rows> auto;

template<concepts::matrix T>
constexpr auto operator-(T const& v) -> concepts::nd_matrix_of_t<typename T::value_type, T::rows, T::cols> auto;

/// @return
/// The elementwise subtraction of <code>lhs</code> and <code>rhs</code> or a
/// matrix-expression equivalent to it.
template<concepts::matrix T, concepts::matrix U>
    requires(T::rows == U::rows && T::cols == U::cols)
constexpr auto operator-(T const& lhs, U const& rhs) -> concepts::nd_matrix<T::cols, T::rows> auto;

template<concepts::matrix T, concepts::matrix U>
    requires(T::cols == U::rows)
constexpr auto operator*(T const& lhs, U const& rhs) -> concepts::nd_matrix<T::rows, U::cols> auto;

template<concepts::matrix T, concepts::vector U>
    requires(T::cols == U::size)
constexpr auto operator*(T const& lhs, U const& rhs) -> concepts::nd_vector<T::rows> auto;

template<concepts::vector T, concepts::matrix U>
    requires(T::suze == U::rows)
constexpr auto operator*(T const& lhs, U const& rhs) -> concepts::nd_vector<T::rows> auto{
    return detail::vector_matrix_adapter{lhs} * rhs;
}

template<concepts::matrix T, concepts::matrix U>
constexpr auto operator<=>(T const& lhs, U const& rhs) -> std::partial_ordering;

constexpr auto operator==(concepts::matrix auto const& lhs, concepts::matrix auto const& rhs) -> bool {
    return lhs <=> rhs == std::partial_ordering::equivalent;
}

}  // namespace stf::blas

#include <stuff/blas/detail/matops.ipp>
