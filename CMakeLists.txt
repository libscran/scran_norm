cmake_minimum_required(VERSION 3.14)

project(scran_nrom
    VERSION 0.1.0
    DESCRIPTION "Scaling normalization of single-cell data"
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Library
add_library(scran_norm INTERFACE)
add_library(libscran::scran_norm ALIAS scran_norm)

target_include_directories(scran_norm INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/scran_norm>)
target_compile_features(scran_norm INTERFACE cxx_std_17)

# Dependencies
option(SCRAN_NORM_FETCH_EXTERN "Automatically fetch scran_norm's external dependencies." ON)
if(SCRAN_NORM_FETCH_EXTERN)
    add_subdirectory(extern)
else()
    find_package(tatami_tatami 3.0.0 CONFIG REQUIRED)
    find_package(tatami_tatami_stats 1.0.0 CONFIG REQUIRED)
endif()

target_link_libraries(scran_norm INTERFACE tatami::tatami tatami::tatami_stats)

# Tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(SCRAN_NORM_TESTS "Build scran_norm's test suite." ON)
else()
    option(SCRAN_NORM_TESTS "Build scran_norm's test suite." OFF)
endif()

if(SCRAN_NORM_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif() 
endif()

# Install
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/scran_norm)

install(TARGETS scran_norm
    EXPORT scran_normTargets)

install(EXPORT scran_normTargets
    FILE libscran_scran_normTargets.cmake
    NAMESPACE libscran::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libscran_scran_norm)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/libscran_scran_normConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libscran_scran_norm)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/libscran_scran_normConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libscran_scran_normConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libscran_scran_normConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libscran_scran_norm)
