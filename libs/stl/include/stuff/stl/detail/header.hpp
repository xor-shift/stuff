#pragma once

#include <stuff/core.hpp>

#include <variant>

namespace stf::stl {

struct header_ascii {
    std::string m_solid_name;
};

struct header_binary {
    std::array<u8, 80> m_header;
    u32 m_num_triangles;
};

using header = std::variant<header_ascii, header_binary>;

}
