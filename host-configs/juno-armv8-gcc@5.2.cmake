# Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# c compiler
set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE PATH "")

# fortran compiler
# set(CMAKE_Fortran_COMPILER  "/usr/bin/gfortran" CACHE PATH "")

set(USE_MSR_SAFE_BEFORE_1_5_0 ON CACHE BOOL "")

set(BUILD_DOCS ON CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

set(VARIORUM_WITH_AMD OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL OFF CACHE BOOL "")
set(VARIORUM_WITH_ARM ON CACHE BOOL "")

set(ENABLE_FORTRAN OFF CACHE BOOL "")

# path to global hwloc install
set(HWLOC_DIR "/usr/local/share/hwloc-1.11.7-install/" CACHE PATH "")

# Path to JANSSON build
set(JANSSON_DIR "/usr/local/" CACHE PATH "")

#SPHINX documentation building
#set("SPHINX_EXECUTABLE" "/collab/usr/gapps/python/build/spack-toss3.2/opt/spack/linux-rhel7-x86_64/gcc-4.9.3/py-sphinx-1.6.3-pd3yayuuejdpggkyqvhxbljwh4fhpo43/bin/sphinx-build" CACHE PATH "")
