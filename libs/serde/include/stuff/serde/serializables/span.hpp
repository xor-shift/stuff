#pragma once

#include <stuff/serde/concepts.hpp>

#include <span>

namespace std {

template<typename Serializer, typename T, usize Extent = std::dynamic_extent>
constexpr auto _stf_adl_serialize(Serializer&& serializer, span<T, Extent> v)  //
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    return serializer.serialize_bytes(static_cast<span<const std::byte, Extent>>(v));
}

}  // namespace std
