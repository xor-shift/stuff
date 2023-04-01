#pragma once

#include <concepts>

#include <stuff/core/integers.hpp>
#include <stuff/core/type_traits.hpp>

namespace stf::blas::concepts {

namespace detail {

template<typename T, usize I>
concept vec_adl_gettable_at =  //
  requires(T const& v) {
      typename T::value_type;
      { get<I>(v) } -> std::convertible_to<typename T::value_type>;
  };

template<typename T, usize I = 0>
constexpr auto vec_gettable_helper() -> bool {
    if constexpr (I >= T::size) {
        return true;
    } else if constexpr (!vec_adl_gettable_at<T, I>) {
        return false;
    } else {
        return vec_gettable_helper<T, I + 1>();
    }
}

}  // namespace detail

/// A vector or a vector-expression.
template<typename T>
concept vector =  //
  requires(T const& v, usize idx) {
      typename T::value_type;

      // FIXME: find out a way to implement this constraint
      // use <code>typename T::template rebind\<U></code> in the meanwhile
      //
      // actually, maybe don't implement this?
      // this behaviour being unconstrained is useful for matrix_row
      //
      // template<typename U = typename T::value_type, size_t N = T::size> class T::rebind;

      { T::size } -> std::convertible_to<usize>;
      { std::integral_constant<usize, T::size>::value } -> std::convertible_to<usize>;

      { std::declval<decltype(v[idx])>() } -> std::convertible_to<typename T::value_type const&>;
  } && (T::size > 0) && detail::vec_gettable_helper<T>();

/// Vector of <code>Dims</code> dimensions.
template<typename T, usize Dims>
concept nd_vector = vector<T> && (T::size == Dims);

template<typename T, typename U>
concept vector_of_t = vector<T> && std::is_same_v<typename T::value_type, U>;

template<typename T, typename U, usize Dims>
concept nd_vector_of_t = nd_vector<T, Dims> && vector_of_t<T, U>;

template<typename T, typename V>
concept vector_like = vector<V> && nd_vector_of_t<T, typename V::value_type, V::size>;

/// A mutable vector.
template<typename T>
concept vector_backend =  //
  vector<T> &&            //
  requires(T& v, usize idx) {
      { std::declval<decltype(v[idx])>() } -> std::convertible_to<typename T::value_type&>;
      true;
  };

/// A matrix or a matrix-expression.
template<typename T>
concept matrix =  //
  requires(T const& v, usize idx) {
      typename T::value_type;

      { T::rows } -> std::convertible_to<usize>;
      { std::integral_constant<usize, T::rows>::value } -> std::convertible_to<usize>;

      { T::cols } -> std::convertible_to<usize>;
      { std::integral_constant<usize, T::cols>::value } -> std::convertible_to<usize>;

      { v[idx] } -> nd_vector_of_t<typename T::value_type, T::cols>;
      { v.column(idx) } -> nd_vector_of_t<typename T::value_type, T::rows>;

      typename T::template preferred_vector_type<typename T::value_type, T::cols>;

      { std::declval<decltype(v[idx][idx])>() } -> std::convertible_to<typename T::value_type const&>;
  } && (T::rows > 0) && (T::cols > 0);

/// Matrix of size <code>Rows</code> × <code>Cols</code>.
template<typename T, usize Rows, usize Cols>
concept nd_matrix = matrix<T> && (T::rows == Rows) && (T::cols == Cols);

template<typename T, typename U>
concept matrix_of_t = matrix<T> && std::is_same_v<typename T::value_type, U>;

template<typename T, typename U, usize Rows, usize Cols>
concept nd_matrix_of_t = nd_matrix<T, Rows, Cols> && matrix_of_t<T, U>;

/// A mutable vector.
template<typename T>
concept matrix_backend =  //
  matrix<T> &&            //
  requires(T& v, usize idx) {
      { std::declval<decltype(v[idx][idx])>() } -> std::convertible_to<typename T::value_type&>;
      true;
  };

}  // namespace stf::blas::concepts
