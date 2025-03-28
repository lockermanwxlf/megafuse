cmake_minimum_required(VERSION 3.24)

set(CXX_STANDARD 23)

set(VCPKG_ROOT "" CACHE PATH "Path to VCPKG")

# MEGA SDK
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/sdk/cmake/modules")
include(sdklib_options)
include(vcpkg_management)
list(APPEND vcpkg_overlay "${CMAKE_CURRENT_LIST_DIR}/sdk/cmake")
process_vcpkg_libraries("${vcpkg_overlay}")

# Project
project(MegaFuse VERSION 0.0.1 LANGUAGES CXX)
add_subdirectory(sdk)
add_subdirectory(src)