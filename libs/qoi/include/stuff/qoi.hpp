#pragma once

#include <stuff/core.hpp>
#include <stuff/expected.hpp>

#include <stuff/qoi/color.hpp>
#include <stuff/qoi/decoder.hpp>
#include <stuff/qoi/encoder.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <span>

namespace stf::qoi {

namespace detail {

struct raw_header {
    char magic[4];
    u32 width;
    u32 height;
    u8 channels;
    u8 colorspace;
};

}  // namespace detail

enum class channels : u8 {
    rgb = 3,
    rgba = 4,
};

enum class color_space : u8 {
    srgb_linear_alpha = 0,
    all_linear = 1,
};

struct header {
    u32 width;
    u32 height;
    channels channels;
    color_space color_space;
};

template<typename Allocator = std::allocator<color>>
struct image {
    constexpr image(Allocator const& allocator = {}) noexcept(noexcept(Allocator(allocator)))
        : m_allocator(allocator) {}

    constexpr image(
      u32 width,
      u32 height,
      color_space color_space = color_space::srgb_linear_alpha,
      Allocator const& allocator = {}
    ) noexcept(noexcept(m_allocator(allocator)) && noexcept(create()))
        : m_allocator(allocator) {
        create(width, height, color_space);
    }

    constexpr image(image const& other)
        : m_allocator(other.m_allocator)
        , m_data(m_allocator.allocate(other.m_width * other.m_height))
        , m_width(other.m_width)
        , m_height(other.m_height)
        , m_color_space(other.m_color_space) {
        std::copy_n(other.m_data, size(), m_data);
    }

    constexpr image(image&& other)
        : m_allocator(other.m_allocator)
        , m_data(other.m_data)
        , m_width(other.m_width)
        , m_height(other.m_height)
        , m_color_space(other.m_color_space) {
        other.m_data = nullptr;
    }

    constexpr ~image() { destroy(); }

    constexpr bool empty() const { return m_data == nullptr; }

    constexpr void destroy() noexcept {
        if (!empty()) {
            m_allocator.deallocate(m_data, m_width * m_height);
            m_data = nullptr;
            m_width = 0;
            m_height = 0;
        }
    }

    constexpr void create(u32 width, u32 height, color_space color_space = color_space::srgb_linear_alpha)  //
      noexcept(noexcept(m_allocator.allocate(0))) {
        destroy();

        m_width = width;
        m_height = height;
        m_color_space = color_space;

        if (width == 0 || height == 0) {
            return;
        }

        m_data = m_allocator.allocate(width * height * 4);
    }

    constexpr void fill(color c) { std::fill(m_data, m_data + m_width * m_height, c); }

    constexpr auto width() const -> u32 { return m_width; }
    constexpr auto height() const -> u32 { return m_height; }
    constexpr auto size() const -> u64 { return static_cast<u64>(width()) * static_cast<u64>(height()); }
    constexpr auto pixels() -> std::span<color> { return {m_data, m_width * m_height}; }
    constexpr auto pixels() const -> std::span<const color> { return {m_data, m_width * m_height}; }

    constexpr auto at(u32 x, u32 y) -> color& { return m_data[y * m_width + x]; }
    constexpr auto at(u32 x, u32 y) const -> color { return m_data[y * m_width + x]; }

    constexpr auto from_memory(std::span<const u8> data) -> expected::expected<void, std::string_view>;

    void from_file(std::string_view filename) { return from_file({std::filesystem::path{filename}}); }

    void from_file(std::ifstream& ifs);

    template<typename It>
    constexpr auto to_memory(It out, double loss_tolerance = 0) const -> expected::expected<It, std::string_view>;

    void to_file(std::string_view filename) const {
        std::ofstream ofs(std::filesystem::path{filename});
        return to_file(ofs);
    }

    void to_file(std::ofstream& ofs) const { to_memory(std::ostreambuf_iterator<char>(ofs)); }

private:
    Allocator m_allocator;

    color* m_data = nullptr;
    u32 m_width = 0;
    u32 m_height = 0;
    color_space m_color_space = color_space::srgb_linear_alpha;
};

}  // namespace stf::qoi

#include <stuff/qoi/detail/qoi.ipp>
