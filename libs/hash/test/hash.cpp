#include <stuff/hash.hpp>
#include <stuff/random.hpp>

#include <gtest/gtest.h>

TEST(hash, basic) {
    auto arr = std::array<int, 4>{1, 2, 3, 4};
    ASSERT_NE(stf::range_hash(arr), 0uz);
}

TEST(hash, sanity_check) {


    const auto bad_lf = set_bad.load_factor();
    const auto bad_size = set_bad.size();
    const auto bad_bucket_count = set_bad.bucket_count();
    const auto bad_call_count = bad_int_hash::m_calls;

    const auto reference_lf = set_reference.load_factor();
    const auto reference_size = set_reference.size();
    const auto reference_bucket_count = set_reference.bucket_count();
    const auto reference_call_count = good_int_hash::m_calls;

    std::ignore = 0;
}

TEST(hash, load_factor_pairs_tuples) {
    auto rand_engine = stf::random::xoshiro_256pp{rand_seed};
    auto rand_dist = std::uniform_int_distribution<int>{
      std::numeric_limits<int>::min(),  //
      std::numeric_limits<int>::max()   //
    };

    auto set_reference = std::unordered_set<int>{};
    auto set_pair = std::unordered_set<std::pair<int, int>>{};
    for (auto i = 0uz; i < 1024uz; i++) {
        set_pair.emplace(rand_dist(rand_engine), rand_dist(rand_engine));
        set_reference.emplace(rand_dist(rand_engine));
    }

    const auto reference_lf = set_reference.load_factor();
    const auto pair_lf = set_pair.load_factor();

    std::ignore = 0;
}
