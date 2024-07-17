#include "gtest/gtest.h"

#include <cmath>
#include <vector>

#include "scran_tests/scran_tests.hpp"

#include "scran_norm/normalize_counts.hpp"

class NormalizeCountsTest : public ::testing::Test {
protected:
    inline static std::vector<double> size_factors;
    inline static std::shared_ptr<tatami::Matrix<double, int> > mat;

    static void SetUpTestSuite() {
        size_factors = scran_tests::simulate_vector(111, []{
            scran_tests::SimulationParameters sparams;
            sparams.lower = 0.1;
            sparams.upper = 2;
            sparams.seed = 42;
            return sparams;
        }());

        size_t nr = 100;
        auto vec = scran_tests::simulate_vector(nr * size_factors.size(), []{
            scran_tests::SimulationParameters sparams;
            sparams.density = 0.3;
            sparams.lower = 1;
            sparams.upper = 10;
            sparams.seed = 69;
            return sparams;
        }());

        tatami::DenseRowMatrix<double, int> dmat(nr, size_factors.size(), std::move(vec));
        mat = tatami::convert_to_compressed_sparse(&dmat, true);
    }

    template<typename Value_, typename Index_>
    static std::vector<Value_> extract(const tatami::Matrix<Value_, Index_>* x, int i) {
        auto ext = x->dense_row();
        std::vector<Value_> buffer(x->ncol());
        auto ptr = ext->fetch(i, buffer.data());
        tatami::copy_n(ptr, x->ncol(), buffer.data());
        return buffer;
    }
};

TEST_F(NormalizeCountsTest, Basic) {
    scran_norm::NormalizeCountsOptions opt;
    auto lmat = scran_norm::normalize_counts(mat, size_factors, opt);

    auto buffer = extract(lmat.get(), 0);
    auto expected = extract(mat.get(), 0);
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] = std::log(expected[i]/size_factors[i] + 1) / std::log(2.0);
    }
    scran_tests::compare_almost_equal(expected, buffer);

    // Now without the log.
    opt.log = false;
    auto nmat = scran_norm::normalize_counts(mat, size_factors, opt);

    buffer = extract(nmat.get(), 0);
    expected = extract(mat.get(), 0);
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] /= size_factors[i];
    }
    scran_tests::compare_almost_equal(expected, buffer);
}

TEST_F(NormalizeCountsTest, PseudoCount) {
    scran_norm::NormalizeCountsOptions opt;
    opt.pseudo_count = 5;
    auto lmat = scran_norm::normalize_counts(mat, size_factors, opt);
    EXPECT_FALSE(lmat->is_sparse());

    auto buffer = extract(lmat.get(), 0);
    auto expected = extract(mat.get(), 0);
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] = std::log(expected[i]/size_factors[i] + 5) / std::log(2.0);
    }
    scran_tests::compare_almost_equal(expected, buffer);

    opt.preserve_sparsity = true;
    lmat = scran_norm::normalize_counts(mat, size_factors, opt);
    EXPECT_TRUE(lmat->is_sparse());

    buffer = extract(lmat.get(), 0);
    expected = extract(mat.get(), 0);
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] = std::log(expected[i]/(size_factors[i] * 5) + 1) / std::log(2.0);
    }
    scran_tests::compare_almost_equal(expected, buffer);

    // Pseudo-count considerations ignored if log = false.
    opt.log = false;
    auto nmat = scran_norm::normalize_counts(mat, size_factors, opt);

    buffer = extract(nmat.get(), 0);
    expected = extract(mat.get(), 0);
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] /= size_factors[i];
    }
    scran_tests::compare_almost_equal(expected, buffer);
}

TEST_F(NormalizeCountsTest, NewType) {
    size_t nr = 10;
    size_t nc = size_factors.size();

    auto vec = scran_tests::simulate_vector(nr * nc, []{
        scran_tests::SimulationParameters sparams;
        sparams.density = 0.3;
        sparams.lower = 1;
        sparams.upper = 10;
        sparams.seed = 6942;
        return sparams;
    }());
    for (auto& v : vec) {
        v = std::round(v);
    }

    std::vector<int> ivec(vec.begin(), vec.end());
    std::shared_ptr<tatami::Matrix<int, int> > imat(new tatami::DenseRowMatrix<int, int>(nr, nc, std::move(ivec)));
    scran_norm::NormalizeCountsOptions opt;
    auto lmat = scran_norm::normalize_counts<double>(imat, size_factors, opt);

    auto buffer = extract(lmat.get(), 0);
    auto expected_raw = extract(imat.get(), 0);
    std::vector<double> expected(expected_raw.begin(), expected_raw.end());
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] = std::log(expected[i]/size_factors[i] + 1) / std::log(2.0);
    }

    scran_tests::compare_almost_equal(expected, buffer);
}
