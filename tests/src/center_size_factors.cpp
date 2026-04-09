#include <gtest/gtest.h>

#include <limits>
#include <cmath>
#include <vector>

#include "scran_norm/center_size_factors.hpp"

/***************************************/

class CenterSizeFactorsSimpleTest : public ::testing::TestWithParam<std::vector<double> > {};

TEST_P(CenterSizeFactorsSimpleTest, Basic) {
    auto sf = GetParam();
    scran_norm::CenterSizeFactorsOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    opt.diagnostics = &diag;

    auto copy = sf;
    auto out = scran_norm::center_size_factors(copy.size(), copy.data(), opt);
    {
        EXPECT_FALSE(diag.has_zero);
        EXPECT_FALSE(diag.has_negative);
        EXPECT_FALSE(diag.has_nan);
        EXPECT_FALSE(diag.has_infinite);
        EXPECT_GT(out, 0);

        double middle = std::accumulate(copy.begin(), copy.end(), 0.0) / copy.size();
        EXPECT_FLOAT_EQ(middle, 1);
    }

    // Works fine if we don't supply a diagnostic object.
    opt.ignore_invalid = false;
    opt.diagnostics = NULL;
    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors(copy2.size(), copy2.data(), opt);
        EXPECT_EQ(out, out2);
        EXPECT_EQ(copy, copy2);
    }

    // Works fine if we don't do any validity checking.
    opt.ignore_invalid = false;
    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors(copy2.size(), copy2.data(), opt);
        EXPECT_EQ(out, out2);
        EXPECT_EQ(copy, copy2);
    }

    // Reports the final mean if requested.
    opt.report_final = true;
    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors(copy2.size(), copy2.data(), opt);
        EXPECT_EQ(out2, 1);
        EXPECT_EQ(copy, copy2);
    }
    opt.report_final = false;

    // Respects alternative centers.
    opt.center = 10;
    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors(copy2.size(), copy2.data(), opt);
        EXPECT_EQ(out, out2);
        double middle = std::accumulate(copy2.begin(), copy2.end(), 0.0) / copy2.size();
        EXPECT_FLOAT_EQ(middle, 10);
    }
}

INSTANTIATE_TEST_SUITE_P(
    CenterSizeFactors,
    CenterSizeFactorsSimpleTest,
    ::testing::Values(
        std::vector<double>{ 0.1, 1, 10, 20 },
        std::vector<double>{ 5, 3, 4, 2, 1 },
        std::vector<double>{ 0.51, 0.35, 0.48, 0.21, 0.11, 0.67 }
    )
);

/***************************************/

class CenterSizeFactorsIgnoreZerosTest : public ::testing::TestWithParam<std::vector<double> > {};

