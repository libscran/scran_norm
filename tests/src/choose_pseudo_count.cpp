#include <gtest/gtest.h>

#include "scran_tests/scran_tests.hpp"
#include "quickstats/quickstats.hpp"

#include "scran_norm/choose_pseudo_count.hpp"

#include <cmath>
#include <random>

TEST(ChoosePseudoCount, FindQuantile) {
    size_t n = 101;
    std::vector<double> contents(n);
    for (size_t r = 0; r < n; ++r) {
        contents[r] = static_cast<double>(r) / 100;
    }


    // ignore the zero at the start.
    quickstats::SingleQuantileFixedNumber lowcalc(n - 1, 0.05);
    auto left = lowcalc(contents.data() + 1);
    quickstats::SingleQuantileFixedNumber hicalc(n - 1, 0.95);
    auto right = hicalc(contents.data() + 1);

    scran_norm::ChoosePseudoCountOptions opt;
    auto chosen = scran_norm::choose_pseudo_count(n, contents.data(), opt);
    scran_tests::compare_almost_equal(chosen, (1/left - 1 /right) / (8 * 0.1));

    opt.quantile = 0;
    chosen = scran_norm::choose_pseudo_count(n, contents.data(), opt);
    scran_tests::compare_almost_equal(chosen, (1/0.01 - 1) / (8 * 0.1));
}

TEST(ChoosePseudoCount, MoreInteresting) {
    size_t n = 99;
    auto contents = scran_tests::simulate_vector(n, []{
        scran_tests::SimulationParameters<> sparams;
        sparams.lower = 0.2;
        sparams.upper = 2;
        sparams.seed = 10000;
        return sparams;
    }());

    {
        scran_norm::ChoosePseudoCountOptions opt;
        auto out = scran_norm::choose_pseudo_count(n, contents.data(), opt);
        EXPECT_TRUE(out > 1);
        EXPECT_TRUE(out < 5);
    }

    {
        scran_norm::ChoosePseudoCountOptions opt;
        opt.min_value = 10;
        auto out = scran_norm::choose_pseudo_count(n, contents.data(), opt);
        EXPECT_EQ(out, 10);
    }

    {
        scran_norm::ChoosePseudoCountOptions opt;
        opt.max_bias = 1;
        auto out = scran_norm::choose_pseudo_count(n, contents.data(), opt);
        EXPECT_EQ(out, 1);
    }
}

TEST(ChoosePseudoCount, EdgeCases) {
    std::vector<double> contents;

    scran_norm::ChoosePseudoCountOptions opt;
    EXPECT_EQ(scran_norm::choose_pseudo_count(0, contents.data(), opt), 1);

    contents.push_back(0);
    EXPECT_EQ(scran_norm::choose_pseudo_count(1, contents.data(), opt), 1);

    contents.push_back(1);
    EXPECT_EQ(scran_norm::choose_pseudo_count(2, contents.data(), opt), 1);

    contents.push_back(0.1);
    auto out = scran_norm::choose_pseudo_count(3, contents.data(), opt);
    EXPECT_NE(out, 1);
    auto out2 = scran_norm::choose_pseudo_count(2, contents.data() + 1, opt);
    EXPECT_EQ(out, out2);

    opt.quantile = -1;
    std::string msg;
    try {
        scran_norm::choose_pseudo_count(3, contents.data(), opt);
    } catch (std::exception& e) {
        msg = e.what();
    }
    EXPECT_TRUE(msg.find("[0, 0.5)") != std::string::npos);

    opt.quantile = 10;
    msg.clear();
    try {
        scran_norm::choose_pseudo_count(3, contents.data(), opt);
    } catch (std::exception& e) {
        msg = e.what();
    }
    EXPECT_TRUE(msg.find("[0, 0.5)") != std::string::npos);
}
