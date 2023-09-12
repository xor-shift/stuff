#include <stuff/ply.hpp>

#include <gtest/gtest.h>

#define ASSERT_NO_ERROR(_x) ASSERT_TRUE((_x)) << ((bool)(_x) ? "no error" : (_x).error().str())

TEST(ply, header) {
    const auto header_0 = R"(ply
format ascii   1.0
comment a
element vertex 2903
comment b
property float32 x
comment c
property float32 y
comment d
property float32 z
comment e
element face 5804
comment f
property list uint8 int32 vertex_indices
comment g
end_header
comment h (won't be parsed)
)"sv;

    auto context = stf::ply::context(header_0.begin(), header_0.end());
    auto res = context.read_header();
    ASSERT_NO_ERROR(res);

    auto header = *res;
    // ASSERT_EQ(it, std::begin(header_0) + 238);

    ASSERT_EQ(header.m_elements.size(), 2);

    ASSERT_EQ(header.m_elements[0].m_name, "vertex");
    ASSERT_EQ(header.m_elements[0].m_size, 2903);
    ASSERT_EQ(header.m_elements[0].m_properties.size(), 3);

    ASSERT_EQ(header.m_elements[0].m_properties[0].m_name, "x");
    ASSERT_TRUE(std::holds_alternative<stf::ply::basic_field_type>(header.m_elements[0].m_properties[0].m_field));
    ASSERT_EQ(std::get<stf::ply::basic_field_type>(header.m_elements[0].m_properties[0].m_field), stf::ply::basic_field_type::f32);

    ASSERT_EQ(header.m_elements[0].m_properties[1].m_name, "y");
    ASSERT_TRUE(std::holds_alternative<stf::ply::basic_field_type>(header.m_elements[0].m_properties[1].m_field));
    ASSERT_EQ(std::get<stf::ply::basic_field_type>(header.m_elements[0].m_properties[1].m_field), stf::ply::basic_field_type::f32);

    ASSERT_EQ(header.m_elements[0].m_properties[2].m_name, "z");
    ASSERT_TRUE(std::holds_alternative<stf::ply::basic_field_type>(header.m_elements[0].m_properties[2].m_field));
    ASSERT_EQ(std::get<stf::ply::basic_field_type>(header.m_elements[0].m_properties[2].m_field), stf::ply::basic_field_type::f32);

    ASSERT_EQ(header.m_elements[1].m_name, "face");
    ASSERT_EQ(header.m_elements[1].m_size, 5804);
    ASSERT_EQ(header.m_elements[1].m_properties.size(), 1);

    ASSERT_EQ(header.m_elements[1].m_properties[0].m_name, "vertex_indices");
    ASSERT_TRUE(std::holds_alternative<stf::ply::list_field_type>(header.m_elements[1].m_properties[0].m_field));
    ASSERT_EQ(std::get<stf::ply::list_field_type>(header.m_elements[1].m_properties[0].m_field).first, stf::ply::basic_field_type::u8);
    ASSERT_EQ(std::get<stf::ply::list_field_type>(header.m_elements[1].m_properties[0].m_field).second, stf::ply::basic_field_type::i32);

    ASSERT_EQ(header.m_comments.size(), 7);
}

static const auto mock_header_base = stf::ply::file_header{
  .m_format{
    .m_data_format = stf::ply::data_format::ascii,
    .m_version_major = 1,
    .m_version_minor = 0,
  },
  .m_comments{},
  .m_elements{
    {
      .m_name = "vertices",
      .m_properties{
        {
          .m_field = stf::ply::basic_field_type::f32,
          .m_name = "x",
        },
        {
          .m_field = stf::ply::basic_field_type::f32,
          .m_name = "y",
        },
        {
          .m_field = stf::ply::basic_field_type::f32,
          .m_name = "z",
        }},
      .m_size = 3,
    },
    {
      .m_name = "faces",
      .m_properties{{
        .m_field{
          stf::ply::list_field_type{stf::ply::basic_field_type::u8, stf::ply::basic_field_type::u32},
        },
        .m_name = "indices",
      }},
      .m_size = 4,
    },
  },
};

