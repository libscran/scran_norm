#include <gtest/gtest.h>

#include <limits>
#include <cmath>
#include <vector>

#include "scran_norm/center_size_factors.hpp"

/***************************************/

TEST(CenterSizeFactors, Simple) {
    std::vector<double> sf { 0.1, 1, 10, 20 };
    scran_norm::CenterSizeFactorsOptions opt;

    scran_norm::SizeFactorDiagnostics diag;
    auto copy = sf;
    auto out = scran_norm::center_size_factors(copy.size(), copy.data(), &diag, opt);
    EXPECT_FALSE(diag.has_zero);
    EXPECT_FALSE(diag.has_negative);
    EXPECT_FALSE(diag.has_nan);
    EXPECT_FALSE(diag.has_infinite);
    EXPECT_GT(out, 0);

    double middle = std::accumulate(copy.begin(), copy.end(), 0.0) / copy.size();
    EXPECT_FLOAT_EQ(middle, 1);

    // Works fine if we don't do any validity checking.
    opt.ignore_invalid = false;
    auto copy2 = sf;
    auto out2 = scran_norm::center_size_factors(copy2.size(), copy2.data(), &diag, opt);
    EXPECT_EQ(out, out2);
    EXPECT_EQ(copy, copy2);
}

TEST(CenterSizeFactors, IgnoreZeros) {
    std::vector<double> sf { 0, 1, 2, 3, 4, 5 };
    scran_norm::CenterSizeFactorsOptions opt;

    auto sf1 = sf;
    scran_norm::SizeFactorDiagnostics diag;
    auto out = scran_norm::center_size_factors(sf1.size(), sf1.data(), &diag, opt);
    EXPECT_TRUE(diag.has_zero);
    EXPECT_EQ(sf1[0], 0);
    double middle = std::accumulate(sf1.begin() + 1, sf1.end(), 0.0) / (sf1.size() - 1);
    EXPECT_FLOAT_EQ(middle, 1);

    // Zero is ignored as if it wasn't even there.
    {
        auto sf2 = sf;
        auto out2 = scran_norm::center_size_factors(sf2.size() - 1, sf2.data() + 1, NULL, opt);
        EXPECT_EQ(out, out2);
        EXPECT_EQ(sf1, sf2);
    }

    // Restoring our acceptance of zeros.
    {
        auto sf3 = sf;
        opt.ignore_invalid = false;
        auto out3 = scran_norm::center_size_factors(sf3.size(), sf3.data(), NULL, opt);
        EXPECT_LT(out3, out);
    }

    // All-zeros avoid division by zero.
    {
        std::vector<double> empty(sf.size());
        scran_norm::CenterSizeFactorsOptions opt;
        auto copy = empty;
        auto out4 = scran_norm::center_size_factors(copy.size(), copy.data(), NULL, opt);
        EXPECT_EQ(out4, 0);
        EXPECT_EQ(copy, empty); 
    }
}

TEST(CenterSizeFactors, Blocked) {
    std::vector<double> sf { 1, 2, 3, 4, 5, 6 };
    std::vector<int> block { 0, 0, 0, 1, 1, 1 };

    scran_norm::CenterSizeFactorsOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    auto sf1 = sf;
    auto out = scran_norm::center_size_factors_blocked(sf1.size(), sf1.data(), block.data(), &diag, opt);

    EXPECT_FALSE(diag.has_zero);
    EXPECT_EQ(out[0], 2);
    EXPECT_EQ(out[1], 5);

    std::vector<double> expected { 0.5, 1, 1.5, 2, 2.5, 3 };
    EXPECT_EQ(sf1, expected);

    // Trying again with the other blocking strategy.
    opt.block_mode = scran_norm::CenterBlockMode::PER_BLOCK;
    auto sf2 = sf;
    auto out2 = scran_norm::center_size_factors_blocked(sf2.size(), sf2.data(), block.data(), NULL, opt);
    EXPECT_EQ(out, out2);

    expected = std::vector<double>{ 0.5, 1, 1.5, 0.8, 1, 1.2 };
    EXPECT_EQ(sf2, expected);
}

TEST(CenterSizeFactors, BlockIgnoreZeros) {
    std::vector<double> sf { 0, 0.5, 1.5, 3, 4, 5 };
    std::vector<int> block { 0, 0, 0, 1, 1, 1 };

    auto sf1 = sf;
    scran_norm::CenterSizeFactorsOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    auto out = scran_norm::center_size_factors_blocked(sf1.size(), sf1.data(), block.data(), &diag, opt);
    EXPECT_TRUE(diag.has_zero);
    EXPECT_EQ(out[0], 1);
    EXPECT_EQ(out[1], 4);

    std::vector<double> expected{ 0, 0.5, 1.5, 3, 4, 5 };
    EXPECT_EQ(sf1, expected);

    // Works in the other mode.
    {
        scran_norm::CenterSizeFactorsOptions opt;
        opt.block_mode = scran_norm::CenterBlockMode::PER_BLOCK;
        auto sf1 = sf;
        auto out = scran_norm::center_size_factors_blocked(sf1.size(), sf1.data(), block.data(), NULL, opt);

        EXPECT_EQ(out[0], 1);
        EXPECT_EQ(out[1], 4);

        std::vector<double> expected{ 0, 0.5, 1.5, 0.75, 1, 1.25 };
        EXPECT_EQ(sf1, expected);
    }

    // Zeros are basically ignored.
    {
        auto sf2 = sf;
        auto out2 = scran_norm::center_size_factors_blocked(sf2.size() -1, sf2.data() + 1, block.data() + 1, NULL, opt);
        EXPECT_EQ(out, out2);
        EXPECT_EQ(sf1, sf2);
    }

    // Unless we force them to be acknowledged.
    {
        scran_norm::CenterSizeFactorsOptions opt;
        opt.ignore_invalid = false;
        auto sf1 = sf;
        auto out = scran_norm::center_size_factors_blocked(sf1.size(), sf1.data(), block.data(), NULL, opt);

        EXPECT_FLOAT_EQ(out[0], 2.0/3);
        EXPECT_EQ(out[1], 4);
    }

    // We can handle all-zero groups.
    {
        std::vector<double> empty(6);

        scran_norm::CenterSizeFactorsOptions opt;
        auto out = scran_norm::center_size_factors_blocked(empty.size(), empty.data(), block.data(), NULL, opt);
        EXPECT_EQ(out, std::vector<double>(2));
        EXPECT_EQ(empty, std::vector<double>(empty.size()));

        opt.block_mode = scran_norm::CenterBlockMode::PER_BLOCK;
        auto out2 = scran_norm::center_size_factors_blocked(empty.size(), empty.data(), block.data(), NULL, opt);
        EXPECT_EQ(out2, std::vector<double>(2));
        EXPECT_EQ(empty, std::vector<double>(empty.size()));
    }
}
