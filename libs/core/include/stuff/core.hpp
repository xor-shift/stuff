#pragma once

#include <stuff/core/bunch_of_types.hpp>
#include <stuff/core/integers.hpp>
#include <stuff/core/string_literal.hpp>
#include <stuff/core/tuple.hpp>
#include <stuff/core/type_exists.hpp>
#include <stuff/core/visitor.hpp>

#include <utility>

namespace stf {

// this place is currently a dumping ground for uncategorized functions and types

/// An analogue of Go's <code>struct {}</code> type\n
/// Intended to be used with stf::channel. (a channel of T=void returns <code>empty</code> values (this is akin to
/// <code>chan struct{}</code> from Go))
struct empty {};

constexpr void assume(bool pred, const char* message = nullptr) noexcept {
    if (!pred) {
#ifdef NDEBUG
        std::unreachable();
#else
#    pragma GCC diagnstic push
#    pragma GCC diagnstic ignored "-Wterminate" // that's the goal
        using namespace std::string_literals;

        auto&& real_message = message != nullptr ? "assumption failed: "s + message : "assumption failed"s;
        throw std::runtime_error(real_message);
#    pragma GCC diagnstic pop
#endif
    }
}

template<string_literal Lit>
constexpr void unreachable_with_message() {
    std::unreachable();
}

};  // namespace stf

#include <string_view>

using namespace std::literals;
using namespace std::string_view_literals;