template<typename T, usize N>
static void test_data(std::span<T, N> data, stf::ply::file_header header) {
    auto context = stf::ply::context(data);
    context.header() = header;

    auto res_0 = context.template read_element<std::tuple<float, float, double>>(0);
    ASSERT_NO_ERROR(res_0);
    ASSERT_FLOAT_EQ(std::get<0>(*res_0), 1);
    ASSERT_FLOAT_EQ(std::get<1>(*res_0), 2);
    ASSERT_FLOAT_EQ(std::get<2>(*res_0), 3);

    auto res_1 = context.template read_element<std::tuple<float, double, float>>(0);
    ASSERT_NO_ERROR(res_1);
    ASSERT_FLOAT_EQ(std::get<0>(*res_1), 4.5f);
    ASSERT_FLOAT_EQ(std::get<1>(*res_1), 6.7f);
    ASSERT_FLOAT_EQ(std::get<2>(*res_1), 8.9f);

    auto res_2 = context.template read_element<std::tuple<double, float, float>>(0);
    ASSERT_NO_ERROR(res_2);
    ASSERT_FLOAT_EQ(std::get<0>(*res_2), -0.f);
    ASSERT_FLOAT_EQ(std::get<1>(*res_2), std::numeric_limits<float>::infinity());
    ASSERT_FLOAT_EQ(std::get<2>(*res_2), -std::numeric_limits<float>::infinity());

    auto res_3 = context.template read_element<std::array<u32, 0>>(1);
    ASSERT_NO_ERROR(res_3);

    auto res_4 = context.template read_element<std::vector<u32>>(1);
    ASSERT_NO_ERROR(res_4);
    ASSERT_EQ(res_4->size(), 1uz);
    ASSERT_EQ((*res_4)[0], 1);

    auto res_5 = context.template read_element<std::vector<u32>>(1);
    ASSERT_NO_ERROR(res_5);
    ASSERT_EQ(res_5->size(), 2uz);
    ASSERT_EQ((*res_5)[0], 1);
    ASSERT_EQ((*res_5)[1], 2);

    auto res_6 = context.template read_element<std::array<u32, 3>>(1);
    ASSERT_NO_ERROR(res_6);
    ASSERT_EQ(*res_6, (std::array<u32, 3>{1, 2, 3}));
}

TEST(ply, data_ascii) {
    auto mock_header = mock_header_base;
    mock_header.m_format.m_data_format = stf::ply::data_format::ascii;

    const auto data = R"(1 2 3
4.5 6.7 8.9
-0 inf -inf
0
1 1
2 1 2
3 1 2 3
2 1 2 3
2 1
excess data :^)
some more excess data ig
idk

)"sv;

    test_data(std::span{data.begin(), data.end()}, mock_header);
}

TEST(ply, data_binary_le) {
    auto mock_header = mock_header_base;
    mock_header.m_format.m_data_format = stf::ply::data_format::binary_le;

    const u8 data[]{
      0x00, 0x00, 0x80, 0x3f,  // 1
      0x00, 0x00, 0x00, 0x40,  // 2
      0x00, 0x00, 0x40, 0x40,  // 3

      0x00, 0x00, 0x90, 0x40,  // 4.5
      0x66, 0x66, 0xd6, 0x40,  // 6.7
      0x66, 0x66, 0x0e, 0x41,  // 8.9

      0x00, 0x00, 0x00, 0x80,  // -0.0
      0x00, 0x00, 0x80, 0x7f,  // inf
      0x00, 0x00, 0x80, 0xff,  // -inf

      0x00,  // []

      0x01,                    // [1]
      0x01, 0x00, 0x00, 0x00,  //

      0x02,                    // [1, 2]
      0x01, 0x00, 0x00, 0x00,  //
      0x02, 0x00, 0x00, 0x00,  //

      0x03,                    // [1, 2, 3]
      0x01, 0x00, 0x00, 0x00,  //
      0x02, 0x00, 0x00, 0x00,  //
      0x03, 0x00, 0x00, 0x00,  //
    };

    test_data(std::span(data), mock_header);
}

