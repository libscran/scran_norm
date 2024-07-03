#ifndef SCRAN_NORMALIZE_COUNTS_HPP
#define SCRAN_NORMALIZE_COUNTS_HPP

#include <type_traits>
#include <vector>
#include <memory>

#include "tatami/tatami.hpp"

/**
 * @namespace normalize_counts.hpp
 * @brief Normalize and log-transform counts.
 */

namespace scran {

/**
 * @namespace scran::normalize_counts
 * @brief Normalize and log-transform counts.
 *
 * Given a count matrix and a set of size factors, compute log2-transformed normalized expression values.
 * Each cell's counts are divided by the cell's size factor, to account for differences in capture efficiency and sequencing depth across cells.
 * The normalized values are then log-transformed so that downstream analyses focus on the relative rather than absolute differences in expression;
 * this process also provides some measure of variance stabilization.
 * These operations are done in a delayed manner using the `tatami::DelayedUnaryIsometricOperation` class.
 *
 * The simplest and most common method for defining size factors is to use the centered library sizes, see `center_size_factors` for details.
 * This removes scaling biases caused by sequencing depth, etc. between cells,
 * while the centering preserves the scale of the counts in the (log-)normalized expression values.
 * That said, users can define size factors from any method of their choice (e.g., median-based normalization, TMM) as long as they are positive for all cells.
 */
namespace normalize_counts {

/**
 * @brief Options for `compute()`.
 */
struct Options {
    /**
     * Pseudo-count to add to each value prior to log-transformation.
     * All values should be positive to ensure that log-transformed values are finite.
     * The default value of 1 preserves sparsity in the log-count matrix.
     * Larger values shrink the differences between cells towards zero, reducing variance at the cost of increasing bias.
     * Ignored if `log = false`.
     */
    double pseudo_count = 1;

    /**
     * Whether to preserve sparsity for non-unity pseudo-counts.
     * If true, we multiply the size factors by the `pseudo_count` and add 1 before log-transformation.
     * This does not change the differences between entries of the resulting matrix,
     * and adding `log2(pseudo_count)` will recover the expected log-count values.
     * Ignored if `log = false`.
     */
    bool preserve_sparsity = false;

    /**
     * Whether to perform the log-transformation.
     */
    bool log = true;
};

/**
 * @tparam OutputValue_ Floating-point type for the output matrix.
 * @tparam InputValue_ Data type for the input matrix.
 * @tparam InputIndex_ Integer type for the input matrix.
 * @tparam SizeFactors_ Container of floats for the size factors.
 * This should have the `size()`, `begin()`, `end()` and `operator[]` methods.
 *
 * @param counts Pointer to a `tatami::Matrix` of gene-by-cell counts.
 * @param size_factors Vector of length equal to the number of columns in `counts`, containing the size factor for each cell.
 * All values should be positive. 
 * @param options Further options.
 *
 * @return Matrix of normalized expression values.
 * These are log-transformed if `Options::log = true`.
 */
template<typename OutputValue_ = double, typename InputValue_ = double, typename Index_ = int, class SizeFactors_ = std::vector<double> >
std::shared_ptr<tatami::Matrix<OutputValue_, Index_> > compute(std::shared_ptr<const tatami::Matrix<InputValue_, Index_> > counts, SizeFactors_ size_factors, const Options& options) {
    auto current_pseudo = options.pseudo_count;
    if (options.preserve_sparsity && current_pseudo != 1 && options.log) {
        for (auto& x : size_factors) { 
            x *= current_pseudo;
        }
        current_pseudo = 1;
    }

    static_assert(std::is_floating_point<OutputValue_>::value);
    auto div = tatami::make_DelayedUnaryIsometricOperation<OutputValue_>(
        std::move(counts), 
        tatami::make_DelayedUnaryIsometricDivideVector<true, InputValue_, SizeFactors_>(
            std::move(size_factors), 
            false
        )
    );

    if (!options.log) {
        return div;
    }

    if (current_pseudo == 1) {
        return tatami::make_DelayedUnaryIsometricOperation<OutputValue_>(
            std::move(div), 
            tatami::DelayedUnaryIsometricLog1p<OutputValue_>(2.0)
        );
    } else {
        auto add = tatami::make_DelayedUnaryIsometricOperation<OutputValue_>(
            std::move(div), 
            tatami::make_DelayedUnaryIsometricAddScalar<OutputValue_>(current_pseudo)
        );
        return tatami::make_DelayedUnaryIsometricOperation<OutputValue_>(
            std::move(add), 
            tatami::DelayedUnaryIsometricLog<OutputValue_>(2.0)
        );
    }
};

/**
 * @cond
 */
template<typename OutputValue_ = double, typename InputValue_ = double, typename Index_ = int, class SizeFactors_ = std::vector<double> >
std::shared_ptr<tatami::Matrix<OutputValue_, Index_> > compute(std::shared_ptr<tatami::Matrix<InputValue_, Index_> > counts, SizeFactors_ size_factors, const Options& options) {
    return compute(std::shared_ptr<const tatami::Matrix<InputValue_, Index_> >(std::move(counts)), std::move(size_factors), options);
}
/**
 * @cond
 */

}

}

#endif
