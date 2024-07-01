#include "gtest/gtest.h"

#include <cmath>
#include <vector>

#include "simulate_vector.h"
#include "compare_almost_equal.h"

#include "log_normalize_counts.hpp"

class LogNormalizeCountsTest : public ::testing::Test {
protected:
    inline static std::vector<double> size_factors;
    inline static std::shared_ptr<tatami::Matrix<double, int> > mat;

    static void SetUpTestSuite() {
        size_factors = simulate_vector(111, /* lower = */ 0.1, /* upper = */ 2, /* seed = */ 42);
        size_t nr = 100;
        tatami::DenseRowMatrix<double, int> dmat(nr, size_factors.size(), simulate_sparse_vector(nr * size_factors.size(), /* lower = */ 1, /* upper = */ 10, /* seed = */ 69));
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

TEST_F(LogNormalizeCountsTest, Basic) {
    scran::log_normalize_counts::Options opt;
    auto lmat = scran::log_normalize_counts::compute(mat, size_factors, opt);

    auto buffer = extract(lmat.get(), 0);
    auto expected = extract(mat.get(), 0);
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] = std::log(expected[i]/size_factors[i] + 1) / std::log(2.0);
    }

    compare_almost_equal(expected, buffer);
}

TEST_F(LogNormalizeCountsTest, PseudoCount) {
    scran::log_normalize_counts::Options opt;
    opt.pseudo_count = 5;
    auto lmat = scran::log_normalize_counts::compute(mat, size_factors, opt);
    EXPECT_FALSE(lmat->is_sparse());

    auto buffer = extract(lmat.get(), 0);
    auto expected = extract(mat.get(), 0);
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] = std::log(expected[i]/size_factors[i] + 5) / std::log(2.0);
    }
    compare_almost_equal(expected, buffer);

    opt.preserve_sparsity = true;
    lmat = scran::log_normalize_counts::compute(mat, size_factors, opt);
    EXPECT_TRUE(lmat->is_sparse());

    buffer = extract(lmat.get(), 0);
    expected = extract(mat.get(), 0);
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] = std::log(expected[i]/(size_factors[i] * 5) + 1) / std::log(2.0);
    }
    compare_almost_equal(expected, buffer);
}

TEST_F(LogNormalizeCountsTest, NewType) {
    size_t nr = 10;
    size_t nc = size_factors.size();
    auto vec = simulate_sparse_vector(nr * nc, /* lower = */ 1, /* upper = */ 10, /* seed = */ 6942);
    for (auto& v : vec) {
        v = std::round(v);
    }

    std::vector<int> ivec(vec.begin(), vec.end());
    std::shared_ptr<tatami::Matrix<int, int> > imat(new tatami::DenseRowMatrix<int, int>(nr, nc, std::move(ivec)));
    scran::log_normalize_counts::Options opt;
    auto lmat = scran::log_normalize_counts::compute<double>(imat, size_factors, opt);

    auto buffer = extract(lmat.get(), 0);
    auto expected_raw = extract(imat.get(), 0);
    std::vector<double> expected(expected_raw.begin(), expected_raw.end());
    for (int i = 0; i < mat->ncol(); ++i) {
        expected[i] = std::log(expected[i]/size_factors[i] + 1) / std::log(2.0);
    }

    compare_almost_equal(expected, buffer);
}
