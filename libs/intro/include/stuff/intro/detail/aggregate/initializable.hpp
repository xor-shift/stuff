#pragma once

#include <utility>

namespace stf::intro::detail::agg {

/// Determines whether an aggregate can be aggregate-initialized with <code>args...</code>.\n
/// Keep brace elision in mind, <code>struct { int a[2]; }</code> can be initialized with two arguments (not one
/// initializer list argument or whatever)
template<typename T, typename... Args>
concept initializable = requires(Args&&... args) { T{std::forward<Args>(args)...}; };

}
