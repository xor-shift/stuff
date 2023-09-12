#include <stuff/stl.hpp>

#include <gtest/gtest.h>

#include <span>

TEST(stl, header_ascii) {
    auto test_header = [](std::string_view header_text, std::string_view expected_name) {
        const auto res = stf::stl::read_header(header_text.begin(), header_text.end());
        ASSERT_TRUE(res);
        ASSERT_EQ(res->second, header_text.end());
        ASSERT_TRUE(std::holds_alternative<stf::stl::header_ascii>(res->first));
        ASSERT_EQ(std::get<stf::stl::header_ascii>(res->first).m_solid_name, expected_name);
    };

    test_header("solid a\n", "a");
    test_header("solid  \n", " ");
    test_header("solid \n", "");
    test_header("solid\n", "");

    test_header("solid\n", "");
}

TEST(stl, header_binary) {
#ifdef __GNUC__
#    pragma push_macro("SEVENTY_SIX_FUCKING_ZEROES")
#    undef SEVENTY_SIX_FUCKING_ZEROES
#    define SEVENTY_SIX_FUCKING_ZEROES \
        0, 0, 0, 0,   /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0, /**/             \
          0, 0, 0, 0

    const u8 header_0[84] = {0, 0, 0, 0, SEVENTY_SIX_FUCKING_ZEROES, 0xEF, 0xBE, 0xAD, 0xDE};
    const u8 header_1[84] = {1, 2, 3, 4, SEVENTY_SIX_FUCKING_ZEROES, 0xBE, 0xBA, 0xFE, 0xCA};
#    undef SEVENTY_SIX_FUCKING_ZEROES
#    pragma pop_macro("SEVENTY_SIX_FUCKING_ZEROES")
#else
    const u8 header_0[84] = {[80] = 0xDE, 0xAD, 0xBE, 0xEF};
    const u8 header_1[84] = {1, 2, 3, 4, [80] = 0xCA, 0xFE, 0xBA, 0xBE};
#endif

    auto test_header = [](std::span<const u8, 84> header_data, std::span<const u8, 80> expected_header, u32 expected_triangles) {
        const auto res = stf::stl::read_header(header_data.begin(), header_data.end());
        ASSERT_TRUE(res);
        ASSERT_EQ(res->second, header_data.end());
        ASSERT_TRUE(std::holds_alternative<stf::stl::header_binary>(res->first));
        ASSERT_EQ(std::get<stf::stl::header_binary>(res->first).m_num_triangles, expected_triangles);
        ASSERT_TRUE(std::ranges::equal(std::get<stf::stl::header_binary>(res->first).m_header, expected_header));
    };

    test_header(header_0, std::array<u8, 80>{}, 0xDEADBEEFu);
    test_header(header_1, std::array<u8, 80>{1, 2, 3, 4}, 0xCAFEBABEu);
}

