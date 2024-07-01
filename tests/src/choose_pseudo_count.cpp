#include <gtest/gtest.h>

#include "compare_almost_equal.h"
#include "simulate_vector.h"

#include "choose_pseudo_count.hpp"

#include <cmath>
#include <random>

TEST(ChoosePseudoCount, FindQuantile) {
    size_t n = 101;
    std::vector<double> contents(n);
    for (size_t r = 0; r < n; ++r) {
        contents[r] = static_cast<double>(r) / 100;
    }

    {
        auto copy = contents;
        compare_almost_equal(scran::choose_pseudo_count::internal::find_quantile(0.1, n, copy.data()), 0.1);
        compare_almost_equal(scran::choose_pseudo_count::internal::find_quantile(0.1111, n, copy.data()), 0.1111);
        compare_almost_equal(scran::choose_pseudo_count::internal::find_quantile(0.9, n, copy.data()), 0.9);
        compare_almost_equal(scran::choose_pseudo_count::internal::find_quantile(0.995, n, copy.data()), 0.995);
    }

    // Works as expected.
    scran::choose_pseudo_count::Options opt;
    auto left = scran::choose_pseudo_count::internal::find_quantile(0.05, n - 1, contents.data() + 1); // ignore the zero at the start.
    auto right = scran::choose_pseudo_count::internal::find_quantile(0.95, n - 1, contents.data() + 1);
    auto chosen = scran::choose_pseudo_count::compute(n, contents.data(), opt);
    compare_almost_equal(chosen, (1/left - 1 /right) / (8 * 0.1));

    opt.quantile = 0;
    chosen = scran::choose_pseudo_count::compute(n, contents.data(), opt);
    compare_almost_equal(chosen, (1/0.01 - 1) / (8 * 0.1));
}

TEST(ChoosePseudoCount, MoreInteresting) {
    size_t n = 99;
    auto contents = simulate_vector(n, /* lower = */ 0.2, /* upper = */ 2, /* seed = */ 10000);

    {
        scran::choose_pseudo_count::Options opt;
        auto out = scran::choose_pseudo_count::compute(n, contents.data(), opt);
        EXPECT_TRUE(out > 1);
        EXPECT_TRUE(out < 5);
    }

    {
        scran::choose_pseudo_count::Options opt;
        opt.min_value = 10;
        auto out = scran::choose_pseudo_count::compute(n, contents.data(), opt);
        EXPECT_EQ(out, 10);
    }

    {
        scran::choose_pseudo_count::Options opt;
        opt.max_bias = 1;
        auto out = scran::choose_pseudo_count::compute(n, contents.data(), opt);
        EXPECT_EQ(out, 1);
    }
}

TEST(ChoosePseudoCount, EdgeCases) {
    std::vector<double> contents;

    scran::choose_pseudo_count::Options opt;
    EXPECT_EQ(scran::choose_pseudo_count::compute(0, contents.data(), opt), 1);

    contents.push_back(0);
    EXPECT_EQ(scran::choose_pseudo_count::compute(1, contents.data(), opt), 1);

    contents.push_back(1);
    EXPECT_EQ(scran::choose_pseudo_count::compute(2, contents.data(), opt), 1);

    contents.push_back(0.1);
    auto out = scran::choose_pseudo_count::compute(3, contents.data(), opt);
    EXPECT_NE(out, 1);
    auto out2 = scran::choose_pseudo_count::compute(2, contents.data() + 1, opt);
    EXPECT_EQ(out, out2);
}
