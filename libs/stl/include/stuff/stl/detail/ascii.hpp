#pragma once

#include <stuff/stl/detail/header.hpp>
#include <stuff/stl/detail/triangle.hpp>

#include <stuff/expected.hpp>

#include <charconv>

namespace stf::stl::detail {

template<typename IIt>
constexpr auto read_header_ascii(IIt it, std::sentinel_for<IIt> auto end) -> stf::expected<std::pair<header, IIt>, std::string_view> {
    auto solid_name = std::string{};

    solid_name.push_back(*it++);
    if (solid_name.back() == ' ') {
        solid_name.pop_back();
    }

    while (it != end) {
        solid_name.push_back(*it++);

        if (solid_name.back() == '\n') {
            break;
        }
    }

    if (solid_name.empty() || (it == end && solid_name.back() != '\n')) {
        return stf::unexpected{"EOF while reading solid name"};
    }

    solid_name.pop_back();
    return std::pair{header_ascii{std::move(solid_name)}, it};
}

template<typename IIt, std::invocable<triangle<float>&&> Fn>
static auto read_triangles_ascii(header_ascii const& header, IIt it, std::sentinel_for<IIt> auto end, Fn&& callback) -> stf::expected<IIt, std::string_view> {
    auto is_ws = [](char c) -> bool { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; };

    auto is_oob = [](char c) -> bool { return c >= '\x7F' || c <= ' '; };

    auto read_word = [&is_ws, &is_oob](IIt& it, std::sentinel_for<IIt> auto end) -> stf::expected<std::string, std::string_view> {
        auto ret = std::string{};

        while (it != end) {
            const auto c = static_cast<char>(*it++);

            if (is_ws(c)) {
                if (ret.empty()) {
                    continue;
                }
                return ret;
            }

            if (is_oob(c)) {
                return stf::unexpected{"bad character in an ASCII STL file"};
            }

            ret.push_back(c);
        }

        return ret;
    };

    auto read_float = [&read_word](IIt& it, std::sentinel_for<IIt> auto end) -> stf::expected<float, std::string_view> {
        const auto word = TRYX(read_word(it, end));

        auto ret = 0.f;
        const auto res = std::from_chars(word.data(), word.data() + word.size(), ret);
        if (res.ec != std::errc{}) {
            return stf::unexpected{"bad ASCII STL file (bad float)"};
        }
        return ret;
    };

    auto read_vector = [&read_float](IIt& it, std::sentinel_for<IIt> auto end) -> stf::expected<std::array<float, 3>, std::string_view> {
        return std::array<float, 3>{
          TRYX(read_float(it, end)),  //
          TRYX(read_float(it, end)),  //
          TRYX(read_float(it, end)),  //
        };
    };

    auto expect_word = [&read_word](IIt& it, std::sentinel_for<IIt> auto end, std::string_view str) -> stf::expected<void, std::string_view> {
        if (TRYX(read_word(it, end)) != str) {
            return stf::unexpected{"bad ASCII STL file (unexpected keyword)"};
        }

        return {};
    };

    while (it != end) {
        auto ret = triangle<float>{};

        const auto first_word = TRYX(read_word(it, end));

        if (first_word.empty()) {
            break;
        }

        if (first_word == "endsolid") {
            const auto end_solid_name = TRYX(read_word(it, end));

            if (end_solid_name != header.m_solid_name) {
                return stf::unexpected{"bad ASCII STL file (bad endsolid name)"};
            }

            return {};
        }

        if (first_word != "facet") {
            return stf::unexpected{"bad ASCII STL file (unexpected keyword)"};
        }

        TRYX(expect_word(it, end, "normal"));
        ret.m_normal = TRYX(read_vector(it, end));

        TRYX(expect_word(it, end, "outer"));
        TRYX(expect_word(it, end, "loop"));

        for (auto i = 0uz; i < 3uz; i++) {
            TRYX(expect_word(it, end, "vertex"));
            ret.m_vertices[i] = TRYX(read_vector(it, end));
        }

        TRYX(expect_word(it, end, "endloop"));
        TRYX(expect_word(it, end, "endfacet"));

        std::invoke(std::forward<Fn>(callback), std::move(ret));
    }

    return it;
}

}