TEST_P(CenterSizeFactorsIgnoreZerosTest, Basic) {
    auto sf = GetParam();
    const int num = sf.size();
    std::vector<double> stripped;
    std::vector<int> idx;
    for (int i = 0; i < num; ++i) {
        if (sf[i]) {
            idx.push_back(i);
            stripped.push_back(sf[i]);
        }
    }

    scran_norm::CenterSizeFactorsOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    opt.diagnostics = &diag;

    auto copy = sf;
    auto out = scran_norm::center_size_factors(copy.size(), copy.data(), opt);
    {
        EXPECT_TRUE(diag.has_zero);

        const double middle = std::accumulate(copy.begin(), copy.end(), 0.0) / stripped.size(); 
        EXPECT_FLOAT_EQ(middle, 1);
    }

    opt.diagnostics = NULL;
    {
        auto copy2 = stripped;
        auto out2 = scran_norm::center_size_factors(copy2.size(), copy2.data(), opt);
        EXPECT_EQ(out, out2);

        std::vector<double> expected(num);
        const int nnz = stripped.size(); 
        for (int i = 0; i < nnz; ++i) {
            expected[idx[i]] = copy2[i];
        }
        EXPECT_EQ(expected, copy);
    }

    // Restoring our acceptance of zeros.
    opt.ignore_invalid = false;
    {
        auto copy3 = sf;
        auto out3 = scran_norm::center_size_factors(copy3.size(), copy3.data(), opt);
        EXPECT_LT(out3, out);

        for (int i = 0; i < num; ++i) {
            EXPECT_FLOAT_EQ(copy3[i], sf[i] / out3);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(
    CenterSizeFactors,
    CenterSizeFactorsIgnoreZerosTest,
    ::testing::Values(
        std::vector<double>{ 0, 0.1, 1, 10, 20, 0 },
        std::vector<double>{ 5, 0, 3, 0, 4, 0, 2, 0, 1 }
    )
);

TEST(CenterSizeFactors, IgnoreZerosAll) {
    std::vector<double> empty(10);
    auto copy = empty;

    // All-zeros avoid division by zero.
    scran_norm::CenterSizeFactorsOptions opt;
    auto out4 = scran_norm::center_size_factors(copy.size(), copy.data(), opt);

    EXPECT_EQ(out4, 0);
    EXPECT_EQ(copy, empty); 
}

/***************************************/

class CenterSizeFactorsBlockedSimpleTest : public ::testing::TestWithParam<int> {
protected:
    std::vector<double> sf;
    std::vector<int> block;

    void initialize(int scenario) {
        if (scenario == 0) {
            sf = std::vector<double>{ 3, 1, 2, 6, 5, 4 };
            block = std::vector<int>{ 0, 0, 0, 1, 1, 1 };
        } else if (scenario == 1) {
            sf = std::vector<double>{ 3, 1, 5, 2, 6, 5, 4, 1, 3 };
            block = std::vector<int>{ 1, 0, 1, 2, 1, 2, 0, 2, 1 };
        } else {
            sf = std::vector<double>{ 0.1, 10, 5.2, 1.8, 2.2, 0.01, 2.3 };
            block = std::vector<int>{   1,  2,   0,   1,   2,    0,   1 };
        }
    }

    std::vector<double> compute_means_manual() const {
        std::vector<double> means;
        std::vector<int> count;

        const std::size_t num = sf.size();
        for (std::size_t i = 0; i < num; ++i) {
            const std::size_t b = block[i];
            if (b >= means.size()) {
                means.resize(b + 1);
                count.resize(b + 1);
            }
            means[b] += sf[i];
            count[b] += 1;
        }

        const std::size_t num_groups = means.size();
        for (std::size_t g = 0; g < num_groups; ++g) {
            means[g] /= count[g];
        }

        return means;
    }
};

TEST_P(CenterSizeFactorsBlockedSimpleTest, Lowest) {
    initialize(GetParam());

    scran_norm::CenterSizeFactorsBlockedOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    opt.diagnostics = &diag;

    auto copy = sf;
    auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
    {
        EXPECT_FALSE(diag.has_zero);

        const auto means = compute_means_manual();
        const std::size_t num_groups = means.size();
        EXPECT_EQ(num_groups, out.size());
        for (std::size_t g = 0; g < num_groups; ++g) {
            EXPECT_FLOAT_EQ(means[g], out[g]);
        }

        const double min = *std::min_element(means.begin(), means.end());
        const std::size_t num_sf = sf.size(); 
        for (std::size_t i = 0; i < num_sf; ++i) {
            EXPECT_FLOAT_EQ(copy[i], sf[i] / min);
        }
    }

    opt.diagnostics = NULL;
    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors_blocked(copy2.size(), copy2.data(), block.data(), opt);
        EXPECT_EQ(out, out2);
        EXPECT_EQ(copy, copy2);
    }

    opt.report_final = true;
    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors_blocked(copy2.size(), copy2.data(), block.data(), opt);

        const double min = *std::min_element(out.begin(), out.end());
        const std::size_t num_groups = out.size();
        for (std::size_t g = 0; g < num_groups; ++g) {
            EXPECT_FLOAT_EQ(out2[g], out[g] / min);
        }

        EXPECT_EQ(copy, copy2);
    }
}

TEST_P(CenterSizeFactorsBlockedSimpleTest, PerBlock) {
    initialize(GetParam());

    scran_norm::CenterSizeFactorsBlockedOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    opt.diagnostics = &diag;
    opt.block_mode = scran_norm::CenterBlockMode::PER_BLOCK;

    auto copy = sf;
    auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
    {
        EXPECT_FALSE(diag.has_zero);

        const auto means = compute_means_manual();
        const std::size_t num_groups = means.size();
        EXPECT_EQ(num_groups, out.size());
        for (std::size_t g = 0; g < num_groups; ++g) {
            EXPECT_FLOAT_EQ(means[g], out[g]);
        }

        const std::size_t num_sf = sf.size(); 
        for (std::size_t i = 0; i < num_sf; ++i) {
            EXPECT_FLOAT_EQ(copy[i], sf[i] / means[block[i]]);
        }
    }

    opt.diagnostics = NULL;
    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors_blocked(copy2.size(), copy2.data(), block.data(), opt);
        EXPECT_EQ(out, out2);
        EXPECT_EQ(copy, copy2);
    }

    opt.report_final = true;
    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors_blocked(copy2.size(), copy2.data(), block.data(), opt);

        const std::size_t num_groups = out.size();
        for (std::size_t g = 0; g < num_groups; ++g) {
            EXPECT_FLOAT_EQ(out2[g], 1);
        }

        EXPECT_EQ(copy, copy2);
    }
}

