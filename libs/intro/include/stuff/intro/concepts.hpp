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

template<typename Intro, typename Member>
using expected_member_type_c = std::conditional_t<
  std::is_const_v<std::remove_reference_t<Intro>> || std::is_const_v<std::remove_reference_t<Member>>,
  const std::remove_cvref_t<Member>,
  std::remove_cvref_t<Member>>;

template<typename Intro, typename Member>
using expected_member_type = std::conditional_t<  //
  std::is_reference_v<Member>,                    //
  std::conditional_t<                             //
    std::is_lvalue_reference_v<Member>,           //
    expected_member_type_c<Intro, Member>&,       //
    expected_member_type_c<Intro, Member>&&>,     //
  expected_member_type_c<Intro, Member>&>;        //

template<typename Intro, usize I>
    requires requires(                            //
      typename Intro::intro_type& lvalue,         //
      typename Intro::intro_type const& clvalue,  //
      typename Intro::intro_type&& rvalue,        //
      typename Intro::intro_type const&& crvalue  //
    )                                             //
{
    typename Intro::template nth_type<I>;
    { Intro::template get<I>(lvalue) } -> std::convertible_to<expected_member_type<Intro&, typename Intro::template nth_type<I>>>;
    { Intro::template get<I>(rvalue) } -> std::convertible_to<expected_member_type<Intro, typename Intro::template nth_type<I>>>;
    { Intro::template get<I>(clvalue) } -> std::convertible_to<expected_member_type<Intro const&, typename Intro::template nth_type<I>>>;
    { Intro::template get<I>(crvalue) } -> std::convertible_to<expected_member_type<const Intro, typename Intro::template nth_type<I>>>;
}
struct get_helper_single {
    using type = void;
};

template<typename Intro, usize I>
struct get_helper;

template<typename Intro, usize I>
    requires requires {
                 typename get_helper_single<Intro, I>::type;
                 typename get_helper<Intro, I - 1>::type;
             }
struct get_helper<Intro, I> {
    using type = void;
};

template<typename Intro>
    requires requires { typename get_helper_single<Intro, 0>::type; }
struct get_helper<Intro, 0> {
    using type = void;
};

template<typename Intro, usize I>
    requires requires { Intro::template nth_name<I>; } &&  //
             (Intro::template named_field<Intro::template nth_name<I>> == I)
struct name_helper_single {
    using type = void;
};

template<typename Intro, usize I>
struct name_helper;

template<typename Intro, usize I>
    requires requires {
                 typename name_helper_single<Intro, I>::type;
                 typename name_helper<Intro, I - 1>::type;
             }
struct name_helper<Intro, I> {
    using type = void;
};

template<typename Intro>
    requires requires { typename name_helper_single<Intro, 0>::type; }
struct name_helper<Intro, 0> {
    using type = void;
};

}  // namespace detail

template<typename Intro>
concept basic_introspector =                   //
  requires { typename Intro::intro_type; } &&  //
  requires(                                    //
    typename Intro::intro_type& lval,          //
    typename Intro::intro_type&& rval,         //
    const typename Intro::intro_type& clval,   //
    const typename Intro::intro_type&& crval   //
  ) {
      { Intro::size(clval) } -> std::convertible_to<usize>;
  };

template<typename Intro>
concept static_introspector =   //
  basic_introspector<Intro> &&  //
  requires {
      Intro::size();
      std::integral_constant<usize, Intro::size()>{};  // constexpr-ness check
  };

/**
 * An introspector that has the following defined:
 *
 * <li> Some <code>T::intro_type</code> representing a type introspectable by
 * this introspector. Does not have to be the only type introspectable.
 *
 * <li> <code>Intro::size()</code> - A compile-time callable function returning
 * the size of the container (meaning of "size" is for the reader to decide :>).
 *
 * <li> <code>Intro::size(clvalue)</code> - Same as <code>Intro::size()</code>.
 *
 * <li> <code>Intro::nth_type\<I></code> - A type representing the I-th
 * element's type in the tuple.
 *
 * <li> <code>Intro::get\<I>(xvalue)</code> - A function returning the value
 * contained at the I-th index of the tuple. The returned value will be of a
 * "similar" value category to the argument given. The returned value's type
 * before reference qualification is given by <code>Intro::nth_type\<I></code>
 */
template<typename Intro>
concept tuple_introspector =     //
  static_introspector<Intro> &&  //
  requires { typename detail::get_helper<Intro, Intro::size() - 1>::type; };

template<typename Intro>
concept variant_introspector =     //
  tuple_introspector<Intro> &&  //
  requires(const typename Intro::intro_type& clval) {
      { Intro::template holds_variant<0uz>(clval) } -> std::convertible_to<bool>;
      { Intro::held_variant(clval) } -> std::convertible_to<usize>;
      Intro::visit(clval, []<typename T>(T&& v) -> void {});
  };

/**
 * An introspector that has the following defined on top of
 * <code>tuple_inspector</code>'s definitions:
 *
 * <li> <code>Intro::named_field\<str></code> - A usize representing the index
 * of the tuple element with the name <code>str</code> (where str is a
 * <code>stf::string_literal</code>).
 *
 * <li> <code>Intro::nth_name\<I></code> - A
 * <code>stf::string_literal</code> representing the "name" of the I-th element
 * in a tuple. (the meaning of a "name" is up to the introspector)
 *
 * \n\n
 * Additional constraints:
 * \n\n
 *
 * <li> <code>Intro::named_field\<Intro::nth_name\<I>></code> *must* equal I for
 * all <code>usize</code> I lesser than <code>Intro::size()</code>
 *
 * <li> <code>Intro::nth_name\<I></code> for <code>usize</code> I greater than
 * or equal to <code>Intro::size()</code> *should not* participate in overload
 * resolution
 *
 * <li> <code>Intro::named_field\<str></code> *should* return
 * <code>Intro::size()</code> if no <code>Intro::nth_name\<I></code> is
 * <code>str</code> for all I less than <code>Intro::size()</code>
 *
 * \n\n
 * (In general, just don't index OOB, ok?)
 */
