#ifndef SCRAN_SANITIZE_SIZE_FACTORS_HPP
#define SCRAN_SANITIZE_SIZE_FACTORS_HPP

#include <cmath>
#include <stdexcept>

/**
 * @file sanitize_size_factors.hpp
 * @brief Sanitize invalid size factors.
 */

namespace scran {

/**
 * @namespace scran::sanitize_size_factors
 * @brief Sanitize invalid size factors.
 *
 * Replace zero, missing or infinite values in the size factor array so that it can be used to compute well-defined normalized values.
 * Such size factors can occasionally arise if, e.g., insufficient quality control was performed upstream.
 * Check out the documentation in `Options` to see what placeholder value is used for each type of invalid size factor.
 *
 * In general, sanitization should occur after calls to `center_size_factors::compute()`, `choose_pseudo_count::compute()`, 
 * or any function that computes a statistic based on the distribution of size factors.
 * This ensures that the results of those functions are not affected by the placeholder values used to replace the invalid size factors.
 * As a rule of thumb, `sanitize_size_factors::compute()` should be called just before passing those size factors to `log_normalize_counts::compute()`.
 */
namespace sanitize_size_factors {

/**
 * @brief Validity of size factors.
 */
struct Diagnostics {
    /**
     * Whether negative factors were detected.
     */
    bool has_negative = false;

    /**
     * Whether size factors of zero were detected.
     */
    bool has_zero = false;

    /**
     * Whether NaN size factors were detected.
     */
    bool has_nan = false;

    /**
     * Whether size factors of positive infinity were detected.
     */
    bool has_infinite = false;
};

/**
 * @cond
 */
namespace internal {

template<typename SizeFactor_>
bool is_invalid(SizeFactor_ sf, Diagnostics& output) {
    if (sf < 0) {
        output.has_negative = true;
        return true;
    }

    if (sf == 0) {
        output.has_zero = true;
        return true;
    }

    if (std::isnan(sf)) {
        output.has_nan = true;
        return true;
    }

    if (std::isinf(sf)) {
        output.has_infinite = true;
        return true;
    }

    return false;
}

template<typename SizeFactor_>
SizeFactor_ find_smallest_valid_factor(size_t num, const SizeFactor_* size_factors) {
    SizeFactor_ smallest = 1;
    bool found = false;

    for (size_t i = 0; i < num; ++i) {
        auto s = size_factors[i];
        if (std::isfinite(s) && s > 0) {
            if (!found || smallest > s) {
                smallest = s;
                found = true;
            }
        }
    }

    return smallest;
}

template<typename SizeFactor_>
double find_largest_valid_factor(size_t num, const SizeFactor_* size_factors) {
    SizeFactor_ largest = 1;
    bool found = false;

    for (size_t i = 0; i < num; ++i) {
        auto s = size_factors[i];
        if (std::isfinite(s) && s > 0) {
            if (!found || largest < s) {
                largest = s;
                found = true;
            }
        }
    }

    return largest;
}

}
/**
 * @endcond
 */

/**
 * Check whether there are any invalid size factors.
 * Size factors are only technically valid if they are finite and positive.
 *
 * @tparam SizeFactor_ Floating-point type for the size factors.
 *
 * @param num Number of size factors.
 * @param[in] size_factors Pointer to an array of size factors of length `num`.
 *
 * @return Validation results, indicating whether any zero or non-finite size factors exist.
 */
template<typename SizeFactor_>
Diagnostics check(size_t num, const SizeFactor_* size_factors) {
    Diagnostics output;
    for (size_t i = 0; i < num; ++i) {
        internal::is_invalid(size_factors[i], output);
    }
    return output;
}

/**
 * How invalid size factors should be handled:
 *
 * - `IGNORE`: ignore invalid size factors with no error or change.
 * - `ERROR`: throw an error.
 * - `SANITIZE`: fix each invalid size factor.
 */
enum class HandleAction : char { IGNORE, ERROR, SANITIZE };

/**
 * @brief Default parameters.
 */
struct Options {
    /**
     * How should we handle zero size factors?
     * If `SANITIZE`, they will be automatically set to the smallest valid size factor (or 1, if all size factors are invalid).
     *
     * This approach is motivated by the observation that size factors of zero are typically generated from all-zero cells.
     * By replacing the size factor with a finite value, we ensure that any all-zero cells are represented by all-zero columns in the normalized matrix,
     * which is a reasonable outcome if those cells cannot be filtered out during upstream quality control.
     *
     * We also need to handle cases where a zero size factor may be generated from a cell with non-zero rows, e.g., with `MedianSizeFactors`.
     * By using a "relatively small" replacement value, we ensure that the normalized values reflect the extremity of the scaling.
     */
    HandleAction handle_zero = HandleAction::ERROR;