TEST_P(CenterSizeFactorsBlockedSimpleTest, Custom) {
    initialize(GetParam());
    const auto means = compute_means_manual();

    scran_norm::CenterSizeFactorsBlockedOptions opt;
    opt.block_mode = scran_norm::CenterBlockMode::CUSTOM;
    {
        auto copy = sf;
        EXPECT_ANY_THROW(scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt));
    }

    opt.custom_centers.emplace();
    {
        auto copy = sf;
        EXPECT_ANY_THROW(scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt));
    }

    std::vector<double> targets;
    const std::size_t num_groups = means.size();
    for (std::size_t g = 0; g < num_groups; ++g) {
        targets.push_back((g + 1) / 3.0);
    }

    opt.custom_centers = targets;
    auto copy = sf;
    auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);

    EXPECT_EQ(num_groups, out.size());
    for (std::size_t g = 0; g < num_groups; ++g) {
        EXPECT_FLOAT_EQ(means[g], out[g]);
    }

    const std::size_t num_sf = sf.size(); 
    for (std::size_t i = 0; i < num_sf; ++i) {
        const auto b = block[i];
        EXPECT_FLOAT_EQ(copy[i], sf[i] * targets[b] / out[b]);
    }

    opt.report_final = true;
    auto copy2 = sf;
    auto out2 = scran_norm::center_size_factors_blocked(copy2.size(), copy2.data(), block.data(), opt);
    EXPECT_EQ(copy2, copy);
    EXPECT_EQ(out2, targets);
}

INSTANTIATE_TEST_SUITE_P(
    CenterSizeFactors,
    CenterSizeFactorsBlockedSimpleTest,
    ::testing::Values(0, 1, 2)
);

/***************************************/

class CenterSizeFactorsBlockedZerosTest : public ::testing::TestWithParam<int> {
protected:
    std::vector<double> sf;
    std::vector<int> block;

    void initialize(int scenario) {
        if (scenario == 0) {
            sf = std::vector<double>{ 3, 1, 0, 6, 0, 4 };
            block = std::vector<int>{ 0, 0, 0, 1, 1, 1 };
        } else if (scenario == 1) {
            sf = std::vector<double>{ 0, 1, 0, 2, 6, 5, 0, 1, 3 };
            block = std::vector<int>{ 1, 0, 1, 2, 1, 2, 0, 2, 1 };
        } else {
            sf = std::vector<double>{ 0.1,  0, 5.2, 0, 2.2, 0.01, 0 };
            block = std::vector<int>{   1,  2,   0, 1,   2,    0, 1 };
        }
    }

    std::vector<double> compute_means_manual(bool use_zeros) const {
        std::vector<double> means;
        std::vector<int> count;

        const std::size_t num = sf.size();
        for (std::size_t i = 0; i < num; ++i) {
            if (use_zeros || sf[i]) {
                const std::size_t b = block[i];
                if (b >= means.size()) {
                    means.resize(b + 1);
                    count.resize(b + 1);
                }
                means[b] += sf[i];
                count[b] += 1;
            }
        }

        const std::size_t num_groups = means.size();
        for (std::size_t g = 0; g < num_groups; ++g) {
            means[g] /= count[g];
        }

        return means;
    }
};

