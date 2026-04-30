#include <gtest/gtest.h>

#include <limits>
#include <cmath>
#include <vector>

#include "scran_tests/scran_tests.hpp"
#include "scran_norm/center_spike_in_factors.hpp"

class CenterSpikeInFactorsTest : public ::testing::TestWithParam<bool> {
protected:
    std::vector<double> endog_sf, spike_sf1, spike_sf2;
    int NC = 100;

    void SetUp() {
        endog_sf = scran_tests::simulate_vector(NC, [&]{
            scran_tests::SimulationParameters<> sparams;
            sparams.lower = 0.5;
            sparams.upper = 3;
            sparams.seed = 69;
            return sparams;
        }());

        spike_sf1 = scran_tests::simulate_vector(NC, [&]{
            scran_tests::SimulationParameters<> sparams;
            sparams.lower = 1;
            sparams.upper = 10;
            sparams.seed = 6969;
            return sparams;
        }());

        spike_sf2= scran_tests::simulate_vector(NC, [&]{
            scran_tests::SimulationParameters<> sparams;
            sparams.lower = 5;
            sparams.upper = 20;
            sparams.seed = 96969;
            return sparams;
        }());
    }
};

TEST_P(CenterSpikeInFactorsTest, Basic) {
    auto use_diag = GetParam();

    scran_norm::CenterSpikeInFactorsOptions opt;
    scran_norm::SizeFactorDiagnostics ediag, sdiag1, sdiag2;
    if (use_diag) {
        opt.endogenous_diagnostics = &ediag;
        opt.spike_in_diagnostics.push_back(&sdiag1);
        opt.spike_in_diagnostics.push_back(&sdiag2);
    }

    scran_norm::center_spike_in_factors(
        NC,
        endog_sf.data(),
        { spike_sf1.data(), spike_sf2.data() },
        opt 
    );

    EXPECT_FLOAT_EQ(NC, std::accumulate(endog_sf.begin(), endog_sf.end(), 0.0));
    EXPECT_FLOAT_EQ(NC, std::accumulate(spike_sf1.begin(), spike_sf1.end(), 0.0));
    EXPECT_FLOAT_EQ(NC, std::accumulate(spike_sf2.begin(), spike_sf2.end(), 0.0));

    if (use_diag) {
        EXPECT_FALSE(ediag.has_negative);
        EXPECT_FALSE(sdiag1.has_negative);
        EXPECT_FALSE(sdiag2.has_negative);
    }
}

TEST_P(CenterSpikeInFactorsTest, Blocked) {
    const bool use_diag = GetParam();

    std::vector<int> block;
    block.insert(block.end(), 20, 3);
    block.insert(block.end(), 30, 2);
    block.insert(block.end(), 10, 0);
    block.insert(block.end(), 40, 1);

    scran_norm::CenterSpikeInFactorsBlockedOptions opt;
    scran_norm::SizeFactorDiagnostics ediag, sdiag1, sdiag2;
    if (use_diag) {
        opt.endogenous_diagnostics = &ediag;
        opt.spike_in_diagnostics.push_back(&sdiag1);
        opt.spike_in_diagnostics.push_back(&sdiag2);
    }

    auto centers = scran_norm::center_spike_in_factors_blocked(
        NC,
        endog_sf.data(),
        { spike_sf1.data(), spike_sf2.data() },
        block.data(),
        opt 
    );

    EXPECT_FLOAT_EQ(*std::min_element(centers.begin(), centers.end()), 1);

    std::vector<int> bound_starts { 50, 60, 20, 0 };
    std::vector<int> bound_lengths { 10, 40, 30, 20 };

    for (int b = 0; b < 4; ++b) {
        const auto bstart = bound_starts[b], blen = bound_lengths[b];
        const auto scaled = centers[b] * blen;
        EXPECT_FLOAT_EQ(scaled, std::accumulate(endog_sf.begin() + bstart, endog_sf.begin() + bstart + blen, 0.0));
        EXPECT_FLOAT_EQ(scaled, std::accumulate(spike_sf1.begin() + bstart, spike_sf1.begin() + bstart + blen, 0.0));
        EXPECT_FLOAT_EQ(scaled, std::accumulate(spike_sf2.begin() + bstart, spike_sf2.begin() + bstart + blen, 0.0));
    }

    if (use_diag) {
        EXPECT_FALSE(ediag.has_negative);
        EXPECT_FALSE(sdiag1.has_negative);
        EXPECT_FALSE(sdiag2.has_negative);
    }
}

INSTANTIATE_TEST_SUITE_P(
    CenterSpikeInFactors,
    CenterSpikeInFactorsTest,
    ::testing::Values(false, true)
);
