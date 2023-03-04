#include <stuff/qoi.hpp>

#include <gtest/gtest.h>

#include <fstream>

static uint8_t simple_image[]{
  'q',  'o',  'i',  'f',         // magic
  0x00, 0x00, 0x00, 0x04,        // width = 4
  0x00, 0x00, 0x00, 0x03,        // height = 3
  0x04,                          // channels = RGBA
  0x01,                          // colorspace = linear
  0xFE, 0xFF, 0xFF, 0xFF,        // QOI_OP_RGB(255, 255, 255) (idx: 38)
  0xC2,                          // QOI_OP_RUN(3)
  0xFF, 0x12, 0x34, 0x56, 0x78,  // QOI_OP_RGBA(18, 52, 86, 120) (idx: 60)
  0x26,                          // QOI_OP_INDEX(38) (255, 255, 255, 255)
  0x3C,                          // QOI_OP_INDEX(60) (18, 52, 86, 120)
  0xC1,                          // QOI_OP_RUN(2)
  0x00,                          // QOI_OP_INDEX(0) (0, 0, 0, 0)
  0x7F,                          // QOI_OP_DIFF(1, 1, 1) (1, 1, 1, 0)
  0xBF, 0xFF,                    // QOI_OP_LUMA(31, 7, 7) (?, 32, ?, 0)
  0x00, 0x00, 0x00, 0x00,        // suffix
  0x00, 0x00, 0x00, 0x00,
};

TEST(qoi, from_memory) {
    stf::qoi::image image{};
    auto res = image.from_memory(simple_image);
    ASSERT_TRUE(res);

    auto got_pixels = static_cast<std::span<const stf::qoi::color, 12>>(image.pixels());

    static constexpr std::array<stf::qoi::color, 12> expected_pixels{{
      {255, 255, 255, 255},
      {255, 255, 255, 255},
      {255, 255, 255, 255},
      {255, 255, 255, 255},

      {18, 52, 86, 120},
      {255, 255, 255, 255},
      {18, 52, 86, 120},
      {18, 52, 86, 120},

      {18, 52, 86, 120},
      {0, 0, 0, 0},
      {1, 1, 1, 0},
      {39, 32, 39, 0},
    }};

    ASSERT_TRUE(std::equal(expected_pixels.begin(), expected_pixels.end(), got_pixels.begin()));
}

#include "./test_images.hpp"

static void output_image(stf::qoi::image<> const& image, std::string_view name) {
    auto pixels = image.pixels();
    std::vector<uint8_t> image_data{};
    image_data.reserve(pixels.size() * 4);
    for (auto px : pixels) {
        image_data.push_back(px.r);
        image_data.push_back(px.g);
        image_data.push_back(px.b);
        image_data.push_back(px.a);
    }

    std::ofstream ofs(std::filesystem::path{name}, std::ios::binary | std::ios::out);
    std::copy(image_data.begin(), image_data.end(), std::ostream_iterator<uint8_t>(ofs));
}

static void file_test(
  std::span<const uint8_t> qoi_data,
  std::span<const uint8_t> expected_data,
  std::pair<uint32_t, uint32_t> expected_dims,
  std::string_view out_name = ""
) {
    stf::qoi::image image{};
    auto res = image.from_memory(qoi_data);

    auto pixels = image.pixels();
    std::vector<uint8_t> image_data{};
    image_data.reserve(pixels.size() * 4);
    for (auto px : pixels) {
        image_data.push_back(px.r);
        image_data.push_back(px.g);
        image_data.push_back(px.b);
        image_data.push_back(px.a);
    }

    if (!out_name.empty()) {
        std::ofstream ofs(std::filesystem::path{out_name}, std::ios::binary | std::ios::out);
        std::copy(image_data.begin(), image_data.end(), std::ostream_iterator<uint8_t>(ofs));
    }

    if (!res) {
        ASSERT_TRUE(res) << std::string(res.error());
    }

    ASSERT_EQ(image.width(), expected_dims.first);
    ASSERT_EQ(image.height(), expected_dims.second);
    ASSERT_EQ(image.size(), expected_data.size() / 4);
    ASSERT_TRUE(std::equal(image_data.begin(), image_data.end(), expected_data.begin()));
}