TEST_P(CenterSizeFactorsBlockedZerosTest, Lowest) {
    initialize(GetParam());

    scran_norm::CenterSizeFactorsBlockedOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    opt.diagnostics = &diag;

    auto copy = sf;
    auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
    EXPECT_TRUE(diag.has_zero);

    const auto means = compute_means_manual(false);
    const std::size_t num_groups = means.size();
    EXPECT_EQ(num_groups, out.size());
    for (std::size_t g = 0; g < num_groups; ++g) {
        EXPECT_FLOAT_EQ(means[g], out[g]);
    }

    const double min = *std::min_element(means.begin(), means.end());
    const std::size_t num_sf = sf.size(); 
    for (std::size_t i = 0; i < num_sf; ++i) {
        EXPECT_FLOAT_EQ(copy[i], sf[i] / min);
    }
}

TEST_P(CenterSizeFactorsBlockedZerosTest, PerBlock) {
    initialize(GetParam());

    scran_norm::CenterSizeFactorsBlockedOptions opt;
    opt.block_mode = scran_norm::CenterBlockMode::PER_BLOCK;

    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors_blocked(copy2.size(), copy2.data(), block.data(), opt);

        const auto means = compute_means_manual(false);
        const std::size_t num_groups = means.size();
        EXPECT_EQ(num_groups, out2.size());
        for (std::size_t g = 0; g < num_groups; ++g) {
            EXPECT_FLOAT_EQ(means[g], out2[g]);
        }

        const std::size_t num_sf = sf.size(); 
        for (std::size_t i = 0; i < num_sf; ++i) {
            EXPECT_FLOAT_EQ(copy2[i], sf[i] / out2[block[i]]);
        }
    }

    // Unless we force them to be acknowledged.
    opt.ignore_invalid = false;
    {
        auto copy2 = sf;
        auto out2 = scran_norm::center_size_factors_blocked(copy2.size(), copy2.data(), block.data(), opt);

        const auto means = compute_means_manual(true);
        const std::size_t num_groups = means.size();
        EXPECT_EQ(num_groups, out2.size());
        for (std::size_t g = 0; g < num_groups; ++g) {
            EXPECT_FLOAT_EQ(means[g], out2[g]);
        }

        const std::size_t num_sf = sf.size(); 
        for (std::size_t i = 0; i < num_sf; ++i) {
            EXPECT_FLOAT_EQ(copy2[i], sf[i] / means[block[i]]);
        }
    }
}

TEST_P(CenterSizeFactorsBlockedZerosTest, Custom) {
    initialize(GetParam());
    const auto means = compute_means_manual(false);

    scran_norm::CenterSizeFactorsBlockedOptions opt;
    opt.block_mode = scran_norm::CenterBlockMode::CUSTOM;

    std::vector<double> targets;
    const std::size_t num_groups = means.size();
    for (std::size_t g = 0; g < num_groups; ++g) {
        targets.push_back((g * 2 + 3) / 5.0);
    }
    opt.custom_centers = targets;

    auto copy2 = sf;
    auto out2 = scran_norm::center_size_factors_blocked(copy2.size(), copy2.data(), block.data(), opt);

    EXPECT_EQ(num_groups, out2.size());
    for (std::size_t g = 0; g < num_groups; ++g) {
        EXPECT_FLOAT_EQ(means[g], out2[g]);
    }

    const std::size_t num_sf = sf.size(); 
    for (std::size_t i = 0; i < num_sf; ++i) {
        EXPECT_FLOAT_EQ(copy2[i], sf[i] * targets[block[i]] / out2[block[i]]);
    }
}

INSTANTIATE_TEST_SUITE_P(
    CenterSizeFactors,
    CenterSizeFactorsBlockedZerosTest,
    ::testing::Values(0, 1, 2)
);

/***************************************/

class CenterSizeFactorsBlockedZerosAllTest : public ::testing::Test {};

