#pragma once

#include <stuff/core.hpp>

#include <concepts>
#include <initializer_list>
#include <limits>

namespace stf::random::concepts {

template<typename G>
concept uniform_random_bit_generator =  //
  requires(G g) {
      typename G::result_type;
      { G::min() } -> std::convertible_to<typename G::result_type>;
      { G::max() } -> std::convertible_to<typename G::result_type>;
      { g() } -> std::convertible_to<typename G::result_type>;
  } &&                                                //
  std::unsigned_integral<typename G::result_type> &&  //
  (G::max() > G::min());                              // we're also verifying that min & max are constexpr

template<typename S>
concept seed_sequence =  //
  requires(
    S q,
    S const& r,
    typename S::result_type* ob,
    const typename S::result_type* ib,
    std::initializer_list<typename S::result_type> il
  ) {
      typename S::result_type;

      S();
      S(ib, ib);
      S(il);

      q.generate(ob, ob);

      { r.size() } -> std::convertible_to<usize>;
      r.param(ob);
  } &&                                                //
  std::unsigned_integral<typename S::result_type> &&  //
  std::numeric_limits<typename S::result_type>::digits >= 32;

namespace detail {

template<std::unsigned_integral T>
struct stub_seed_seq {
    using result_type = T;

    constexpr stub_seed_seq() = default;

    template<typename IIt>
    constexpr stub_seed_seq(IIt beg, IIt end) {}

    constexpr stub_seed_seq(std::initializer_list<T> il)
        : stub_seed_seq(il.begin(), il.end()) {}

    template<typename OIt>
    constexpr void generate(OIt rb, OIt re) {
        std::fill(rb, re, u32(0));
    }

    constexpr auto size() const -> usize { return 0; }

    template<typename OIt>
    constexpr void param(OIt) const {}
};

static_assert(seed_sequence<stub_seed_seq<u32>>);
static_assert(seed_sequence<stub_seed_seq<u64>>);

}  // namespace detail

template<typename E>
concept random_number_engine =        //
  uniform_random_bit_generator<E> &&  //
  requires(E e, unsigned long long z, detail::stub_seed_seq<u32>& seq32, detail::stub_seed_seq<u32>& seq64) {
      E(seq32);
      E(seq64);

      e.discard(z);
      e.seed(seq32);
      e.seed(seq64);
  };

}  // namespace stf::random::concepts
