# Normalize scaling biases in count data

![Unit tests](https://github.com/libscran/normalize_counts/actions/workflows/run-tests.yaml/badge.svg)
![Documentation](https://github.com/libscran/normalize_counts/actions/workflows/doxygenate.yaml/badge.svg)
[![Codecov](https://codecov.io/gh/libscran/normalize_counts/graph/badge.svg?token=JWV0I4WJX2)](https://codecov.io/gh/libscran/normalize_counts)

## Overview

This repository contains functions to perform scaling normalization and log-transformation of a gene-by-cell count matrix.
Normalization removes per-cell scaling biases such as differences in capture efficiency and sequencing depth,
while the log-transformation provides some variance stabilization and allows differences in values to be interpreted as log-fold changes. 
The code itself was originally derived from the [**scran** R package](https://bioconductor.org/packages/scran),
factored out into a separate C++ library for easier re-use.

## Quick start

Given a measure of the per-cell scaling bias - for example, the sum of counts for a cell -
we can convert them into centered size factors:

```cpp
// Assuming that counts is a std::shared_ptr<tatami::Matrix>
std::vector<double> bias = tatami_stats::sums::by_column(counts.get()); 

scran::center_size_factors::Options copt;
scran::center_size_factors::compute(bias.size(), bias.data(), NULL, copt);

// 'bias' is now centered at unity and can be used as size factors.
auto& size_factors = bias;
```

Alternatively, in the presence of blocks, we adjust our centering so that the mean size factor in each block is no less than 1.
This avoids inflated variances from applying small size factors to low-coverage blocks.

```cpp
scran::center_size_factors::compute_blocked(
    bias.size(), 
    bias.data(), 
    block.data(), 
    NULL, 
    copt
);
```

If our size factors might contain invalid values (i.e., zero, negative, or non-finite),
we can sanitize them prior to the construction of the log-normalized matrix:

```cpp
scran::sanitize_size_factors::Options sopt;
opt.handle_zero = scran::sanitize_size_factors::HandleAction::SANITIZE;
opt.handle_infinite = scran::sanitize_size_factors::HandleAction::SANITIZE;
scran::sanitize_size_factors::compute(size_factors.size(), size_factors.data(), sopt);
// Gets rid of any zero and infinite values in 'size_factors'.
```

Finally, we convert our a [`tatami::Matrix`](https://github.com/tatami-inc/tatami) of counts to a log-transformed normalized matrix:

```cpp
scran::normalize_counts::Options lopts;
auto logcounts = scran::normalize_counts::compute(counts, size_factors, lopt);
```

Check out the [reference documentation](https://libscran.github.io/normalize_counts) for more details.

## Building projects

This repository is part of the broader [**libscran**](https://github.com/libscran/libscran) library,
so users are recommended to use the latter in their projects.
**libscran** developers should just use CMake with `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
  scran_normalize_counts 
  GIT_REPOSITORY https://github.com/libscran/normalize_counts
  GIT_TAG master # or any version of interest
)

FetchContent_MakeAvailable(scran_normalize_counts)

# For executables:
target_link_libraries(myexe scran_normalize_counts)

# For libaries
target_link_libraries(mylib INTERFACE scran_normalize_counts)
```
