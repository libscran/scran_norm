cmake_minimum_required(VERSION 3.14)

project(scran_normalize_counts
    VERSION 1.0.0
    DESCRIPTION "Log-transformed normalized expression values"
    LANGUAGES CXX)

add_library(scran_normalize_counts INTERFACE)
target_compile_features(scran_normalize_counts INTERFACE cxx_std_17)
target_include_directories(scran_normalize_counts INTERFACE include/scran)

add_subdirectory(extern)
target_link_libraries(scran_normalize_counts INTERFACE tatami::tatami tatami::tatami_stats)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()
