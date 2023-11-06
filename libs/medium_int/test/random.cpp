#include "./random.hpp"

#include <mutex>

namespace stf::tests {

namespace {

auto generate_seed() -> usize {
    [[maybe_unused]] const auto _ = 0; // just change this value to cause a recompilation of this TU

    return std::hash<std::string_view>{}(__DATE__ __TIME__);
}

}

auto get_rng_seed() -> usize {
    static usize seed;

    static std::once_flag s_once_flag {};
    std::call_once(s_once_flag, [] {
        seed = generate_seed();
    });

    return seed;
}

}