    /**
     * How should we handle negative size factors?
     * If `SANITIZE`, they will be automatically set to the smallest valid size factor (or 1, if all size factors are invalid).
     * This approach follows the same logic as `set_handle_zero()`, though negative size factors are quite unusual.
     */
    HandleAction handle_negative = HandleAction::ERROR;

    /**
     * How should we handle NaN size factors?
     * If `SANITIZE, NaN size factors will be automatically set to 1, meaning that scaling is a no-op.
     */
    HandleAction handle_nan = HandleAction::ERROR;

    /**
     * How should we handle infinite size factors?
     * If `SANITIZE`, infinite size factors will be automatically set to the largest valid size factor (or 1, if all size factors are invalid).
     * This ensures that any normalized values will be, at least, finite; the choice of a relatively large replacement value reflects the extremity of the scaling.
     */
    HandleAction handle_infinite = HandleAction::ERROR;
};

/**
 * @tparam SizeFactor_ Floating-point type for the size factors.
 *
 * @param num Number of size factors.
 * @param[in,out] size_factors Pointer to an array of positive size factors of length `n`.
 * On output, invalid size factors are replaced.
 * @param status A pre-computed object indicating whether invalid size factors are present in `size_factors`.
 * This can be useful if this information is already provided by, e.g., `check()` or `center_size_factors::compute()`.
 * @param options Further options.
 */
template<typename SizeFactor_>
void compute(size_t num, SizeFactor_* size_factors, const Diagnostics& status, const Options& options) {
    SizeFactor_ smallest = -1;

    if (status.has_negative) {
        if (options.handle_negative == HandleAction::ERROR) {
            throw std::runtime_error("detected negative size factor");
        } else if (options.handle_negative == HandleAction::SANITIZE) {
            smallest = internal::find_smallest_valid_factor(num, size_factors);
            for (size_t i = 0; i < num; ++i) {
                auto& s = size_factors[i];
                if (s < 0) {
                    s = smallest;
                }
            }
        }
    }

    if (status.has_zero) {
        if (options.handle_zero == HandleAction::ERROR) {
            throw std::runtime_error("detected size factor of zero");
        } else if (options.handle_zero == HandleAction::SANITIZE) {
            if (smallest < 0) {
                smallest = internal::find_smallest_valid_factor(num, size_factors);
            }
            for (size_t i = 0; i < num; ++i) {
                auto& s = size_factors[i];
                if (s == 0) {
                    s = smallest;
                }
            }
        }
    }

    if (status.has_nan) {
        if (options.handle_nan == HandleAction::ERROR) {
            throw std::runtime_error("detected NaN size factor");
        } else if (options.handle_nan == HandleAction::SANITIZE) {
            for (size_t i = 0; i < num; ++i) {
                auto& s = size_factors[i];
                if (std::isnan(s)) {
                    s = 1;
                }
            }
        }
    }

    if (status.has_infinite) {
        if (options.handle_infinite == HandleAction::ERROR) {
            throw std::runtime_error("detected infinite size factor");
        } else if (options.handle_infinite == HandleAction::SANITIZE) {
            auto largest = internal::find_largest_valid_factor(num, size_factors);
            for (size_t i = 0; i < num; ++i) {
                auto& s = size_factors[i];
                if (std::isinf(s)) {
                    s = largest;
                }
            }
        }
    }
}

/**
 * @tparam SizeFactor_ Floating-point type for the size factors.
 *
 * @param num Number of size factors.
 * @param[in,out] size_factors Pointer to an array of positive size factors of length `n`.
 * On output, invalid size factors are replaced.
 * @param options Further options.
 *
 * @return An object whether each type of invalid size factors is present in `size_factors`.
 */
template<typename SizeFactor_>
Diagnostics compute(size_t num, SizeFactor_* size_factors, const Options& options) {
    auto output = check(num, size_factors);
    compute(num, size_factors, output, options);
    return output;
}

}

}

#endif