TEST(qoi, file_test) {
    using namespace stf::qoi::test_images;
    file_test(dice_qoi, dice_data, {800, 600}, "dice_out.data");
    file_test(edgecase_qoi, edgecase_data, {256, 64}, "edgecase_out.data");
    file_test(kodim10_qoi, kodim10_data, {512, 768}, "kodim10_out.data");
    file_test(kodim23_qoi, kodim23_data, {768, 512}, "kodim23_out.data");
    file_test(qoi_logo_qoi, qoi_logo_data, {448, 220}, "qoi_logo_out.data");
    file_test(testcard_qoi, testcard_data, {256, 256}, "testcard_out.data");
    file_test(testcard_rgba_qoi, testcard_rgba_data, {256, 256}, "testcard_rgba_out.data");
    file_test(wikipedia_008_qoi, wikipedia_008_data, {1152, 858}, "wikipedia_008_out.data");
}

static void round_trip_test(std::span<const uint8_t> qoi_data) {
    stf::qoi::image image_decode_0{};
    auto res_decode_0 = image_decode_0.from_memory(qoi_data);
    ASSERT_TRUE(res_decode_0);

    std::vector<uint8_t> data_encode_0{};
    image_decode_0.to_memory(back_inserter(data_encode_0));

    stf::qoi::image image_decode_1{};
    auto res_decode_1 = image_decode_1.from_memory(data_encode_0);
    ASSERT_TRUE(res_decode_1);

    ASSERT_EQ(image_decode_0.width(), image_decode_1.width());
    ASSERT_EQ(image_decode_0.height(), image_decode_1.height());

    auto px_0 = image_decode_0.pixels();
    auto px_1 = image_decode_1.pixels();

    ASSERT_TRUE(std::equal(px_0.begin(), px_0.end(), px_1.begin()));
}

TEST(qoi, round_trip) {
    using namespace stf::qoi::test_images;

    round_trip_test(simple_image);
    round_trip_test(dice_qoi);
    round_trip_test(edgecase_qoi);
    round_trip_test(kodim10_qoi);
    round_trip_test(kodim23_qoi);
    round_trip_test(qoi_logo_qoi);
    round_trip_test(testcard_qoi);
    round_trip_test(testcard_rgba_qoi);
    round_trip_test(wikipedia_008_qoi);
}

static void loss_test(std::string_view name, std::span<const uint8_t> qoi_data, double tolerance) {
    stf::qoi::image image{};
    auto res = image.from_memory(qoi_data);
    ASSERT_TRUE(res);

    std::vector<uint8_t> re_encode{};
    ASSERT_TRUE(image.to_memory(back_inserter(re_encode), tolerance));

    double ratio = static_cast<double>(re_encode.size()) / static_cast<double>(qoi_data.size());

    std::cout << name << ": "
              << "orig=" << qoi_data.size() << ", lossy=" << re_encode.size() << ", ratio=" << ratio << std::endl;

    stf::qoi::image decode{};
    res = decode.from_memory(re_encode);
    ASSERT_TRUE(res);

    output_image(decode, std::string(name) + "_lossy.data");
}

TEST(qoi, reencode_lossy) {
    using namespace stf::qoi::test_images;

    double tolerance = 3;

    loss_test("simple_image", simple_image, tolerance);
    loss_test("dice_qoi", dice_qoi, tolerance);
    loss_test("edgecase_qoi", edgecase_qoi, tolerance);
    loss_test("kodim10_qoi", kodim10_qoi, tolerance);
    loss_test("kodim23_qoi", kodim23_qoi, tolerance);
    loss_test("qoi_logo_qoi", qoi_logo_qoi, tolerance);
    loss_test("testcard_qoi", testcard_qoi, tolerance);
    loss_test("testcard_rgba_qoi", testcard_rgba_qoi, tolerance);
    loss_test("wikipedia_008_qoi", wikipedia_008_qoi, tolerance);
}
