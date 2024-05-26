#pragma once

#include <stuff/blas/vector.hpp>

#include <ranges>

namespace stf::blas {

namespace concepts {

template<typename T>
concept matrix = requires(T const& cv) {
    typename T::value_type;

    { cv[0uz] } -> concepts::vector;
    { cv.column(0uz) } -> concepts::vector;
    { T::rows } -> std::convertible_to<usize>;
    { T::cols } -> std::convertible_to<usize>;

    typename std::integral_constant<usize, T::rows>;
    typename std::integral_constant<usize, T::cols>;
};

}

template<typename T, usize Cols>
struct matrix_row_adapter {
    using value_type = T;

    template<typename U, usize N>
    using rebind = vector<U, N>;

    inline static constexpr usize size = Cols;

    constexpr matrix_row_adapter() noexcept = delete;

    constexpr matrix_row_adapter(std::span<T, Cols> data)
        : m_data(data) {}

    constexpr auto operator[](usize i) const noexcept -> T const& { return m_data[i]; }
    constexpr auto operator[](usize i) noexcept -> T& { return m_data[i]; }

private:
    std::span<T, Cols> m_data;
};

template<typename T, usize Rows, usize Cols>
struct matrix_col_adapter {
    using value_type = T;

    template<typename U, usize N>
    using rebind = vector<U, N>;

    inline static constexpr usize size = Rows;

    constexpr matrix_col_adapter() = delete;

    template<usize Extent = std::dynamic_extent>
    constexpr matrix_col_adapter(std::span<T, Extent> data)
        : m_data(data) {}

    constexpr auto operator[](usize i) const noexcept -> T const& { return m_data[Cols * i]; }
    constexpr auto operator[](usize i) noexcept -> T& { return m_data[Cols * i]; }

private:
    std::span<T> m_data;
};

template<typename T, usize Rows, usize Cols>
struct matrix {
    using value_type = T;
    inline static constexpr usize rows = Rows;
    inline static constexpr usize cols = Cols;

    template<typename U, usize RRows, usize CCols>
    using rebind = matrix<U, RRows, CCols>;

    constexpr matrix() noexcept = default;

    constexpr matrix(std::initializer_list<T> il) { std::ranges::copy(il, m_data); }

    template<concepts::matrix Mat>
        requires(!std::is_same_v<Mat, matrix>) && (Mat::rows == Rows && Mat::cols == Cols && std::is_convertible_v<typename Mat::value_type, T>)
    constexpr matrix(Mat&& mat) noexcept {
        for (auto i : std::views::iota(0uz, Rows)) {
            for (auto j : std::views::iota(0uz, Cols)) {
                std::construct_at(&m_data[j + i * Cols], std::move(mat[i][j]));
            }
        }
    }

    constexpr auto operator[](usize i) const noexcept { return matrix_row_adapter(std::span<const T, Cols>(&m_data[Cols * i], Cols)); }
    constexpr auto operator[](usize i) noexcept { return matrix_row_adapter(std::span<T, Cols>(&m_data[Cols * i], Cols)); }

    constexpr auto column(usize i) const noexcept { return matrix_col_adapter<const T, Rows, Cols>(std::span<const T>(m_data + i, Rows * Cols - i)); }
    constexpr auto column(usize i) noexcept { return matrix_col_adapter<T, Rows, Cols>(std::span<T>(m_data + i, Rows * Cols - i)); }

private:
    T m_data[Rows * Cols];
};

template<concepts::matrix Mat>
constexpr auto materialise(Mat mat) {
    return typename Mat::template rebind<typename Mat::value_type, Mat::rows, Mat::cols>{std::move(mat)};
}

template<typename T, usize Rows, usize Cols>
constexpr auto materialise(matrix<T, Rows, Cols> mat) {
    return mat;
}

template<concepts::matrix Lhs, concepts::matrix Rhs>
    requires(Lhs::rows == Rhs::rows && Lhs::cols == Rhs::cols) && std::equality_comparable_with<typename Lhs::value_type, typename Rhs::value_type>
constexpr auto operator==(Lhs const& lhs, Rhs const& rhs) -> bool {
    for (auto i : std::views::iota(0uz, Lhs::rows)) {
        for (auto j : std::views::iota(0uz, Lhs::cols)) {
            if (lhs[i][j] != rhs[i][j]) {
                return false;
            }
        }
    }
    return true;
}

namespace expressions {

template<typename T, usize Rows, usize Cols, typename Fun>
struct lazy_expr_matrix;

template<typename T, usize Rows, usize Cols, typename Fun>
struct lazy_expr_row_helper {
    using value_type = T;
    inline static constexpr usize size = Cols;

    template<typename U, usize N>
    using rebind = vector<U, N>;

    using matrix_type = lazy_expr_matrix<T, Rows, Cols, Fun>;

