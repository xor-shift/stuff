#pragma once

#include <stuff/sfloat/env.hpp>

namespace stf::sfloat::concepts {

template<typename T>
concept soft_float =  //
  requires(T& l, T const& cl, fenv& env) {
      typename T::repr_type;
      { T::padding_bytes } -> std::convertible_to<size_t>;

      T();
      T(typename T::repr_type{});

      { cl.get_repr() } -> std::convertible_to<typename T::repr_type>;

      { classify(cl) } -> std::convertible_to<classification>;
      { is_finite(cl) } -> std::convertible_to<bool>;
      { is_infinite(cl) } -> std::convertible_to<bool>;
      { is_nan(cl) } -> std::convertible_to<bool>;
      { is_normal(cl) } -> std::convertible_to<bool>;
      { sign_bit(cl) } -> std::convertible_to<bool>;
      { order(cl, cl) } -> std::convertible_to<std::partial_ordering>;
      { cl == cl } -> std::convertible_to<bool>;

      { add(cl, cl) } -> std::convertible_to<T>;
      { add(cl, cl, env) } -> std::convertible_to<T>;
      { sub(cl, cl) } -> std::convertible_to<T>;
      { sub(cl, cl, env) } -> std::convertible_to<T>;
      { mul(cl, cl) } -> std::convertible_to<T>;
      { mul(cl, cl, env) } -> std::convertible_to<T>;
      { div(cl, cl) } -> std::convertible_to<T>;
      { div(cl, cl, env) } -> std::convertible_to<T>;

      { abs(cl) } -> std::convertible_to<T>;
      { abs(cl, env) } -> std::convertible_to<T>;

      { ceil(cl) } -> std::convertible_to<T>;
      { ceil(cl, env) } -> std::convertible_to<T>;
      { floor(cl) } -> std::convertible_to<T>;
      { floor(cl, env) } -> std::convertible_to<T>;
      { round(cl) } -> std::convertible_to<T>;
      { round(cl, env) } -> std::convertible_to<T>;
      { trunc(cl) } -> std::convertible_to<T>;
      { trunc(cl, env) } -> std::convertible_to<T>;
      { rint(cl) } -> std::convertible_to<T>;
      { rint(cl, env) } -> std::convertible_to<T>;
      { lrint<long>(cl) } -> std::convertible_to<long>;
      { lrint<long>(cl, env) } -> std::convertible_to<long>;
  };

}
