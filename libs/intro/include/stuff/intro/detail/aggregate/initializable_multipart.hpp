#pragma once

#include <utility>
#include <tuple>

namespace stf::intro::detail::agg {

template<typename T, typename Types0, typename Types1, typename Types2>
struct initializable_multipart;

/// Checks if <code>T</code> is initializable with values of <code>{T0s..., {T1s...}, T2s...}</code>
/// (<code>T(0|1|2)s...</code> are forwarded perfectly)
template<typename T, typename... T0s, typename T1, typename... T1s, typename... T2s>
struct initializable_multipart<T, std::tuple<T0s...>, std::tuple<T1, T1s...>, std::tuple<T2s...>> {
    /*using prev_type = std::tuple<T0s...>;
    using cur_type = std::tuple<T1, T1s...>;
    using slack_type = std::tuple<T2s...>;*/

    inline static constexpr bool value =  //
      requires(T0s && ... t0s, T1&& t1, T1s&&... t1s, T2s&&... t2s) {
          T{std::forward<T0s>(t0s)..., {std::forward<T1>(t1), std::forward<T1s>(t1s)...}, std::forward<T2s>(t2s)...};
      };
};

/// Checks if <code>T</code> is initializable with values of <code>{T0s..., T1, T2s...}</code> (<code>T(0|2)s...</code>
/// and <code>T1</code> are forwarded perfectly)
template<typename T, typename... T0s, typename T1, typename... T2s>
struct initializable_multipart<T, std::tuple<T0s...>, std::tuple<T1>, std::tuple<T2s...>> {
    inline static constexpr bool value =  //
      requires(T0s && ... t0s, T1&& t1, T2s&&... t2s) {
          T{std::forward<T0s>(t0s)..., std::forward<T1>(t1), std::forward<T2s>(t2s)...};
      };
};

/// Checks if <code>T</code> is initializable with values of <code>{T0s..., {}, T2s...}</code> (<code>T(0|2)s...</code>
/// are forwarded perfectly)
template<typename T, typename... T0s, typename... T2s>
struct initializable_multipart<T, std::tuple<T0s...>, std::tuple<>, std::tuple<T2s...>> {
    inline static constexpr bool value =  //
      requires(T0s && ... t0s, T2s&&... t2s) {
          T{std::forward<T0s>(t0s)..., {}, std::forward<T2s>(t2s)...};
      };
};

/// Checks if <code>T</code> is initializable with values of <code>{T0s..., {T1s...}}</code> (<code>T(0|1)s...</code>
/// are forwarded perfectly)
/// \tparam Types0 <code>std::tuple\<T0s...></code>
/// \tparam Types1 <code>std::tuple\<T1s...></code>
template<typename T, typename Types0, typename Types1, typename Types2>
inline constexpr bool initializable_multipart_v = initializable_multipart<T, Types0, Types1, Types2>::value;


}
