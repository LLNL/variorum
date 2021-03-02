# Copyright 2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# c compiler
set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE PATH "")

# fortran compiler
set(ENABLE_FORTRAN OFF CACHE BOOL "")

set(USE_MSR_SAFE_BEFORE_1_5_0 ON CACHE BOOL "")

set(BUILD_DOCS OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

set(VARIORUM_WITH_AMD OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL OFF CACHE BOOL "")
set(VARIORUM_WITH_ARM ON CACHE BOOL "")

set(VARIORUM_LOG OFF CACHE BOOL "")

# path to global hwloc install
set(HWLOC_DIR "/usr/local/share/hwloc-1.11.7-install/" CACHE PATH "")

# Path to JANSSON build
set(JANSSON_DIR "/usr/local/" CACHE PATH "")
