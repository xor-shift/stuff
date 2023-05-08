#pragma once

#include <stuff/serde/concepts.hpp>

#include <stuff/serde/serializables/basic.hpp>
#include <stuff/serde/serializables/span.hpp>
#include <stuff/serde/serializables/string_view.hpp>

#include <stuff/serde/serializers/json.hpp>

#include <stuff/intro.hpp>

namespace stf::serde {

namespace detail {

template<typename T, typename Serializer>
concept adl_serializable = requires(Serializer&& ser, T&& v) { _stf_adl_serialize(std::forward<Serializer>(ser), std::forward<T>(v)); };

template<typename Serializer, typename T, typename Intro>
struct intro_serializer;

template<typename Serializer, typename T, intro::concepts::span_introspector Intro>
struct intro_serializer<Serializer, T, Intro> {
    constexpr auto operator()(Serializer&& serializer, T&& v)
      -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
        usize sz = Intro::size(v);

        stf::serde::concepts::seq_serializer auto seq_ser = TRYX(serializer.serialize_seq(sz));

        auto beg = Intro::begin(v);
        auto end = Intro::end(v);
        for (auto it = beg; it != end;) {
            TRYX(seq_ser.serialize_element(*it++));
        }

        return seq_ser.end();
    }
};

template<typename Serializer, typename T, intro::concepts::tuple_introspector Intro>
    requires(!intro::concepts::span_introspector<Intro>)
struct intro_serializer<Serializer, T, Intro> {
    using return_type = stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type>;

    constexpr auto operator()(Serializer&& serializer, T&& v) -> return_type {
        stf::serde::concepts::tuple_serializer auto tup_ser = TRYX(serializer.template serialize_tuple<Intro::size()>());

        TRYX(impl(tup_ser, std::forward<T>(v)));

        return tup_ser.end();
    }

private:
    template<typename TupleSerializer, usize I = 0>
    constexpr auto impl(TupleSerializer&& serializer, T&& v) -> return_type {
        TRYX(serializer.serialize_element(Intro::template get<I>(std::forward<T>(v))));

        if constexpr (I + 1 >= Intro::size()) {
            return {};
        } else {
            return impl<TupleSerializer, I + 1>(std::forward<TupleSerializer>(serializer), std::forward<T>(v));
        }
    }
};

template<typename Serializer, typename T>
struct serializer_fn;

template<typename Serializer, typename T>
    requires intro::has_introspector<std::remove_cvref_t<T>> && (!adl_serializable<T, Serializer>)
struct serializer_fn<Serializer, T> {
    constexpr auto operator()(Serializer&& serializer, T&& v) {
        using intro_type = intro::introspector_t<std::remove_cvref_t<T>>;
        return intro_serializer<Serializer, T, intro_type>{}(std::forward<Serializer>(serializer), std::forward<T>(v));
    }
};

template<typename Serializer, adl_serializable<Serializer> T>
struct serializer_fn<Serializer, T> {
    constexpr auto operator()(Serializer&& serializer, T&& v) { _stf_adl_serialize(std::forward<Serializer>(serializer), std::forward<T>(v)); }
};

}  // namespace detail

template<typename Serializer, typename T>
constexpr auto serialize(Serializer&& serializer, T&& v) {
    detail::serializer_fn<Serializer, T>{}(std::forward<Serializer>(serializer), std::forward<T>(v));
}

}  // namespace stf::serde
