#include "gtest/gtest.h"

#include <cmath>
#include <vector>

#include "scran_tests/scran_tests.hpp"
#include "tatami_test/tatami_test.hpp"

#include "scran_norm/normalize_counts.hpp"

typedef std::tuple<double, double> HelperSimulationOptions;

class DelayedLogNormalizeHelperTest : public ::testing::TestWithParam<std::tuple<HelperSimulationOptions, tatami_test::StandardTestAccessOptions> > {
protected:
    static constexpr int NR = 100, NC = 250;
    inline static std::shared_ptr<tatami::NumericMatrix> dense_mod, sparse_mod, ref;

    inline static HelperSimulationOptions last_params;

    static void assemble(HelperSimulationOptions sim_params) {
        if (sim_params == last_params) {
            return;
        }
        last_params = sim_params;

        auto log_base = std::get<0>(sim_params);
        auto pseudo_count = std::get<1>(sim_params);

        auto size_factors = scran_tests::simulate_vector(NC, [&]{
            scran_tests::SimulationParameters<> sparams;
            sparams.lower = 0.1;
            sparams.upper = 2;
            sparams.seed = 96969 * log_base + pseudo_count;
            return sparams;
        }());

        auto vec = scran_tests::simulate_vector(NR * NC, [&]{
            scran_tests::SimulationParameters<> sparams;
            sparams.density = 0.3;
            sparams.lower = 1;
            sparams.upper = 10;
            sparams.seed = 24242 * log_base + pseudo_count;
            return sparams;
        }());

        auto dense = std::make_shared<tatami::DenseMatrix<double, int, decltype(vec)> >(NR, NC, vec, true); // row major
        auto sparse = tatami::convert_to_compressed_sparse<double, int>(*dense, false, {}); // column major.

        auto op = std::make_shared<scran_norm::DelayedLogNormalizeHelper<double, double, int, decltype(size_factors)> >(size_factors, log_base, pseudo_count);
        dense_mod = std::make_shared<tatami::DelayedUnaryIsometricOperation<double, double, int> >(std::move(dense), op);
        sparse_mod = std::make_shared<tatami::DelayedUnaryIsometricOperation<double, double, int> >(std::move(sparse), op);

        for (int r = 0; r < NR; ++r) {
            for (int c = 0; c < NC; ++c) {
                const auto offset = sanisizer::nd_offset<std::size_t>(c, NC, r);
                if (pseudo_count == 1) {
                    vec[offset] = std::log1p(vec[offset] / size_factors[c]) / std::log(log_base);
                } else {
                    vec[offset] = std::log(vec[offset] / size_factors[c] + pseudo_count) / std::log(log_base);
                }
            }
        }
        ref.reset(new tatami::DenseMatrix<double, int, decltype(vec)>(NR, NC, std::move(vec), true));
    }

    void SetUp() {
        assemble(std::get<0>(GetParam()));
    }
};

TEST_P(DelayedLogNormalizeHelperTest, Full) {
    auto opts = tatami_test::convert_test_access_options(std::get<1>(GetParam()));
    tatami_test::test_full_access<double, int>(*dense_mod, *ref, opts);
    tatami_test::test_full_access<double, int>(*sparse_mod, *ref, opts);

    EXPECT_FALSE(dense_mod->is_sparse());
    if (std::get<1>(last_params) == 1) {
        EXPECT_TRUE(sparse_mod->is_sparse());
    } else {
        EXPECT_FALSE(sparse_mod->is_sparse());
    }
}

TEST_P(DelayedLogNormalizeHelperTest, Block) {
    auto opts = tatami_test::convert_test_access_options(std::get<1>(GetParam()));
    tatami_test::test_block_access<double, int>(*dense_mod, *ref, 0.2, 0.6, opts); \
    tatami_test::test_block_access<double, int>(*dense_mod, *ref, 0.1, 0.4, opts); \
    tatami_test::test_block_access<double, int>(*sparse_mod, *ref, 0.25, 0.5, opts); \
    tatami_test::test_block_access<double, int>(*sparse_mod, *ref, 0.6, 0.3, opts); \
}

TEST_P(DelayedLogNormalizeHelperTest, Indexed) {
    auto opts = tatami_test::convert_test_access_options(std::get<1>(GetParam()));
    tatami_test::test_indexed_access<double, int>(*dense_mod, *ref, 0.2, 0.15, opts); \
    tatami_test::test_indexed_access<double, int>(*dense_mod, *ref, 0.1, 0.1, opts); \
    tatami_test::test_indexed_access<double, int>(*sparse_mod, *ref, 0.3, 0.2, opts); \
    tatami_test::test_indexed_access<double, int>(*sparse_mod, *ref, 0.5, 0.3, opts); \
}

INSTANTIATE_TEST_SUITE_P(
    DelayedLogNormalizeHelper,
    DelayedLogNormalizeHelperTest,
    ::testing::Combine(
        ::testing::Combine(
            ::testing::Values(2, 10),
            ::testing::Values(1, 3)
        ),
        tatami_test::standard_test_access_options_combinations() 
    )
);

/********************************************/