template<typename Intro>
concept named_tuple_introspector =  //
  tuple_introspector<Intro> &&      //
  requires { typename detail::name_helper<Intro, Intro::size() - 1>::type; };

/**
 * An introspector that has the following defined:
 *
 * <li> Some <code>T::intro_type</code> representing a type introspectable by
 * this introspector. Does not have to be the only type introspectable.
 *
 * <li> Some <code>T::value_type</code> representing the type of the elements
 * in the sequence.
 *
 * <li> <code>Intro::size(clvalue)</code> - A function returning the number of
 * elements in the sequence.
 *
 * <li> <code>Intro::index(xvalue, i)</code> - A function returning the value
 * contained at the i-th index of the sequence. The returned value will be of a
 * "similar" value category to the argument given. The returned value's type
 * before reference qualification is given by <code>T::value_type</code>.
 *
 * <li> <code>Intro::begin(xvalue)</code> - CBA to explain, returns an iterator.
 * <li> <code>Intro::end(xvalue)</code> - CBA to explain, returns an iterator.
 */
template<typename Intro>
concept span_introspector =                    //
  basic_introspector<Intro> &&                 //
  requires(                                    //
    typename Intro::intro_type& lval,          //
    typename Intro::intro_type&& rval,         //
    const typename Intro::intro_type& clval,   //
    const typename Intro::intro_type&& crval,  //
    usize i
  ) {
      typename Intro::value_type;

      Intro::begin(lval);
      Intro::begin(clval);
      Intro::begin(rval);
      Intro::begin(crval);

      Intro::end(lval);
      Intro::end(clval);
      Intro::end(rval);
      Intro::end(crval);

      // clang-format off
      { Intro::index(std::declval<typename Intro::intro_type>(), i) } -> std::same_as<typename Intro::value_type &&>;
      { Intro::index(std::declval<typename Intro::intro_type&>(), i) } -> std::same_as<typename Intro::value_type &>;
      { Intro::index(std::declval<const typename Intro::intro_type>(), i) } -> std::convertible_to<typename Intro::value_type const &&>;
      { Intro::index(std::declval<typename Intro::intro_type const&>(), i) } -> std::convertible_to<typename Intro::value_type const&>;
      // clang-format on
  };

/**
 * An introspector that has the following defined on top of
 * <code>span_introspector</code>'s definitions:
 *
 * <li> <code>Intro::size()</code> - A compile-time callable function returning
 * the number of elements contained within the array/sequence.
 *
 * <li> <code>Intro::get\<I>(xvalue)</code> - Same behaviour as
 * <code>Intro::index(xvalue, I)</code>.
 */
template<typename Intro>
concept array_introspector =     //
  span_introspector<Intro> &&    //
  static_introspector<Intro> &&  //
  requires() {
      typename detail::get_helper<Intro, Intro::size() - 1>;  // this will also check for `nth_type`
  };

/**
 * An introspector that has the following defined:
 *
 * <li> Some <code>T::intro_type</code> representing a type introspectable by
 * this introspector. Does not have to be the only type introspectable.
 *
 * <li> Some <code>T::key_type</code> representing the type of the elements
 * being mapped from in the map. *Should* be the same as
 * <code>std::string_view</code> (types outside of <code>std::string_view</code>
 * are not required to be supported by serializers in <code>stf::serde</code>.)
 *
 * <li> Some <code>T::mapped_type</code> representing the type of the elements
 * being mapped to in the map.
 *
 * <li> <code>Intro::size(clvalue)</code> - A function returning the number of
 * mappings in the given map.
 *
 * <li> <code>Intro::contains(xvalue, key)</code> - A function to check if the
 * given <code>key</code> maps to some <code>T::mapped_type</code> in the given
 * map
 *
 * <li> <code>Intro::index(xvalue, key)</code> - A function mapping the given
 * <code>key</code> into some value of type <code>T::mapped_type</code>
 * according to the given map.
 *
 * <li> <code>Intro::begin(xvalue)</code> - CBA to explain, returns an iterator.
 * <li> <code>Intro::end(xvalue)</code> - CBA to explain, returns an iterator.
 */
template<typename Intro>
concept map_introspector =                    //
  basic_introspector<Intro> &&                //
  requires(                                   //
    typename Intro::intro_type& lval,         //
    const typename Intro::intro_type& clval,  //
    typename Intro::key_type const& key       //
  ) {
      typename Intro::key_type;
      typename Intro::mapped_type;

      Intro::begin(lval);
      Intro::begin(clval);

      Intro::end(lval);
      Intro::end(clval);

      { Intro::contains(clval, key) } -> std::convertible_to<bool>;

      { Intro::index(lval, key) } -> std::same_as<typename Intro::mapped_type&>;
      { Intro::index(clval, key) } -> std::convertible_to<typename Intro::mapped_type const&>;
  };

}  // namespace concepts

}  // namespace stf::intro
