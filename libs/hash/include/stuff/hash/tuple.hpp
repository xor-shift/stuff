#pragma once

#include <stuff/hash/combine.hpp>

#include <tuple>

template<typename T, typename... Ts>
struct std::hash<std::tuple<T, Ts...>> {
    constexpr auto operator()(std::tuple<T, Ts...> const& v) const -> usize {
        return impl<0>(0uz, v);
    }

private:
    template<usize I>
    static constexpr auto impl(usize hash_so_far, std::tuple<T, Ts...> const& v) -> usize {
        auto const& elem = std::get<I>(v);
        const auto elem_hash = std::hash<std::remove_cvref_t<decltype(elem)>>{}(elem);

        const auto new_hash = ::stf::hash_combine(hash_so_far, elem_hash);

        if constexpr (I < sizeof...(Ts)) {
            return impl<I + 1>(new_hash, v);
        }

        return new_hash;
    }
};
