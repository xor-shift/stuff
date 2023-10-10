#pragma once

#include <stuff/core.hpp>

#include <expected>
#include <optional>
#include <string_view>

namespace stf::serde::concepts {

template<typename T>
concept serializer =  //
  requires(T& ser) {
      typename T::value_type;
      typename T::error_type;

      typename T::serialize_seq_type;
      typename T::serialize_tuple_type;
      typename T::serialize_map_type;
      typename T::serialize_struct_type;

      ser.serialize_bool(bool());

      ser.serialize_integral(i8());
      ser.serialize_integral(u8());
      ser.serialize_integral(i16());
      ser.serialize_integral(u16());
      ser.serialize_integral(i32());
      ser.serialize_integral(u32());
      ser.serialize_integral(i64());
      ser.serialize_integral(u64());

      ser.serialize_float(float());
      ser.serialize_float(double());

      ser.serialize_char(char());
      ser.serialize_char(wchar_t());
      ser.serialize_char(char8_t());
      ser.serialize_char(char16_t());
      ser.serialize_char(char32_t());

      ser.serialize_str(std::basic_string_view<char>{});
      ser.serialize_str(std::basic_string_view<wchar_t>{});
      ser.serialize_str(std::basic_string_view<char8_t>{});
      ser.serialize_str(std::basic_string_view<char16_t>{});
      ser.serialize_str(std::basic_string_view<char32_t>{});

      // ser.serialize_bytes(std::span<const std::byte>{});
      // ser.serialize_bytes(std::span<const uint8_t>{});

      // should support all types supported by `serialize_*`
      // should ideally call ADL-serialize at some point
      ser.serialize_optional(std::optional<int>{});

      { ser.serialize_seq(std::optional<usize>{}) } -> std::convertible_to<std::expected<typename T::serialize_seq_type, typename T::error_type>>;
      { ser.template serialize_tuple<0uz>() } -> std::convertible_to<std::expected<typename T::serialize_tuple_type, typename T::error_type>>;
      { ser.template serialize_map<std::string_view, int>(usize{}) } -> std::convertible_to<std::expected<typename T::serialize_map_type, typename T::error_type>>;
      //{ ser.template serialize_struct<..., intro::introspector_t<...>>() } -> std::convertible_to<std::expected<typename T::serialize_struct_type, typename T::error_type>>;
  };

template<typename T>
concept seq_serializer =  //
  requires(T& ser) {
      typename T::value_type;
      typename T::error_type;

      { ser.serialize_element(int()) } -> std::convertible_to<std::expected<typename T::value_type, typename T::error_type>>;
      { ser.end() } -> std::convertible_to<std::expected<typename T::value_type, typename T::error_type>>;
  };

template<typename T>
concept tuple_serializer =  //
  requires(T& ser) {
      typename T::value_type;
      typename T::error_type;

      { ser.serialize_element(int()) } -> std::convertible_to<std::expected<typename T::value_type, typename T::error_type>>;
      { ser.end() } -> std::convertible_to<std::expected<typename T::value_type, typename T::error_type>>;
  };

template<typename T>
concept map_serializer =  //
  requires(T& ser) {
      typename T::value_type;
      typename T::error_type;

      { ser.serialize_entry(std::string_view{}, int()) } -> std::convertible_to<std::expected<typename T::value_type, typename T::error_type>>;
      { ser.end() } -> std::convertible_to<std::expected<typename T::value_type, typename T::error_type>>;
  };

}  // namespace stf::serde::concepts

namespace stf::serde {

template<typename Serializer, typename T>
constexpr auto serialize(Serializer&& serializer, T&& v)
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type>;

}
