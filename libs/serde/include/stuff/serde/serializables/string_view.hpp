#pragma once

#include <stuff/serde/concepts.hpp>

namespace std {

template<typename Serializer, typename Char, typename Traits = std::char_traits<Char>>
constexpr auto _stf_adl_serialize(Serializer&& serializer, basic_string_view<Char, Traits> v)  //
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    return serializer.serialize_str(v);
}

}  // namespace std

template<typename Serializer>
constexpr auto _stf_adl_serialize(Serializer&& serializer, const char* v)  //
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    return _stf_adl_serialize(std::forward<Serializer>(serializer), std::string_view{v});
}
