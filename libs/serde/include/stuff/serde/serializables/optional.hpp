#pragma once

#include <stuff/serde/concepts.hpp>

#include <optional>

namespace std {

template<typename Serializer, typename T>
constexpr auto _stf_adl_serialize(Serializer&& serializer, optional<T> const& v)  //
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    if ((bool)v) {
        return serializer.serialize_some(*v);
    } else {
        return serializer.template serialize_none<T>();
    }
}

template<typename Serializer, typename T>
constexpr auto _stf_adl_serialize(Serializer&& serializer, optional<T>&& v)  //
  -> std::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    if ((bool)v) {
        return serializer.serialize_some(std::move(*std::move(v)));
    } else {
        return serializer.template serialize_none<T>();
    }
}

}  // namespace std