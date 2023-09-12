#pragma once

#include <stuff/ply/detail/element.hpp>
#include <stuff/ply/detail/file_format.hpp>

namespace stf::ply {

struct file_header {
    file_format m_format;

    std::vector<std::string> m_comments;
    std::vector<element> m_elements;
};

}  // namespace stf::ply
