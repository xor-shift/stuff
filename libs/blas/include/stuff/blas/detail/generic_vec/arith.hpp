#pragma once

namespace stf::blas {

#pragma push_macro("BINARY_ARITH_FACTORY_VV")
#pragma push_macro("BINARY_ARITH_FACTORY_VS_SV")

// clang-format off
#define BINARY_ARITH_FACTORY_VV(_name) \
template<concepts::vector X, concepts::vector Y, typename T = typename X::value_type, typename U = typename Y::value_type, usize N = X::size> \
constexpr auto _name(X const& x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto

#define BINARY_ARITH_FACTORY_VS_SV(_name, _sym) \
template<concepts::vector X, typename U, typename T = typename X::value_type, usize N = X::size>                                        \
    requires(!concepts::vector<U>)                                                                                                      \
constexpr auto _name(X const& x, U y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto requires requires { x[0] _sym y; }; \
                                                                                                                                        \
template<typename T, concepts::vector Y, typename U = typename Y::value_type, usize N = Y::size>                                        \
    requires(!concepts::vector<T>)                                                                                                      \
constexpr auto _name(T x, Y const& y) -> concepts::generic_vector<std::common_type_t<T, U>, N> auto requires requires { x _sym y[0]; }

// clang-format on

BINARY_ARITH_FACTORY_VV(operator+);
BINARY_ARITH_FACTORY_VV(operator-);
BINARY_ARITH_FACTORY_VV(operator*);
BINARY_ARITH_FACTORY_VV(operator/);

BINARY_ARITH_FACTORY_VS_SV(operator*, *);
BINARY_ARITH_FACTORY_VS_SV(operator/, /);

#undef BINARY_ARITH_FACTORY_VV
#pragma pop_macro("BINARY_ARITH_FACTORY_VV")
#undef BINARY_ARITH_FACTORY_VS
#pragma pop_macro("BINARY_ARITH_FACTORY_VS_SV")

template<concepts::vector X, typename T = typename X::value_type, usize N = X::size>
constexpr auto operator-(X const& x) -> concepts::generic_vector<T, N> auto;

}  // namespace stf::blas

#include <stuff/blas/detail/generic_vec/arith.ipp>
