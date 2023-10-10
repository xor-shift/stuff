#pragma once

#include <stuff/serde/concepts.hpp>

#include <charconv>

namespace stf::serde {

template<typename OIt, std::sentinel_for<OIt> OItEnd, typename Allocator = std::allocator<char>>
struct json_array_serializer;

template<typename OIt, std::sentinel_for<OIt> OItEnd, typename Allocator = std::allocator<char>>
struct json_object_serializer;

template<typename Extender, typename OIt, std::sentinel_for<OIt> OItEnd, typename Allocator = std::allocator<char>>
struct json_serializer_base {
    friend Extender;

    using value_type = void;
    using error_type = std::string_view;
    using return_type = std::expected<value_type, error_type>;

    using serialize_seq_type = json_array_serializer<OIt, OItEnd, Allocator>;
    using serialize_tuple_type = json_array_serializer<OIt, OItEnd, Allocator>;
    using serialize_map_type = json_object_serializer<OIt, OItEnd, Allocator>;
    using serialize_struct_type = json_object_serializer<OIt, OItEnd, Allocator>;

    constexpr json_serializer_base(OIt& it, OItEnd end, Allocator const& alloc = Allocator{})  //
      noexcept(noexcept(Allocator(alloc)) && noexcept(Allocator()))
        : m_it(it)
        , m_end(end) {}

    constexpr auto serialize_char(char c) -> return_type { return emit_char('\'') && emit_char(c) && emit_char('\'') ? return_type{} : std::unexpected{"buffer overrun"}; }

    template<std::integral T>
    constexpr auto serialize_integral(T v) -> return_type {
        return emit_to_chars(v);
    }

    template<std::floating_point T>
    auto serialize_float(T v) -> return_type {
        return emit_to_chars<T, sizeof(T) * 8>(v);
    }

    constexpr auto serialize_bool(bool v) -> return_type { return emit_str(v ? "true" : "false") ? return_type{} : std::unexpected{"buffer overrun"}; }

    template<typename T>
    constexpr auto serialize_some(T&& v) -> return_type {
        return serialize(*this, std::forward<T>(v));
    }

    template<typename T>
    constexpr auto serialize_none() -> return_type {
        std::string_view to_emit = std::is_arithmetic_v<std::remove_cvref_t<T>> ? "undefined" : "null";

        return emit_str(to_emit) ? return_type{} : std::unexpected{"buffer overrun"};
    }

    template<typename Char, typename Traits = std::char_traits<Char>>
    constexpr auto serialize_str(std::basic_string_view<Char, Traits> v) -> return_type {
        return emit_char('"') && emit_str(v) && emit_char('"') ? return_type{} : std::unexpected{"buffer overrun"};
    }

    template<typename T>
        requires requires(T&& v) {
                     typename T::value_type;
                     (bool)v;
                     { *v } -> std::convertible_to<typename T::value_type const&>;
                 }
    constexpr auto serialize_optional(T&& opt_v) -> return_type {
        if ((bool)opt_v) {
            return serialize(*this, *opt_v);
        } else {
            return emit_str("null") ? return_type{} : std::unexpected{"buffer overrun"};
        }
    }

    constexpr auto serialize_seq(std::optional<usize>) -> std::expected<serialize_seq_type, error_type>;

    template<usize N>
    constexpr auto serialize_tuple() -> std::expected<serialize_seq_type, error_type> {
        return serialize_seq(N);
    }

    template<typename K, typename V>
    constexpr auto serialize_map(std::optional<usize>) -> std::expected<serialize_map_type, error_type>;

    template<typename K>
    constexpr auto serialize_struct(std::optional<usize>) -> std::expected<serialize_struct_type, error_type>;

private:
    Allocator m_allocator;
    OIt& m_it;
    OItEnd m_end;

    constexpr auto emit_char(char c) -> bool {
        if (m_it == m_end) {
            return false;
        }

        *m_it++ = c;

        return true;
    }

    constexpr auto emit_str(std::string_view str) -> bool {
        // TODO: check what kind of iterators we've and use std::copy conditionally

        for (char c : str) {
            if (!emit_char(c)) {
                return false;
            }
        }
        return true;
    }

    template<typename T, usize N = std::numeric_limits<T>::digits10>
    constexpr auto emit_to_chars(T v) -> return_type {
        std::array<char, 1 + N> buffer{};
        std::string_view chars{};

        if (std::to_chars_result res = std::to_chars(buffer.begin(), buffer.end(), v); res.ec != std::errc()) {
            return std::unexpected{"to_chars failed (this should not happen)"};
        } else {
            chars = std::string_view{buffer.begin(), res.ptr};
        }

        return emit_str(chars) ? return_type{} : std::unexpected{"buffer overrun"};
    }
};

template<typename OIt, std::sentinel_for<OIt> OItEnd, typename Allocator = std::allocator<char>>
struct json_serializer : json_serializer_base<json_serializer<OIt, OItEnd, Allocator>, OIt, OItEnd, Allocator> {
    using base_type = json_serializer_base<json_serializer<OIt, OItEnd, Allocator>, OIt, OItEnd, Allocator>;

    using value_type = typename base_type::value_type;
    using error_type = typename base_type::error_type;
    using return_type = typename base_type::return_type;