TEST(DelayedLogNormalizeHelper, AnyZero) {
    const int NR = 150, NC = 80;

    auto size_factors = scran_tests::simulate_vector(NC, [&]{
        scran_tests::SimulationParameters<> sparams;
        sparams.lower = 0.1;
        sparams.upper = 2;
        sparams.seed = 96;
        return sparams;
    }());
    size_factors.front() = 0;
    size_factors.back() = 0;

    auto vec = scran_tests::simulate_vector(NR * NC, [&]{
        scran_tests::SimulationParameters<> sparams;
        sparams.density = 0.3;
        sparams.lower = 1;
        sparams.upper = 10;
        sparams.seed = 24;
        return sparams;
    }());

    auto dense = std::make_shared<tatami::DenseMatrix<double, int, decltype(vec)> >(NR, NC, vec, true); // row major
    auto sparse = tatami::convert_to_compressed_sparse<double, int>(*dense, false, {}); // column major.

    auto op = std::make_shared<scran_norm::DelayedLogNormalizeHelper<double, double, int, decltype(size_factors)> >(size_factors, 2.0, 1.0);
    auto dense_mod = std::make_shared<tatami::DelayedUnaryIsometricOperation<double, double, int> >(std::move(dense), op);
    EXPECT_FALSE(dense_mod->is_sparse());
    auto sparse_mod = std::make_shared<tatami::DelayedUnaryIsometricOperation<double, double, int> >(std::move(sparse), op);
    EXPECT_FALSE(sparse_mod->is_sparse());

    for (int r = 0; r < NR; ++r) {
        for (int c = 0; c < NC; ++c) {
            const auto offset = sanisizer::nd_offset<std::size_t>(c, NC, r);
            vec[offset] = std::log1p(vec[offset] / size_factors[c]) / std::log(2.0);
        }
    }
    tatami::DenseMatrix<double, int, decltype(vec)> ref(NR, NC, std::move(vec), true);

    tatami_test::test_simple_column_access<double, int>(*dense_mod, ref);
    tatami_test::test_simple_column_access<double, int>(*sparse_mod, ref); 

    tatami_test::test_simple_row_access<double, int>(*dense_mod, ref);
    tatami_test::test_simple_row_access<double, int>(*sparse_mod, ref); 
};

TEST(DelayedLogNormalizeHelper, OtherTypes) {
    const int NR = 150, NC = 80;

    auto size_factors = scran_tests::simulate_vector<float>(NC, [&]{
        scran_tests::SimulationParameters<float> sparams;
        sparams.lower = 0.1;
        sparams.upper = 2;
        sparams.seed = 96;
        return sparams;
    }());

    auto vec = scran_tests::simulate_vector<int>(NR * NC, [&]{
        scran_tests::SimulationParameters<int> sparams;
        sparams.density = 0.3;
        sparams.lower = 1;
        sparams.upper = 10;
        sparams.seed = 24;
        return sparams;
    }());

    auto dense = std::make_shared<tatami::DenseMatrix<int, int, decltype(vec)> >(NR, NC, vec, true); // row major
    auto sparse = tatami::convert_to_compressed_sparse<int, int>(*dense, false, {}); // column major.

    auto op = std::make_shared<scran_norm::DelayedLogNormalizeHelper<double, int, int, decltype(size_factors)> >(size_factors, 2.0, 1.0);
    auto dense_mod = std::make_shared<tatami::DelayedUnaryIsometricOperation<double, int, int> >(std::move(dense), op);
    auto sparse_mod = std::make_shared<tatami::DelayedUnaryIsometricOperation<double, int, int> >(std::move(sparse), op);
    EXPECT_FALSE(dense_mod->is_sparse());
    EXPECT_TRUE(sparse_mod->is_sparse());

    std::vector<double> refvec(vec.size());
    for (int r = 0; r < NR; ++r) {
        for (int c = 0; c < NC; ++c) {
            const auto offset = sanisizer::nd_offset<std::size_t>(c, NC, r);
            refvec[offset] = std::log1p(vec[offset] / size_factors[c]) / std::log(2.0);
        }
    }
    tatami::DenseMatrix<double, int, decltype(refvec)> ref(NR, NC, std::move(refvec), true);

    tatami_test::test_simple_column_access<double, int>(*dense_mod, ref);
    tatami_test::test_simple_column_access<double, int>(*sparse_mod, ref); 

    tatami_test::test_simple_row_access<double, int>(*dense_mod, ref);
    tatami_test::test_simple_row_access<double, int>(*sparse_mod, ref); 
};

/********************************************/

class NormalizeCountsTest : public ::testing::Test {
protected:
    inline static std::vector<double> size_factors;
    inline static std::shared_ptr<tatami::Matrix<double, int> > mat;

    static void SetUpTestSuite() {
        size_factors = scran_tests::simulate_vector(111, []{
            scran_tests::SimulationParameters<> sparams;
            sparams.lower = 0.1;
            sparams.upper = 2;
            sparams.seed = 42;
            return sparams;
        }());

        size_t nr = 100;
        auto vec = scran_tests::simulate_vector(nr * size_factors.size(), []{
            scran_tests::SimulationParameters<> sparams;
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
        scran_tests::SimulationParameters<> sparams;
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

TEST_F(NormalizeCountsTest, WrongLength) {
    scran_norm::NormalizeCountsOptions opt;
    scran_tests::expect_error([&]() { scran_norm::normalize_counts(mat, std::vector<double>(), opt); }, "equal to the number of columns");
}
