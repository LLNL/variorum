# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# c compiler
set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE PATH "")

set(ENABLE_MPI OFF CACHE BOOL "")
set(ENABLE_OPENMP OFF CACHE BOOL "")
set(ENABLE_FORTRAN ON CACHE PATH "")
set(CMAKE_Fortran_COMPILER  "/usr/bin/gfortran" CACHE PATH "")
set(ENABLE_LIBJUSTIFY ON CACHE BOOL "")

set(VARIORUM_WITH_AMD_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_AMD_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_ARM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL_CPU ON CACHE BOOL "")
set(VARIORUM_WITH_INTEL_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA_GPU OFF CACHE BOOL "")

set(USE_MSR_SAFE_BEFORE_1_5_0 OFF CACHE BOOL "")

set(VARIORUM_DEBUG OFF CACHE BOOL "")

set(BUILD_DOCS OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

# path to hwloc install
set(HWLOC_DIR "/usr/include/hwloc/" CACHE PATH "")

# Path to JANSSON build
set(JANSSON_DIR "/usr/include/" CACHE PATH "")

# Path to libjustify install
set(LIBJUSTIFY_DIR "/usr/local/share/libjustify-install/" CACHE PATH "")