TEST_F(CenterSizeFactorsBlockedZerosAllTest, Lowest) {
    std::vector<double> partial { 0, 0, 0, 1, 2, 3, 4, 5, 6 };
    std::vector<int> block { 0, 0, 0, 1, 1, 1, 2, 2, 2 };

    scran_norm::CenterSizeFactorsBlockedOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    opt.diagnostics = &diag;

    std::vector<double> expected { 0, 0, 0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0 };
    {
        auto copy = partial;
        auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
        EXPECT_TRUE(diag.has_zero);
        EXPECT_EQ(out[0], 0);
        EXPECT_EQ(out[1], 2);
        EXPECT_EQ(out[2], 5);
        EXPECT_EQ(copy, expected);
    }

    opt.report_final = true;
    opt.diagnostics = NULL;
    {
        auto copy = partial;
        auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
        EXPECT_TRUE(diag.has_zero);
        EXPECT_EQ(out[0], 0);
        EXPECT_EQ(out[1], 1);
        EXPECT_EQ(out[2], 2.5);
        EXPECT_EQ(copy, expected);
    }

    {
        std::vector<double> empty(9, 0);
        auto copy = empty;
        auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
        EXPECT_EQ(out[0], 0);
        EXPECT_EQ(out[1], 0);
        EXPECT_EQ(out[2], 0);
        EXPECT_EQ(copy, empty);
    }
}

TEST_F(CenterSizeFactorsBlockedZerosAllTest, PerBlock) {
    std::vector<double> partial { 0, 0, 0, 1, 2, 3, 4, 5, 6 };
    std::vector<int> block { 0, 0, 0, 1, 1, 1, 2, 2, 2 };

    scran_norm::CenterSizeFactorsBlockedOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    opt.diagnostics = &diag;
    opt.block_mode = scran_norm::CenterBlockMode::PER_BLOCK;

    std::vector<double> expected { 0, 0, 0, 0.5, 1.0, 1.5, 0.8, 1, 1.2 };
    {
        auto copy = partial;
        auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
        EXPECT_TRUE(diag.has_zero);
        EXPECT_EQ(out[0], 0);
        EXPECT_EQ(out[1], 2);
        EXPECT_EQ(out[2], 5);

        const std::size_t num_sf = partial.size();
        for (std::size_t i = 0; i < num_sf; ++i) {
            EXPECT_FLOAT_EQ(copy[i], expected[i]);
        }
    }

    opt.report_final = true;
    opt.diagnostics = NULL;
    {
        auto copy = partial;
        auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
        EXPECT_TRUE(diag.has_zero);
        EXPECT_EQ(out[0], 0);
        EXPECT_EQ(out[1], 1);
        EXPECT_EQ(out[2], 1);

        const std::size_t num_sf = partial.size();
        for (std::size_t i = 0; i < num_sf; ++i) {
            EXPECT_FLOAT_EQ(copy[i], expected[i]);
        }
    }

    {
        std::vector<double> empty(9, 0);
        auto copy = empty;
        auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
        EXPECT_EQ(out[0], 0);
        EXPECT_EQ(out[1], 0);
        EXPECT_EQ(out[2], 0);
        EXPECT_EQ(copy, empty);
    }
}

TEST_F(CenterSizeFactorsBlockedZerosAllTest, Custom) {
    std::vector<double> partial { 0, 0, 0, 1, 2, 3, 4, 5, 6 };
    std::vector<int> block { 0, 0, 0, 1, 1, 1, 2, 2, 2 };

    scran_norm::CenterSizeFactorsBlockedOptions opt;
    scran_norm::SizeFactorDiagnostics diag;
    opt.diagnostics = &diag;
    opt.block_mode = scran_norm::CenterBlockMode::CUSTOM;
    opt.custom_centers = std::vector<double>{ 20, 10, 5 };

    std::vector<double> expected { 0, 0, 0, 5, 10, 15, 4, 5, 6 };
    {
        auto copy = partial;
        auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
        EXPECT_TRUE(diag.has_zero);
        EXPECT_EQ(out[0], 0);
        EXPECT_EQ(out[1], 2);
        EXPECT_EQ(out[2], 5);
        EXPECT_EQ(copy, expected);
    }

    opt.report_final = true;
    opt.diagnostics = NULL;
    {
        auto copy = partial;
        auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
        EXPECT_TRUE(diag.has_zero);
        EXPECT_EQ(out[0], 0);
        EXPECT_EQ(out[1], 10);
        EXPECT_EQ(out[2], 5);
        EXPECT_EQ(copy, expected);
    }

    {
        std::vector<double> empty(9, 0);
        auto copy = empty;
        auto out = scran_norm::center_size_factors_blocked(copy.size(), copy.data(), block.data(), opt);
        EXPECT_EQ(out[0], 0);
        EXPECT_EQ(out[1], 0);
        EXPECT_EQ(out[2], 0);
        EXPECT_EQ(copy, empty);
    }
}
