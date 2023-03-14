#pragma once

namespace stf::blas {

namespace detail {

constexpr auto swizzle_index(char c) -> usize {
    switch (c) {
        case 's': [[fallthrough]];
        case 'r': [[fallthrough]];
        case 'x': return 0;

        case 't': [[fallthrough]];
        case 'g': [[fallthrough]];
        case 'y': return 1;

        case 'p': [[fallthrough]];
        case 'b': [[fallthrough]];
        case 'z': return 2;

        case 'q': [[fallthrough]];
        case 'a': [[fallthrough]];
        case 'w': return 3;

        default: return std::numeric_limits<usize>::max();
    }
}

template<string_literal Lit>
constexpr auto swizzle_needed_dimensions() -> usize {
    usize dims = 0;
    for (char c : Lit) {
        dims = std::max(dims, swizzle_index(c) + 1);
    }
    return dims;
}

template<string_literal Lit, typename T>
concept can_swizzle = concepts::vector<T> && (T::size >= swizzle_needed_dimensions<Lit>());

}  // namespace detail

template<string_literal Lit, concepts::vector T>
constexpr auto swizzle(T const& vec) -> concepts::nd_vector_of_t<typename T::value_type, Lit.size()> auto{
    static_assert(detail::can_swizzle<Lit, T>);

    concepts::nd_vector_of_t<typename T::value_type, Lit.size()> auto ret =
      typename T::template rebind_n<typename T::value_type, Lit.size()>{};

    for (usize i = 0; char c : Lit) {
        usize index = detail::swizzle_index(c);
        ret[i++] = vec[index];
    }

    return ret;
}

}
