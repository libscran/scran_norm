# Scaling normalization of single-cell count data

![Unit tests](https://github.com/libscran/scran_norm/actions/workflows/run-tests.yaml/badge.svg)
![Documentation](https://github.com/libscran/scran_norm/actions/workflows/doxygenate.yaml/badge.svg)
[![Codecov](https://codecov.io/gh/libscran/scran_norm/graph/badge.svg?token=JWV0I4WJX2)](https://codecov.io/gh/libscran/scran_norm)

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
#include "scran_norm/scran_norm.hpp"

// Assuming that counts is a std::shared_ptr<tatami::Matrix>
std::vector<double> bias = tatami_stats::sums::by_column(counts.get()); 

scran_norm::CenterSizeFactorsOptions copt;
scran_norm::center_size_factors(bias.size(), bias.data(), NULL, copt);

// 'bias' is now centered at unity and can be used as size factors.
auto& size_factors = bias;
```

Alternatively, in the presence of blocks, we adjust our centering so that the mean size factor in each block is no less than 1.
This avoids inflated variances from applying small size factors to low-coverage blocks.

```cpp
scran_norm::center_size_factors_blocked(
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
scran_norm::SanitizeSizeFactorsOptions sopt;
sopt.handle_zero = scran_norm::SanitizeAction::SANITIZE;
sopt.handle_infinite = scran_norm::SanitizeAction::SANITIZE;
scran_norm::sanitize_size_factors(size_factors.size(), size_factors.data(), sopt);
// Gets rid of any zero and infinite values in 'size_factors'.
```

Finally, we convert our [`tatami::Matrix`](https://github.com/tatami-inc/tatami) of counts into a log-transformed normalized matrix:

```cpp
scran_norm::NormalizeCountsOptions lopts;
auto logcounts = scran_norm::normalize_counts(counts, size_factors, lopt);
```

Check out the [reference documentation](https://libscran.github.io/scran_norm) for more details.

## Building projects

### CMake with `FetchContent`

If you're using CMake, you just need to add something like this to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  scran_norm
  GIT_REPOSITORY https://github.com/libscran/scran_norm
  GIT_TAG master # or any version of interest
)

FetchContent_MakeAvailable(scran_norm)
```

Then you can link to **scran_norm** to make the headers available during compilation:

```cmake
# For executables:
target_link_libraries(myexe libscran::scran_norm)

# For libaries
target_link_libraries(mylib INTERFACE libscran::scran_norm)
```

### CMake with `find_package()`

```cmake
find_package(libscran_scran_norm CONFIG REQUIRED)
target_link_libraries(mylib INTERFACE libscran::scran_norm)
```

To install the library, use:

```sh
mkdir build && cd build
cmake .. -DSCRAN_NORM_TESTS=OFF
cmake --build . --target install
```

By default, this will use `FetchContent` to fetch all external dependencies.
If you want to install them manually, use `-DSCRAN_NORM_FETCH_EXTERN=OFF`.
See the tags in [`extern/CMakeLists.txt`](extern/CMakeLists.txt) to find compatible versions of each dependency.

### Manual

If you're not using CMake, the simple approach is to just copy the files in `include/` - either directly or with Git submodules - and include their path during compilation with, e.g., GCC's `-I`.
This requires the external dependencies listed in [`extern/CMakeLists.txt`](extern/CMakeLists.txt), which also need to be made available during compilation.
