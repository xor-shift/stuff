#pragma once

#include <stuff/core.hpp>

namespace stf::stl {

template<std::floating_point Repr>
struct triangle {
    using float_type = Repr;
    using vector_type = std::array<float_type, 3>;

    vector_type m_normal;
    std::array<vector_type, 3> m_vertices;
    u16 m_attributes;

    friend constexpr auto operator==(triangle const& lhs, triangle const& rhs) -> bool {
        return lhs.m_normal == rhs.m_normal &&      //
               lhs.m_vertices == rhs.m_vertices &&  //
               lhs.m_attributes == rhs.m_attributes;
    }
};

}
