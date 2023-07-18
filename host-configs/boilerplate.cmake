# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

##################################
# empty  host-config
##################################
# insert compiler name here
##################################

# c compiler
set(CMAKE_C_COMPILER "/path/to/c_compiler" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/path/to/cxx_compiler" CACHE PATH "")

# build fortran example
set(ENABLE_FORTRAN ON CACHE PATH "")
set(CMAKE_Fortran_COMPILER  "/path/to/fortran_compiler" CACHE PATH "")

# build for specific architecture
set(VARIORUM_WITH_AMD_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_AMD_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_ARM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL_CPU ON CACHE BOOL "")
set(VARIORUM_WITH_INTEL_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA_GPU OFF CACHE BOOL "")

set(VARIORUM_DEBUG OFF CACHE BOOL "")

set(ENABLE_MPI ON CACHE BOOL "")
set(ENABLE_OPENMP ON CACHE BOOL "")

# build documentation (depends on sphinx, doxygen, and breathe)
set(BUILD_DOCS ON CACHE BOOL "")

# build unit tests
set(BUILD_TESTS ON CACHE BOOL "")

# SPHINX documentation building
set("SPHINX_EXECUTABLE" "/path/to/sphinx-build" CACHE PATH "")

# path to global hwloc install
set(HWLOC_DIR "/path/to/hwloc-build" CACHE PATH "")

# path to global jansson install
set(JANSSON_DIR "/path/to/jansson-build" CACHE PATH "")
