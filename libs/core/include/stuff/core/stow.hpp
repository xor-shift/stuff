#pragma once

#include <stuff/core/string_literal.hpp>

namespace stf::detail {

template<typename Struct, typename MemberType, string_literal Identifier = "">
struct get_stown_member_ptr {
    inline static MemberType m_ptr = nullptr;
};

template<typename Struct, auto Member, string_literal Identifier = "">
struct stow_private {
    inline static const auto m_ptr                                         //
      = get_stown_member_ptr<Struct, decltype(Member), Identifier>::m_ptr  //
      = Member;
};

}  // namespace stf::detail