    lazy_expr_row_helper() = delete;

    constexpr lazy_expr_row_helper(matrix_type const& mat, usize row)
        : m_mat(mat)
        , m_row(row) {}

    constexpr auto operator[](usize col) const noexcept;

private:
    matrix_type const& m_mat;
    usize m_row;
};

template<typename T, usize Rows, usize Cols, typename Fun>
struct lazy_expr_col_helper {
    using value_type = T;
    inline static constexpr usize size = Cols;

    template<typename U, usize N>
    using rebind = vector<U, N>;

    using matrix_type = lazy_expr_matrix<T, Rows, Cols, Fun>;

    lazy_expr_col_helper() = delete;

    constexpr lazy_expr_col_helper(matrix_type const& mat, usize col)
        : m_mat(mat)
        , m_col(col) {}

    constexpr auto operator[](usize row) const noexcept;

private:
    matrix_type const& m_mat;
    usize m_col;
};

/// "lazy" as in i was too lazy to make more specialised ones
template<typename T, usize Rows, usize Cols, typename Fun>
struct lazy_expr_matrix {
    friend struct lazy_expr_row_helper<T, Rows, Cols, Fun>;
    friend struct lazy_expr_col_helper<T, Rows, Cols, Fun>;

    using value_type = T;
    inline static constexpr usize rows = Rows;
    inline static constexpr usize cols = Cols;

    template<typename U, usize CCols, usize RRows>
    using rebind = matrix<U, CCols, RRows>;

    constexpr lazy_expr_matrix(Fun&& fun)
        : m_fun(std::move(fun)) {}

    constexpr auto operator[](usize row) const noexcept { return lazy_expr_row_helper(*this, row); }

    constexpr auto column(usize col) const noexcept { return lazy_expr_col_helper(*this, col); }

private:
    Fun m_fun;
};

template<typename T, usize Rows, usize Cols, typename Fun>
constexpr auto lazy_expr_row_helper<T, Rows, Cols, Fun>::operator[](usize col) const noexcept {
    return m_mat.m_fun(m_row, col);
}

template<typename T, usize Rows, usize Cols, typename Fun>
constexpr auto lazy_expr_col_helper<T, Rows, Cols, Fun>::operator[](usize row) const noexcept {
    return m_mat.m_fun(row, m_col);
}

}  // namespace expressions

template<concepts::matrix Mat>
constexpr auto operator-(Mat mat) {
    auto lambda = [mat](usize row, usize col) { return -mat[row][col]; };
    return expressions::lazy_expr_matrix<typename Mat::value_type, Mat::rows, Mat::cols, typeof(lambda)>{std::move(lambda)};
}

template<concepts::matrix Mat>
constexpr auto transpose(Mat mat) {
    auto lambda = [mat](usize row, usize col) { return mat[col][row]; };
    return expressions::lazy_expr_matrix<typename Mat::value_type, Mat::cols, Mat::rows, typeof(lambda)>{std::move(lambda)};
}

template<concepts::matrix Lhs, concepts::matrix Rhs>
    requires(Lhs::rows == Rhs::cols) && requires(typename Lhs::value_type const& lhs, typename Rhs::value_type const& rhs) {
        lhs* rhs;
        lhs + rhs;
    }
constexpr auto operator*(Lhs const& lhs, Rhs const& rhs) {
    /* [a b] * [x y z w]
     * [c d]   [p q r s]
     * [e f]
     *
     */

    using result_type = typeof(std::declval<typename Lhs::value_type>() * std::declval<typename Rhs::value_type>());

    auto ret = typename Lhs::template rebind<result_type, Lhs::rows, Rhs::cols> {};

    for (auto row : std::views::iota(0uz, Lhs::rows)) {
        for (auto col : std::views::iota(0uz, Rhs::cols)) {
            auto sum = lhs[row][0] * rhs[0][col];
            for (auto i : std::views::iota(1uz, Lhs::cols)) {
                sum += lhs[row][i] * rhs[i][col];
            }
            ret[row][col] = sum;
        }
    }

    return ret;
}

template<concepts::matrix Lhs, concepts::vector Rhs>
    requires(Lhs::cols == Rhs::size)
constexpr auto operator*(Lhs const& lhs, Rhs const& rhs) {
    using result_type = typeof(std::declval<typename Lhs::value_type>() * std::declval<typename Rhs::value_type>());

    auto ret = typename Rhs::template rebind<result_type, Lhs::rows> {};

    for (auto row : std::views::iota(0uz, Lhs::rows)) {
        auto sum = lhs[row][0] * rhs[0];
        for (auto col : std::views::iota(1uz, Lhs::cols)) {
            sum += lhs[row][col] * rhs[col];
        }
        ret[row] = sum;
    }

    return ret;
}

}  // namespace stf::blas
