#pragma once

#include <stuff/core.hpp>

namespace stf::intro {

/* Some notes on the properties of containers
 *
 * | container | integral indexable? | string indexable?   | iterable?           | multi-valued? |
 * +-----------+---------------------+---------------------+---------------------+---------------+
 * | c array   | compile-time        | no                  | functor (ct)        | no            |
 * | c struct  | compile-time (hard) | compile-time (hard) | functor (ct) (hard) | yes           |
 * | vector    | runtime             | no                  | runtime             | no            |
 * | umap      | no                  | runtime             | runtime             | no            |
 * | uset      | no                  | no                  | runtime             | no            |
 * | pair      | compile-time (sz=2) | no                  | functor (ct)        | yes           |
 * | tuple     | compile-time        | no                  | functor (ct)        | yes           |
 *               operator[], get      .at(), get             beg/end, functor
 *
 * runtime indexable inspectors must have an `operator[]` defined taking either
 * a `usize` argument and/or a `std::string_view` argument (depending on the
 * string-indexability status)
 *
 * compile-time indexable inspectors must have a `get<>` defined with no
 * arguments and with either a `usize` and/or a `stf::string_literal` as the
 * first template parameter depending on the string-indexability status
 *
 * ---
 *
 * compile-time iterable inspectors can take functors to a function called
 * `iterate` with one of the two signatures:
 *  - `template<typename T> void fn(usize, T&&)`
 *  - `template<usize I, typename T> void fn(std::integral_constant<usize, I>, T&&)`
 *
 * runtime iterable inspectors have the same `iterate` function valid with only
 * the first signature mentioned above. on top of that they support iterating
 * through `begin` and `end`
 *
 * ---
 *
 * an instrospector doesn't necessarily have to inspect the type it is intended
 * to inspect. one such example is the introspector of `std::array<T, N>` and
 * `T[N]`. despite `intro_type` being `std::array<T, N>`, the introspector can
 * inspect T-of-N types (i.e. `T[N]`).
 *
 */

namespace concepts {

namespace detail {

template<typename Intro, usize I>
    requires requires {
                 // clang-format off
                typename Intro::template nth_type<I>;
                { Intro::template get<I>(std::declval<typename Intro::intro_type>()) } -> std::same_as<typename Intro::template nth_type<I> &&>;
                { Intro::template get<I>(std::declval<typename Intro::intro_type&>()) } -> std::same_as<typename Intro::template nth_type<I> &>;
                { Intro::template get<I>(std::declval<const typename Intro::intro_type>()) } -> std::convertible_to<typename Intro::template nth_type<I> const &&>;
                { Intro::template get<I>(std::declval<typename Intro::intro_type const&>()) } -> std::convertible_to<typename Intro::template nth_type<I> const&>;
                 // clang-format on
             }
struct get_helper_single;

template<typename Intro, usize I>
struct get_helper;

template<typename Intro, usize I>
    requires requires {
                 typename get_helper_single<Intro, I>;
                 typename get_helper<Intro, I - 1>;
             }
struct get_helper<Intro, I>;

template<typename Intro>
    requires requires {
                 typename get_helper_single<Intro, 0>;  //
             }
struct get_helper<Intro, 0>;

template<typename Intro, usize I>
    requires requires { Intro::template nth_name<I>; } &&  //
             (Intro::template named_field<Intro::template nth_name<I>> == I)
struct name_helper_single;

template<typename Intro, usize I>
struct name_helper;

template<typename Intro, usize I>
    requires requires {
                 typename name_helper_single<Intro, I>;
                 typename name_helper<Intro, I - 1>;
             }
struct name_helper<Intro, I>;

template<typename Intro>
    requires requires {
                 typename name_helper_single<Intro, 0>;  //
             }
struct name_helper<Intro, 0>;

}  // namespace detail

template<typename Intro>
concept tuple_introspector =  //
  requires(
    typename Intro::intro_type& lval,         //
    typename Intro::intro_type&& rval,        //
    const typename Intro::intro_type& clval,  //
    const typename Intro::intro_type&& crval  //
  ) {
      typename Intro::intro_type;  // not necessarily the only type that can be introspected using `Intro`

      Intro::size();
      Intro::size(lval);
      std::integral_constant<usize, Intro::size()>{};  // constexpr-ness check

      typename detail::get_helper<Intro, Intro::size() - 1>;  // this will also check for `nth_type`
  };

template<typename Intro>
concept named_tuple_introspector =  //
  tuple_introspector<Intro> &&      //
  requires(
    typename Intro::intro_type& lval,         //
    typename Intro::intro_type&& rval,        //
    const typename Intro::intro_type& clval,  //
    const typename Intro::intro_type&& crval  //
  ) {
      typename detail::name_helper<Intro, Intro::size() - 1>;  // this will also check for `nth_type`
  };

// a runtime usize-indexable introspector
template<typename Intro>
concept span_introspector =  //
  requires(
    typename Intro::intro_type& lval,          //
    typename Intro::intro_type&& rval,         //
    const typename Intro::intro_type& clval,   //
    const typename Intro::intro_type&& crval,  //
    usize i
  ) {
      typename Intro::intro_type;  // not necessarily the only type that can be introspected using `Intro`
      typename Intro::value_type;

      Intro::size(lval);

      // clang-format off
      { Intro::index(std::declval<typename Intro::intro_type>(), i) } -> std::same_as<typename Intro::value_type &&>;
      { Intro::index(std::declval<typename Intro::intro_type&>(), i) } -> std::same_as<typename Intro::value_type &>;
      { Intro::index(std::declval<const typename Intro::intro_type>(), i) } -> std::convertible_to<typename Intro::value_type const &&>;
      { Intro::index(std::declval<typename Intro::intro_type const&>(), i) } -> std::convertible_to<typename Intro::value_type const&>;
      // clang-format on
  };

// a compile-time usize-indexable introspector
template<typename Intro>
concept array_introspector =   //
  span_introspector<Intro> &&  //
  requires() {
      Intro::size();
      std::integral_constant<usize, Intro::size()>{};  // constexpr-ness check

      typename detail::get_helper<Intro, Intro::size() - 1>;  // this will also check for `nth_type`
  };

}  // namespace concepts

}  // namespace stf::intro
