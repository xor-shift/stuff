## Progress
The order of functions is taken form [cppreference](https://en.cppreference.com/w/cpp/header/cmath)

80-bit `long double` won't be supported, fuck intel

stubbed implementations won't update the environment argument upon errors

Arithmetic: (1/5)
- [ ] add (stubbed implementation exists)
- [ ] sub (stubbed implementation exists)
- [ ] mul (stubbed implementation exists)
- [ ] div (stubbed implementation exists)
- [x] unary `operator-`

Basic operations: (1/9)
- [x] `abs`
- [ ] `fmod`
- [ ] `remainder`
- [ ] `remquo`
- [ ] `fma`
- [ ] `fmax`
- [ ] `fmin`
- [ ] `fdim`
- [ ] `nan`

Nearest integer floating-point operations: (5/5)
- [x] `ceil`
- [x] `floor`
- [x] `trunc`
  - [x] `inverse_trunc` (nonstandard, rounds away from zero)
- [x] `round`
  - nearbyint and rint will not be implemented

Floating-point manipulation functions: (0/7)
- [ ] `frexp`
- [ ] `modf`
- [ ] `scalbn`
  - `ldexp` won't be implemented as radix is assumed to be 2
- [ ] `ilogb`
- [ ] `logb`
- [ ] `nextafter`
- [ ] `copysign`

Classification and comparison: (7/7)
- [x] `fpclassify` (nonstandard return type of `stf::sfloat::classification`)
- [x] `isfinite`
- [x] `isinf`
- [x] `isnan`
- [x] `isnormal`
- [x] `signbit`
- [x] `order` (nonstandard return type of `std::partial_ordering`)
  - `isgreater`, `isgreaterequal`, etc. will not be implemented in favour of `order`

Linear interpolation: (0/2)
- [ ] `lerp`
- [ ] `inverse_lerp` (nonstandard)

Exponential functions: (0/7)
- [ ] `exp`
- [ ] `exp2`
- [ ] `expm1`
- [ ] `log`
- [ ] `log10`
- [ ] `log2`
- [ ] `log1p`

Power functions: (0/4)
- [ ] `pow`
- [ ] `sqrt`
- [ ] `cbrt`
- [ ] `hypot`

Trigonometric functions: (0/7)
- [ ] `sin`
- [ ] `cos`
- [ ] `tan`
- [ ] `asin`
- [ ] `acos`
- [ ] `atan`
- [ ] `atan2`

Hyperbolic functions: (0/6)
- [ ] `sinh`
- [ ] `cosh`
- [ ] `tanh`
- [ ] `asinh`
- [ ] `acosh`
- [ ] `atanh`

Error and gamma functions: (0/4)
- [ ] `erf`
- [ ] `erfc`
- [ ] `tgamma`
- [ ] `lgamma`