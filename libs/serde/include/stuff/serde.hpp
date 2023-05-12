#pragma once

#include <stuff/serde/concepts.hpp>

#include <stuff/serde/serializables/basic.hpp>
#include <stuff/serde/serializables/span.hpp>
#include <stuff/serde/serializables/string_view.hpp>

#include <stuff/serde/serializers/json.hpp>

#include <stuff/intro.hpp>

namespace stf::serde {

template<typename Serializer, typename T>
constexpr auto serialize(Serializer&& serializer, T&& v)
  -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type>;

namespace detail {

template<typename T>
struct stop_implicit_conversions;

template<typename T>
    requires std::is_array_v<T>
struct stop_implicit_conversions<T> {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-inline"

    constexpr operator T const&() const& noexcept;
    constexpr operator T&&() && noexcept;

#pragma GCC diagnostic pop
};

template<typename T>
    requires(!std::is_array_v<T>)
struct stop_implicit_conversions<T> {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-inline"

    constexpr operator T() const noexcept;

#pragma GCC diagnostic pop
};

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
    requires(!intro::concepts::span_introspector<Intro> && !intro::concepts::named_tuple_introspector<Intro> && !intro::concepts::variant_introspector<Intro>)
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

template<typename Serializer, typename T, intro::concepts::map_introspector Intro>
struct intro_serializer<Serializer, T, Intro> {
    using return_type = stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type>;

    constexpr auto operator()(Serializer&& serializer, T&& v) -> return_type {
        using key_type = typename Intro::key_type;
        using mapped_type = typename Intro::mapped_type;
        stf::serde::concepts::map_serializer auto map_ser = TRYX(serializer.template serialize_map<key_type, mapped_type>(Intro::size(v)));

        auto beg = Intro::begin(v);
        auto end = Intro::end(v);
        for (auto it = beg; it != end; it++) {
            TRYX(map_ser.serialize_entry(it->first, it->second));
        }

        return map_ser.end();
    }
};

template<typename Serializer, typename T, intro::concepts::named_tuple_introspector Intro>
struct intro_serializer<Serializer, T, Intro> {
    using return_type = stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type>;

    constexpr auto operator()(Serializer&& serializer, T&& v) -> return_type {
        stf::serde::concepts::map_serializer auto map_ser = TRYX(serializer.template serialize_struct<std::string_view>(Intro::size()));

        TRYX(impl(map_ser, std::forward<T>(v)));

        return map_ser.end();
    }

private:
    template<typename MapSerializer, usize I = 0>
    constexpr auto impl(MapSerializer&& serializer, T&& v) -> return_type {
        std::string_view key = Intro::template nth_name<I>.c_str();
        TRYX(serializer.serialize_entry(key, Intro::template get<I>(std::forward<T>(v))));

        if constexpr (I + 1 >= Intro::size()) {
            return {};
        } else {
            return impl<MapSerializer, I + 1>(std::forward<MapSerializer>(serializer), std::forward<T>(v));
        }
    }
};

template<typename Serializer, typename T, intro::concepts::variant_introspector Intro>
struct intro_serializer<Serializer, T, Intro> {
    using return_type = stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type>;

    constexpr auto operator()(Serializer&& serializer, T&& v) -> return_type {
        return Intro::visit(
          [&serializer]<typename U>(U&& v) {  //
              return serialize(std::forward<Serializer>(serializer), std::forward<U>(v));
          },
          std::forward<T>(v)
        );
    }
};

template<typename Serializer, typename T>
struct serializer_fn;

template<typename Serializer, typename T>
    requires std::is_arithmetic_v<std::remove_cvref_t<T>>
struct serializer_fn<Serializer, T> {
    /*
     * NOTE TO MY FUTURE SELF:
     * having these in the global scope with _stf_adl_serialize causes implicit
     * conversions to the fundamental types' introspectors in the global scope
     * over the true ADL-serializer in namespaces.
     */

    using U = std::remove_cvref_t<T>;
    inline static constexpr bool is_char = std::is_same_v<U, char> ||      //
                                           std::is_same_v<U, wchar_t> ||   //
                                           std::is_same_v<U, char8_t> ||   //
                                           std::is_same_v<U, char16_t> ||  //
                                           std::is_same_v<U, char32_t>;

    constexpr auto operator()(Serializer&& serializer, U v)  //
      -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type>
        requires(is_char && std::is_integral_v<U> && !std::is_same_v<U, bool>)
    {
        return serializer.serialize_char(v);
    }

    constexpr auto operator()(Serializer&& serializer, U v)  //
      -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type>
        requires(!is_char && std::is_integral_v<U> && !std::is_same_v<U, bool>)
    {
        return serializer.serialize_integral(v);
    }

    constexpr auto operator()(Serializer&& serializer, bool v)  //
      -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
        return serializer.serialize_bool(v);
    }

    constexpr auto operator()(Serializer&& serializer, U v)
      -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type>
        requires std::is_floating_point_v<U>
    {
        return serializer.serialize_float(v);
    }
};

template<typename Serializer, typename T>
    requires (intro::has_introspector<std::remove_cvref_t<T>> && !adl_serializable<T, Serializer>)
struct serializer_fn<Serializer, T> {
    constexpr auto operator()(Serializer&& serializer, T&& v)
      -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
        using intro_type = intro::introspector_t<std::remove_cvref_t<T>>;
        return intro_serializer<Serializer, T, intro_type>{}(std::forward<Serializer>(serializer), std::forward<T>(v));
    }
};

template<typename Serializer, typename T>
    requires (std::is_pointer_v<std::remove_cvref_t<T>> && !adl_serializable<T, Serializer>)
struct serializer_fn<Serializer, T> {
    using U = std::remove_pointer_t<std::remove_reference_t<T>>;

    constexpr auto operator()(Serializer&& serializer, T&& v)
      -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
        if (v != nullptr) {
            return serializer.serialize_some(*v);
        } else {
            return serializer.template serialize_none<U>();
        }
    }
};

template<typename Serializer, adl_serializable<Serializer> T>
struct serializer_fn<Serializer, T> {
    constexpr auto operator()(Serializer&& serializer, T&& v)
      -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
        return _stf_adl_serialize(std::forward<Serializer>(serializer), std::forward<T>(v));
    }
};

}  // namespace detail

template<typename Serializer, typename T>
constexpr auto serialize(Serializer&& serializer, T&& v)
  -> stf::expected<typename std::remove_cvref_t<Serializer>::value_type, typename std::remove_cvref_t<Serializer>::error_type> {
    return detail::serializer_fn<Serializer, T>{}(std::forward<Serializer>(serializer), std::forward<T>(v));
}

}  // namespace stf::serde
