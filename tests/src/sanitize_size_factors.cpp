#include <gtest/gtest.h>

#include <cmath>
#include <vector>

#include "scran_tests/expect_error.hpp"

#include "scran_norm/sanitize_size_factors.hpp"

TEST(SanitizeSizeFactors, Error) {
    std::vector<double> sf { 1, 2, 3, 4, 5 };

    auto copy = sf;
    scran_norm::SanitizeSizeFactorsOptions opt;
    auto diags = scran_norm::sanitize_size_factors(copy.size(), copy.data(), opt);
    EXPECT_EQ(copy, sf);
    EXPECT_FALSE(diags.has_nan);
    EXPECT_FALSE(diags.has_infinite);
    EXPECT_FALSE(diags.has_zero);
    EXPECT_FALSE(diags.has_negative);

    sf[0] = 0;
    scran_tests::expect_error([&]() { scran_norm::sanitize_size_factors(sf.size(), sf.data(), opt); }, "zero");

    sf[0] = -1;
    scran_tests::expect_error([&]() { scran_norm::sanitize_size_factors(sf.size(), sf.data(), opt); }, "negative");

    sf[0] = std::numeric_limits<double>::quiet_NaN();
    scran_tests::expect_error([&]() { scran_norm::sanitize_size_factors(sf.size(), sf.data(), opt); }, "NaN");

    sf[0] = std::numeric_limits<double>::infinity();
    scran_tests::expect_error([&]() { scran_norm::sanitize_size_factors(sf.size(), sf.data(), opt); }, "infinite");
}

TEST(SanitizeSizeFactors, Ignored) {
    std::vector<double> sf { 0, -1, std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::infinity() };

    auto copy = sf;
    scran_norm::SanitizeSizeFactorsOptions opt;
    opt.handle_zero = scran_norm::SanitizeAction::IGNORE;
    opt.handle_negative = scran_norm::SanitizeAction::IGNORE;
    opt.handle_infinite = scran_norm::SanitizeAction::IGNORE;
    opt.handle_nan = scran_norm::SanitizeAction::IGNORE;

    auto diags = scran_norm::sanitize_size_factors(copy.size(), copy.data(), opt);
    EXPECT_TRUE(diags.has_nan);
    EXPECT_TRUE(diags.has_infinite);
    EXPECT_TRUE(diags.has_zero);
    EXPECT_TRUE(diags.has_negative);

    EXPECT_TRUE(std::isnan(copy[2]));
    copy[2] = 0;
    sf[2] = 0;
    EXPECT_EQ(copy, sf);
}

TEST(SanitizeSizeFactors, SmallestReplacement) {
    {
        std::vector<double> sf { 0.1, 0.2, 0.3, 0.4, 0.5 };
        scran_norm::SanitizeSizeFactorsOptions opt;
        opt.handle_zero = scran_norm::SanitizeAction::SANITIZE;

        auto copy = sf;
        copy[1] = 0;
        scran_norm::sanitize_size_factors(copy.size(), copy.data(), opt);

        EXPECT_EQ(copy[1], 0.1);
        auto ref = sf;
        ref[1] = 0.1;
        EXPECT_EQ(copy, ref);
    }

    {
        std::vector<double> sf { 0.5, 0.2, 0.3, 0.4, 0.01 };

        scran_norm::SanitizeSizeFactorsOptions opt;
        opt.handle_negative = scran_norm::SanitizeAction::SANITIZE;

        auto copy = sf;
        copy[2] = -1;
        scran_norm::sanitize_size_factors(copy.size(), copy.data(), opt);

        EXPECT_EQ(copy[2], 0.01);
        auto ref = sf;
        ref[2] = 0.01;
        EXPECT_EQ(copy, ref);
    }

    // Works when the smallest value is already computed.
    {
        std::vector<double> sf { 0.2, 0.5, 0.04, 0.1, 0.3 };

        scran_norm::SanitizeSizeFactorsOptions opt;
        opt.handle_negative = scran_norm::SanitizeAction::SANITIZE;
        opt.handle_zero = scran_norm::SanitizeAction::SANITIZE;

        auto copy = sf;
        copy[0] = 0;
        copy[4] = -1;
        scran_norm::sanitize_size_factors(copy.size(), copy.data(), opt);

        EXPECT_EQ(copy[0], 0.04);
        EXPECT_EQ(copy[4], 0.04);
        auto ref = sf;
        ref[0] = 0.04;
        ref[4] = 0.04;
        EXPECT_EQ(copy, ref);
    }

    // Still works in the presence of non-finite values.
    {
        std::vector<double> sf { 0.2, std::numeric_limits<double>::quiet_NaN(), 0.5, 0.03, 0.4, std::numeric_limits<double>::infinity(), 0.3 };

        scran_norm::SanitizeSizeFactorsOptions opt;
        opt.handle_negative = scran_norm::SanitizeAction::SANITIZE;
        opt.handle_zero = scran_norm::SanitizeAction::SANITIZE;
        opt.handle_infinite = scran_norm::SanitizeAction::IGNORE;
        opt.handle_nan = scran_norm::SanitizeAction::IGNORE;

        auto copy = sf;
        copy[0] = 0;
        copy.back() = -1;
        scran_norm::sanitize_size_factors(copy.size(), copy.data(), opt);

        EXPECT_EQ(copy[0], 0.03);
        EXPECT_EQ(copy.back(), 0.03);
    }
}

TEST(SanitizeSizeFactors, LargestReplacement) {
    {
        std::vector<double> sf { 0.1, 0.2, 0.3, 0.4, 0.5 };

        scran_norm::SanitizeSizeFactorsOptions opt;
        opt.handle_infinite = scran_norm::SanitizeAction::SANITIZE;

        auto copy = sf;
        copy[1] = std::numeric_limits<double>::infinity();
        scran_norm::sanitize_size_factors(copy.size(), copy.data(), opt);

        EXPECT_EQ(copy[1], 0.5);
        auto ref = sf;
        ref[1] = 0.5;
        EXPECT_EQ(copy, ref);
    }

    // Still works in the presence of non-finite values.
    {
        std::vector<double> sf { 0.1, 0.2, 0.3, 0.4, 0.5, std::numeric_limits<double>::quiet_NaN() };

        scran_norm::SanitizeSizeFactorsOptions opt;
        opt.handle_infinite = scran_norm::SanitizeAction::SANITIZE;
        opt.handle_nan = scran_norm::SanitizeAction::IGNORE;

        auto copy = sf;
        copy[2] = std::numeric_limits<double>::infinity();
        scran_norm::sanitize_size_factors(copy.size(), copy.data(), opt);

        EXPECT_EQ(copy[2], 0.5);
    }
}

TEST(SanitizeSizeFactors, Missings) {
    std::vector<double> sf { 0.1, 0.2, 0.3, 0.4, 0.5, std::numeric_limits<double>::quiet_NaN() };

    scran_norm::SanitizeSizeFactorsOptions opt;
    opt.handle_nan = scran_norm::SanitizeAction::SANITIZE;

    auto copy = sf;
    scran_norm::sanitize_size_factors(copy.size(), copy.data(), opt);

    EXPECT_EQ(copy.back(), 1);
    auto ref = sf;
    ref.back() = 1;
    EXPECT_EQ(copy, ref);
}
