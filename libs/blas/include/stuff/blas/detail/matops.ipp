#pragma once

namespace stf::blas {

template<typename T, usize Size, template<typename U, usize Rows, usize Cols> class matrix_type>
    requires concepts::matrix_backend<matrix_type<T, Size, Size>>
constexpr auto identity_matrix() -> matrix_type<T, Size, Size> {
    matrix_type<T, Size, Size> ret{};

    for (usize i = 0; i < Size; i++) {
        ret[i][i] = 1;
    }

    return ret;
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::identity() -> matrix requires(Rows == Cols)
{
    matrix ret{};
    for (usize i = 0; i < Rows; i++) {
        ret[i][i] = 1;
    }
    return ret;
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::rotate(T theta) -> matrix<T, Rows, Cols>
    requires(Rows == Cols && Rows == 2)
{
    T s = static_cast<T>(std::sin(theta));
    T c = static_cast<T>(std::cos(theta));

    matrix<T, 2, 2> ret{c, -s, s, c};

    return ret;
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::rotate(T x, T y, T z) -> matrix<T, Rows, Cols>
    requires(Rows == Cols && Rows == 3)
{
    T xs = static_cast<T>(std::sin(x));
    T xc = static_cast<T>(std::cos(x));
    T ys = static_cast<T>(std::sin(y));
    T yc = static_cast<T>(std::cos(y));
    T zs = static_cast<T>(std::sin(z));
    T zc = static_cast<T>(std::cos(z));

    matrix<T, 3, 3> x_mat{1, 0, 0, 0, xc, -xs, 0, xs, xc};
    matrix<T, 3, 3> y_mat{yc, 0, ys, 0, 1, 0, -ys, 0, yc};
    matrix<T, 3, 3> z_mat{zc, -zs, 0, zs, zc, 0, 0, 0, 1};

    return x_mat * y_mat * z_mat;
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::rotate(T x, T y, T z) -> matrix<T, Rows, Cols>
    requires(Rows == Cols && Rows == 4)
{
    T xs = static_cast<T>(std::sin(x));
    T xc = static_cast<T>(std::cos(x));
    T ys = static_cast<T>(std::sin(y));
    T yc = static_cast<T>(std::cos(y));
    T zs = static_cast<T>(std::sin(z));
    T zc = static_cast<T>(std::cos(z));

    matrix<T, 4, 4> x_mat{1, 0, 0, 0, /**/ 0, xc, -xs, 0, /**/ 0, xs, xc, 0, /**/ 0, 0, 0, 1};
    matrix<T, 4, 4> y_mat{yc, 0, ys, 0, /**/ 0, 1, 0, 0, /**/ -ys, 0, yc, 0, /**/ 0, 0, 0, 1};
    matrix<T, 4, 4> z_mat{zc, -zs, 0, 0, /**/ zs, zc, 0, 0, /**/ 0, 0, 1, 0, /**/ 0, 0, 0, 1};

    return x_mat * y_mat * z_mat;
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::scale(T x, T y) -> matrix<T, Rows, Cols>
    requires(Rows == Cols && Rows == 2)
{
    return matrix<T, 2, 2>{x, 0, 0, y};
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::scale(T x, T y) -> matrix<T, Rows, Cols>
    requires(Rows == Cols && Rows == 3)
{
    return matrix<T, 3, 3>{
      x, 0, 0,  //
      0, y, 0,  //
      0, 0, 1   //
    };
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::scale(T x, T y, T z) -> matrix<T, Rows, Cols>
    requires(Rows == Cols && Rows == 3)
{
    return matrix<T, 3, 3>{
      x, 0, 0,  //
      0, y, 0,  //
      0, 0, z   //
    };
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::scale(T x, T y, T z) -> matrix<T, Rows, Cols>
    requires(Rows == Cols && Rows == 4)
{
    return matrix<T, 4, 4>{
      x, 0, 0, 0,  //
      0, y, 0, 0,  //
      0, 0, z, 0,  //
      0, 0, 0, 1   //
    };
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::translate(T x, T y) -> matrix<T, Rows, Cols>
    requires(Rows == Cols && Rows == 3)
{
    return matrix<T, 3, 3>{
      1, 0, x,  //
      0, 1, y,  //
      0, 0, 1   //
    };
}

template<typename T, usize Rows, usize Cols>
constexpr auto matrix<T, Rows, Cols>::translate(T x, T y, T z) -> matrix<T, Rows, Cols>
    requires(Rows == Cols && Rows == 4)
{
    return matrix<T, 4, 4>{
      1, 0, 0, x,  //
      0, 1, 0, y,  //
      0, 0, 1, z,  //
      0, 0, 0, 1   //
    };
}

namespace detail {

template<concepts::matrix T, concepts::matrix U, typename Fn>
    requires(T::rows == U::rows && T::cols == U::cols)
constexpr auto elementwise_fn(T const& lhs, U const& rhs, Fn&& functor = {}) -> concepts::nd_matrix<T::cols, T::rows> auto{
    using ret_value_type = decltype(typename T::value_type{} + typename U::value_type{});
    using ret_type = typename T::template rebind_n<ret_value_type, T::rows, T::cols>;
    concepts::matrix_backend auto ret = ret_type{};

    for (usize row = 0; row < T::rows; row++) {
        for (usize col = 0; col < T::cols; col++) {
            ret[row][col] = std::invoke(std::forward<Fn>(functor), lhs[row][col], rhs[row][col]);
        }
    }

    return ret;
}

}  // namespace detail

template<concepts::matrix T, concepts::matrix U>
    requires(T::rows == U::rows && T::cols == U::cols)
constexpr auto operator+(T const& lhs, U const& rhs) -> concepts::nd_matrix<T::cols, T::rows> auto{
    return elementwise_fn(lhs, rhs, std::plus<>{});
}

template<concepts::matrix T>
constexpr auto operator-(T const& v) -> concepts::nd_matrix_of_t<typename T::value_type, T::rows, T::cols> auto{
    using ret_type = typename T::template rebind<typename T::value_type>;
    concepts::nd_matrix_of_t<typename T::value_type, T::rows, T::cols> auto ret = ret_type{};

    for (usize row = 0; row < T::rows; row++) {
        for (usize col = 0; col < T::cols; col++) {
            ret[row][col] = -v[row][col];
        }
    }

    return ret;
}

template<concepts::matrix T, concepts::matrix U>
    requires(T::rows == U::rows && T::cols == U::cols)
constexpr auto operator-(T const& lhs, U const& rhs) -> concepts::nd_matrix<T::rows, T::cols> auto{
    return elementwise_fn(lhs, rhs, std::minus<>{});
}

template<concepts::matrix T, concepts::matrix U>
    requires(T::cols == U::rows)
constexpr auto operator*(T const& lhs, U const& rhs) -> concepts::nd_matrix<T::rows, U::cols> auto{
    using ret_value_type = decltype(typename T::value_type{} + typename U::value_type{});
    using ret_type = typename T::template rebind_n<ret_value_type, T::rows, U::cols>;

    concepts::matrix_backend auto ret = ret_type{};

    for (usize row = 0; row < T::rows; row++) {
        for (usize col = 0; col < U::cols; col++) {
            auto lhs_row = lhs[row];
            auto rhs_col = rhs.column(col);
            ret[row][col] = dot(lhs_row, rhs_col);
        }
    }

    return ret;
}

template<concepts::matrix T, concepts::vector U>
    requires(T::cols == U::size)
constexpr auto operator*(T const& lhs, U const& rhs) -> concepts::nd_vector<T::rows> auto{
    using ret_value_type = decltype(typename T::value_type{}, typename U::value_type{});
    using ret_type = typename T::template preferred_vector_type<ret_value_type, T::rows>;

    concepts::nd_vector<T::rows> auto ret = ret_type{};
    concepts::nd_matrix<T::rows, 1> auto res = lhs * detail::vector_matrix_adapter<U>{rhs};

    for (usize i = 0; i < T::rows; i++) {
        ret[i] = res[i][0];
    }

    return ret;
}

template<concepts::matrix T, concepts::matrix U>
constexpr auto operator<=>(T const& lhs, U const& rhs) -> std::partial_ordering {
    if constexpr (T::rows != U::rows || T::cols != U::cols) {
        return std::partial_ordering::unordered;
    }

    // rely on vector <=>
    std::partial_ordering ret = lhs[0] <=> rhs[0];

    if (ret == std::partial_ordering::unordered) {
        return ret;
    }

    for (usize i = 1; i < T::rows; i++) {
        std::partial_ordering t = lhs[i] <=> rhs[i];
        if (t != ret) {
            return std::partial_ordering::unordered;
        }
    }

    return ret;
}

}  // namespace stf::blas
