#pragma once

#include <expected>

/*
template<typename Serializer, std::integral T>
    requires(std::is_same_v<T, char> ||      //
             std::is_same_v<T, wchar_t> ||   //
             std::is_same_v<T, char8_t> ||   //
             std::is_same_v<T, char16_t> ||  //
             std::is_same_v<T, char32_t>     //
    )
constexpr auto _stf_adl_serialize(Serializer&& serializer, T v)  //
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    return serializer.serialize_char(v);
}

template<typename Serializer, std::integral T>
    requires(!std::is_same_v<T, char> &&      //
             !std::is_same_v<T, wchar_t> &&   //
             !std::is_same_v<T, char8_t> &&   //
             !std::is_same_v<T, char16_t> &&  //
             !std::is_same_v<T, char32_t>     //
    )
constexpr auto _stf_adl_serialize(Serializer&& serializer, T v)  //
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    return serializer.serialize_integral(v);
}

template<typename Serializer, std::floating_point T>
constexpr auto _stf_adl_serialize(Serializer&& serializer, T v)  //
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    return serializer.serialize_float(v);
}

template<typename Serializer>
constexpr auto _stf_adl_serialize(Serializer&& serializer, bool v)  //
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    return serializer.serialize_bool(v);
}
*/