#pragma once

namespace stf::sfloat::concepts {

template<typename T>
concept soft_float =  //
  requires(T& l, T const& cl) {
      typename T::repr_type;
      { T::padding_bytes } -> std::convertible_to<size_t>;

      T();
      T(typename T::repr_type {});

      { cl.get_repr() } -> std::convertible_to<typename T::repr_type>;

      { abs(cl) } -> std::convertible_to<T>;
      { ceil(cl) } -> std::convertible_to<T>;
      { floor(cl) } -> std::convertible_to<T>;
      { round(cl) } -> std::convertible_to<T>;
      { trunc(cl) } -> std::convertible_to<T>;
  };

}
