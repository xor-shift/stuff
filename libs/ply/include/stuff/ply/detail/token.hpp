#pragma once

#include <stuff/ply/detail/error.hpp>

#include <stuff/expected.hpp>

#include <charconv>
#include <ranges>

namespace stf::ply::detail {

struct token {
    std::string_view m_content;
    usize m_row;
    usize m_col;

    constexpr auto generate_error(error_class error_class, std::string_view additional = "", usize offset = 0, usize length = 0) const -> stf::unexpected<error> {
        return stf::unexpected{error{
          .m_class = error_class,
          .m_location = get_location(),
          .m_additional = additional,
        }};
    }

    constexpr auto get_location() const -> ascii_location {
        return ascii_location{
          .m_row = m_row,
          .m_col = m_col,
          .m_col_span = m_content.size(),
        };
    }

    constexpr auto set_error_location(error err) const -> error {
        err.m_location = get_location();
        return err;
    }

    template<typename T>
    constexpr auto as_scalar() const -> stf::expected<T, error> {
        auto ret = T{};
        const auto res = std::from_chars(m_content.data(), m_content.data() + m_content.size(), ret);

        if (res.ec != std::errc{}) {
            return generate_error(error_class::bad_data_in_token, "error while converting to a scalar type, res.ec is not std::errc{}");
        }

        if (res.ptr != m_content.data() + m_content.size()) {
            return generate_error(error_class::excess_data_in_token, "error while converting to a scalar type, excess data after parsing scalar");
        }

        return ret;
    }

    constexpr auto substring(usize pos, usize length = std::numeric_limits<usize>::max()) const -> token {
        return token {
            .m_content = m_content.substr(pos, length),
            .m_row = m_row,
            .m_col = m_col + pos,
        };
    }
};

struct line {
    std::string m_content;
    usize m_row;

    /// @return
    /// The returned token objets' lifetimes are bound by this line object
    constexpr auto tokens() const -> std::vector<token> {
       auto tokens_view = m_content                                                                                                    //
                         | std::views::enumerate                                                                                        //
                         | std::views::chunk_by([](auto p0, auto p1) { return (std::get<1>(p0) == ' ') == (std::get<1>(p1) == ' '); })  //
                         | std::views::filter([](auto chunk) { return !std::ranges::any_of(chunk, [](auto p) { return std::get<1>(p) == ' '; }); });

        auto ret = std::vector<token>{};

        /*for (auto chunk : tokens_view) {
            auto str_view = chunk | std::views::transform([](auto p) { return std::get<1>(p); });

            auto tok = token{
              .m_content{std::ranges::begin(str_view), std::ranges::end(str_view)},
              .m_row = m_row,
              .m_col = static_cast<usize>(std::get<0>(*std::ranges::begin(chunk))),
            };

            ret.emplace_back(std::move(tok));
        }*/

        for (auto chunk : tokens_view) {
            auto size = std::ranges::distance(chunk);
            auto start = static_cast<usize>(std::get<0>(*std::ranges::begin(chunk)));

            auto&& tok = token {
              .m_content{m_content.data() + start, m_content.data() + start + size},
              .m_row = m_row,
              .m_col = start,
            };

            ret.emplace_back(std::move(tok));
        }

        return ret;
    }
};

}  // namespace stf::ply::detail