    using serialize_seq_type = typename base_type::serialize_seq_type;
    using serialize_tuple_type = typename base_type::serialize_tuple_type;
    using serialize_map_type = typename base_type::serialize_map_type;
    using serialize_struct_type = typename base_type::serialize_struct_type;

    constexpr json_serializer(OIt beg, OItEnd end, Allocator const& alloc = Allocator{})  //
      noexcept(noexcept(Allocator(alloc)) && noexcept(Allocator()))
        : base_type(m_it, end, alloc)
        , m_it(beg) {}

private:
    OIt m_it;
};

template<typename OIt, std::sentinel_for<OIt> OItEnd, typename Allocator>
struct json_array_serializer : json_serializer_base<json_array_serializer<OIt, OItEnd, Allocator>, OIt, OItEnd, Allocator> {
    using base_type = json_serializer_base<json_array_serializer<OIt, OItEnd, Allocator>, OIt, OItEnd, Allocator>;

    using value_type = typename base_type::value_type;
    using error_type = typename base_type::error_type;
    using return_type = typename base_type::return_type;

    using serialize_seq_type = typename base_type::serialize_seq_type;
    using serialize_tuple_type = typename base_type::serialize_tuple_type;
    using serialize_map_type = typename base_type::serialize_map_type;
    using serialize_struct_type = typename base_type::serialize_struct_type;

    constexpr json_array_serializer(OIt& it, OItEnd end, Allocator const& alloc = Allocator{})  //
      noexcept(noexcept(Allocator(alloc)) && noexcept(Allocator()))
        : base_type(it, end, alloc) {}

    template<typename Elem>
    constexpr auto serialize_element(Elem&& elem) -> return_type {
        if (m_counter > 0) {
            this->emit_char(',') ? return_type{} : std::unexpected{"buffer overrun"};
        }
        ++m_counter;

        return serialize(*this, static_cast<Elem const&>(elem));
    }

    constexpr auto end() -> return_type { return this->emit_char(']') ? return_type{} : std::unexpected{"buffer overrun"}; }

private:
    usize m_counter = 0;
};

template<typename OIt, std::sentinel_for<OIt> OItEnd, typename Allocator>
struct json_object_serializer : json_serializer_base<json_object_serializer<OIt, OItEnd, Allocator>, OIt, OItEnd, Allocator> {
    using base_type = json_serializer_base<json_object_serializer<OIt, OItEnd, Allocator>, OIt, OItEnd, Allocator>;

    using value_type = typename base_type::value_type;
    using error_type = typename base_type::error_type;
    using return_type = typename base_type::return_type;

    using serialize_seq_type = typename base_type::serialize_seq_type;
    using serialize_tuple_type = typename base_type::serialize_tuple_type;
    using serialize_map_type = typename base_type::serialize_map_type;
    using serialize_struct_type = typename base_type::serialize_struct_type;

    constexpr json_object_serializer(OIt& it, OItEnd end, Allocator const& alloc = Allocator{})  //
      noexcept(noexcept(Allocator(alloc)) && noexcept(Allocator()))
        : base_type(it, end, alloc) {}

    template<typename Key, typename Value>
    constexpr auto serialize_entry(Key const& key, Value&& value) -> return_type {
        if (m_counter > 0) {
            this->emit_char(',') ? return_type{} : std::unexpected{"buffer overrun"};
        }
        ++m_counter;

        TRYX(serialize(*this, key));
        if (!this->emit_char(':')) {
            return std::unexpected{"buffer overrun"};
        }
        TRYX(serialize(*this, std::forward<Value>(value)));

        return {};
    }

    constexpr auto end() -> return_type { return this->emit_char('}') ? return_type{} : std::unexpected{"buffer overrun"}; }

private:
    usize m_counter = 0;
};

template<typename Extender, typename OIt, std::sentinel_for<OIt> OItEnd, typename Allocator>
constexpr auto json_serializer_base<Extender, OIt, OItEnd, Allocator>::serialize_seq(std::optional<usize>) -> std::expected<serialize_seq_type, error_type> {
    using U = std::expected<serialize_seq_type, error_type>;

    return this->emit_char('[') ? U{json_array_serializer(m_it, m_end, m_allocator)} : std::unexpected{"buffer overrun"};
}

template<typename Extender, typename OIt, std::sentinel_for<OIt> OItEnd, typename Allocator>
template<typename K, typename V>
constexpr auto json_serializer_base<Extender, OIt, OItEnd, Allocator>::serialize_map(std::optional<usize>) -> std::expected<serialize_map_type, error_type> {
    using U = std::expected<serialize_map_type, error_type>;

    return this->emit_char('{') ? U{serialize_map_type(m_it, m_end, m_allocator)} : std::unexpected{"buffer overrun"};
}

template<typename Extender, typename OIt, std::sentinel_for<OIt> OItEnd, typename Allocator>
template<typename K>
constexpr auto json_serializer_base<Extender, OIt, OItEnd, Allocator>::serialize_struct(std::optional<usize>) -> std::expected<serialize_struct_type, error_type> {
    using U = std::expected<serialize_map_type, error_type>;

    return this->emit_char('{') ? U{serialize_map_type(m_it, m_end, m_allocator)} : std::unexpected{"buffer overrun"};
}

static_assert(concepts::serializer<json_serializer<std::back_insert_iterator<std::string>, std::unreachable_sentinel_t>>);

}  // namespace stf::serde