TEST(ply, data_binary_be) {
    auto mock_header = mock_header_base;
    mock_header.m_format.m_data_format = stf::ply::data_format::binary_be;

    const u8 data[]{
      0x3f, 0x80, 0x00, 0x00,  // 1
      0x40, 0x00, 0x00, 0x00,  // 2
      0x40, 0x40, 0x00, 0x00,  // 3

      0x40, 0x90, 0x00, 0x00,  // 4.5
      0x40, 0xd6, 0x66, 0x66,  // 6.7
      0x41, 0x0e, 0x66, 0x66,  // 8.9

      0x80, 0x00, 0x00, 0x00,  // -0.0
      0x7f, 0x80, 0x00, 0x00,  // inf
      0xff, 0x80, 0x00, 0x00,  // -inf

      0x00,  // []

      0x01,                    // [1]
      0x00, 0x00, 0x00, 0x01,  //

      0x02,                    // [1, 2]
      0x00, 0x00, 0x00, 0x01,  //
      0x00, 0x00, 0x00, 0x02,  //

      0x03,                    // [1, 2, 3]
      0x00, 0x00, 0x00, 0x01,  //
      0x00, 0x00, 0x00, 0x02,  //
      0x00, 0x00, 0x00, 0x03,  //
    };

    test_data(std::span(data), mock_header);
}

#include "./cow_ascii.hpp"

using namespace stf::ply::detail::test_models;

template<usize I, typename It, typename Sentinel, typename T, usize N, typename... Ts, usize... Ns>
    requires((N % 2) == 0)
constexpr void test_elements(stf::ply::context<It, Sentinel>& context, std::span<T, N> test_values, std::span<Ts, Ns>... rest) {
    auto const& header = context.header();
    const auto element = header.m_elements[I];

    const auto start_span = test_values.template subspan<0, N / 2>();
    const auto end_span = test_values.template subspan<N / 2>();

    using element_type = std::remove_cvref_t<T>;

    auto start_array = std::array<element_type, N / 2>{};
    auto end_array = std::array<element_type, N / 2>{};

    for (usize i = 0; i < element.m_size; i++) {
        const auto res = context.template read_element<element_type>(I);
        ASSERT_NO_ERROR(res);
        const auto elem = *res;

        if (i < N / 2) {
            start_array[i] = elem;
        } else if (i >= (element.m_size - N / 2)) {
            end_array[i - (element.m_size - N / 2)] = elem;
        }
    }

    for (usize i = 0; i < N / 2; i++) {
        ASSERT_EQ(start_span[i], start_array[i]) << "element no: " << i;
        ASSERT_EQ(end_span[i], end_array[i]) << "element no: " << i;
    }
}

TEST(ply, real_ascii) {
    auto context = stf::ply::context(cow_ascii_ply.begin(), cow_ascii_ply.end());

    auto header_res = context.read_header();
    ASSERT_NO_ERROR(header_res);

    constexpr std::tuple<double, double, double> vertex_values[]{
      {0.605538, 0.183122, -0.472278},   //
      {0.649223, 0.1297, -0.494875},     //
      {0.601082, 0.105512, -0.533343},   //
      {0.691245, 0.0569483, -0.524762},  //
      {0.652035, 0.0379582, -0.542332},  //
      {-1.51079, 0.678959, 0.252184},    //
      {-1.48045, 0.682309, 0.256751},    //
      {-1.45577, 0.674789, 0.244898},    //
      {-1.24479, 0.648768, 0.200864},    //
      {-1.48926, 0.64369, 0.227226},     //
    };

    constexpr std::array<u16, 3> index_values[]{
      {0, 1, 2},           //
      {1, 3, 4},           //
      {5, 6, 2},           //
      {6, 7, 8},           //
      {7, 9, 10},          //
      {2876, 2875, 2900},  //
      {1458, 2847, 2840},  //
      {2370, 2839, 2837},  //
      {2491, 2013, 2011},  //
      {2498, 2381, 2433},  //
    };

    test_elements<0>(context, std::span(vertex_values), std::span(index_values));
}

#include "./cow_binary.hpp"

TEST(ply, real_binary) {
    auto context = stf::ply::context(cow_binary_ply.begin(), cow_binary_ply.end());

    auto header_res = context.read_header();
    ASSERT_NO_ERROR(header_res);

    constexpr std::tuple<double, double, double> vertex_values[]{
      {0.47227799892425537, -0.605538010597229, 0.18312199413776398},   //
      {0.49487501382827759, -0.64922302961349487, 0.12970000505447388},     //
      {-0.20086400210857391, 1.2447899580001831, 0.64876800775527954},    //
      {-0.22722600400447845, 1.4892599582672119, 0.64368999004364014},    //
    };

    constexpr std::array<u16, 3> index_values[]{
      {0, 1, 2},           //
      {1, 3, 4},           //
      {2876, 2875, 2900},  //
      {1458, 2847, 2840},  //
    };

    test_elements<0>(context, std::span(vertex_values), std::span(index_values));
}
