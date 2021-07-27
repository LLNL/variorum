# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# c compiler
set(CMAKE_C_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/g++" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER  "/usr/tce/packages/gcc/gcc-4.9.3/bin/gfortran" CACHE PATH "")

set(USE_MSR_SAFE_BEFORE_1_5_0 ON CACHE BOOL "")

set(BUILD_DOCS ON CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

set(VARIORUM_WITH_AMD OFF CACHE BOOL "")
set(VARIORUM_WITH_ARM OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL ON CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA OFF CACHE BOOL "")

#SPHINX documentation building
set("SPHINX_EXECUTABLE" "/usr/bin/sphinx-build" CACHE PATH "")