TEST(stl, binary) {
    const u8 triangles_raw[] = {
      0x00, 0x00, 0x00, 0x00,  // normal[0]
      0x00, 0x00, 0x00, 0x00,  // normal[1]
      0x00, 0x00, 0x00, 0x00,  // normal[2]
      0x00, 0x00, 0x00, 0x00,  // vert[0][0]
      0x00, 0x00, 0x00, 0x00,  // vert[0][1]
      0x00, 0x00, 0x00, 0x00,  // vert[0][2]
      0x00, 0x00, 0x00, 0x00,  // vert[1][0]
      0x00, 0x00, 0x00, 0x00,  // vert[1][1]
      0x00, 0x00, 0x00, 0x00,  // vert[1][2]
      0x00, 0x00, 0x00, 0x00,  // vert[2][0]
      0x00, 0x00, 0x00, 0x00,  // vert[2][1]
      0x00, 0x00, 0x00, 0x00,  // vert[2][2]
      0x00, 0x00,              // attrib

      0x00, 0x00, 0x00, 0x80,  // normal[0]  (-0.0f)
      0x00, 0x00, 0x80, 0xff,  // normal[1]  (-inf)
      0x00, 0x00, 0x80, 0x7f,  // normal[2]  (inf)
      0x00, 0x00, 0x80, 0x3f,  // vert[0][0] (1.f)
      0x00, 0x00, 0x00, 0x40,  // vert[0][1] (2.f)
      0x00, 0x00, 0x40, 0x40,  // vert[0][2] (3.f)
      0xff, 0xff, 0x7f, 0x3f,  // vert[1][0] (0.999999940395355224609375f)
      0xff, 0xff, 0xff, 0x3f,  // vert[1][1] (1.99999988079071044921875f)
      0xff, 0xff, 0x3f, 0x40,  // vert[1][2] (2.9999997615814208984375f)
      0x00, 0x00, 0x00, 0x00,  // vert[2][0] (0.f)
      0x00, 0x00, 0x00, 0x00,  // vert[2][1] (0.f)
      0x00, 0x00, 0x00, 0x00,  // vert[2][2] (0.f)
      0x34, 0x12,              // attrib     (0x1234)
    };

    auto triangles = std::vector<stf::stl::triangle<float>>{};
    triangles.reserve(2uz);
    auto res = stf::stl::read_triangles(
      stf::stl::header_binary{.m_num_triangles = 2}, std::begin(triangles_raw), std::end(triangles_raw),
      [&triangles](stf::stl::triangle<float>&& triangle) { triangles.emplace_back(std::move(triangle)); }
    );

    ASSERT_TRUE(res);
    ASSERT_EQ(*res, std::end(triangles_raw));
    ASSERT_EQ(triangles.size(), 2uz);

    ASSERT_EQ(triangles[0], stf::stl::triangle<float>{});
    ASSERT_EQ(
      triangles[1],  //
      (stf::stl::triangle<float>{
        .m_normal = {-0.f, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()},
        .m_vertices = {{{1.f, 2.f, 3.f}, {0.999999940395355224609375f, 1.99999988079071044921875f, 2.9999997615814208984375f}, {0, 0, 0}}},
        .m_attributes = 0x1234,
      })
    );
}

template<typename T>
void test_file(std::span<const u8> file_contents, T const& expected_header) {
    const auto res = stf::stl::read_header(file_contents.begin(), file_contents.end());
    ASSERT_TRUE(res);

    auto&& [header, it] = *res;
    ASSERT_TRUE(std::holds_alternative<T>(header));

    if constexpr (std::is_same_v<T, stf::stl::header_binary>) {
        ASSERT_EQ(std::get<T>(header).m_header, expected_header.m_header);
        ASSERT_EQ(std::get<T>(header).m_num_triangles, expected_header.m_num_triangles);
    } else if constexpr (std::is_same_v<T, stf::stl::header_ascii>) {
        ASSERT_EQ(std::get<T>(header).m_solid_name, expected_header.m_solid_name);
    } else {
        std::unreachable();
    }

    auto vec = std::vector<stf::stl::triangle<float>>{};
    if (std::holds_alternative<stf::stl::header_binary>(header)) {
        vec.reserve(std::get<stf::stl::header_binary>(header).m_num_triangles);
    }

    const auto res_tri = stf::stl::read_triangles(header, it, file_contents.end(), [&vec]<typename Tri>(Tri&& tri) { vec.emplace_back(std::forward<Tri>(tri)); });
    ASSERT_TRUE(res_tri);
}

#include "./sphericon.stl.h"
#include "./utah_teapot.stl.h"

TEST(stl, real_files) {
    test_file(
      model_binary_utah_teapot,
      stf::stl::header_binary{
        .m_header =
          {
            'E', 'x', 'p', 'o', 'r', 't', 'e', 'd', ' ', 'f', 'r', 'o', 'm', ' ', 'B', 'l', 'e',
            'n', 'd', 'e', 'r', '-', '2', '.', '7', '4', ' ', '(', 's', 'u', 'b', ' ', '5', ')',
          },
        .m_num_triangles = 0x000024DEu,
      }
    );

    test_file(
      model_ascii_sphericon,
      stf::stl::header_ascii{
        .m_solid_name = "ASCII_STL_of_a_sphericon_by_CMG_Lee",
      }
    );
}
