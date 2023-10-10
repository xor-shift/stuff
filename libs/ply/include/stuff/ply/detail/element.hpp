#pragma once

#include <stuff/ply/detail/error.hpp>
#include <stuff/ply/detail/property.hpp>
#include <stuff/ply/detail/token.hpp>

namespace stf::ply {

struct element {
    std::string m_name;
    std::vector<property> m_properties;
    usize m_size;

    static constexpr auto from_tokens(std::span<const detail::token> tokens, usize line) -> std::expected<element, error> {
        if (tokens.size() != 3) {
            return std::unexpected{error::new_line_error(error_class::insufficient_tokens, line)};
        }

        if (tokens[0].m_content != "element") {
            return tokens[0].generate_error(error_class::internal, "internal error: expected \"element\"");
        }

        return element{
          .m_name = std::string(tokens[1].m_content),
          .m_properties{},
          .m_size = TRYX(tokens[2].as_scalar<usize>()),
        };
    }
};

namespace detail {

template<typename T>
struct element_traits {
    static constexpr usize property_count = 1;
    static constexpr bool is_tuple = false;

    template<usize I>
        requires(I == 0)
    using nth_type = T;
};

template<typename T, typename... Ts>
struct element_traits<std::tuple<T, Ts...>> {
    static constexpr usize property_count = 1 + sizeof...(Ts);
    static constexpr bool is_tuple = true;

    template<usize I>
    using nth_type = std::tuple_element_t<I, std::tuple<T, Ts...>>;
};

template<>
struct element_traits<std::tuple<>> {
    static constexpr usize property_count = 0;
    static constexpr bool is_tuple = true;
};

}  // namespace detail

}  // namespace stf::ply
