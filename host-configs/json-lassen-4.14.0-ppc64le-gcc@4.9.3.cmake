# Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# c compiler
set(CMAKE_C_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/g++" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER  "/usr/tce/packages/gcc/gcc-4.9.3/bin/gfortran" CACHE PATH "")

set(BUILD_DOCS ON CACHE BOOL "")
set(BUILD_TESTS ON CACHE BOOL "")

set(VARIORUM_WITH_AMD OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM ON CACHE BOOL "")
set(VARIORUM_WITH_INTEL OFF CACHE BOOL "")

set("JANSSON_DIR" "/g/g90/patki1/src/spack/opt/spack/linux-rhel7-x86_64/gcc-4.9.3/jansson-2.9-riikbrzlsoam4d4f3z3qwtteno6vpxd6" CACHE_PATH "")

#SPHINX documentation building
set("SPHINX_EXECUTABLE" "/collab/usr/gapps/python/build/spack-coralea/opt/spack/linux-rhel7-ppc64le/gcc-4.8.5/py-sphinx-1.6.1-txjs2mkq6eoq6eteawczvpnnfopaychw/bin/sphinx-build" CACHE PATH "")
