# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# c compiler
set(CMAKE_C_COMPILER "gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "g++" CACHE PATH "")

set(ENABLE_MPI OFF CACHE BOOL "")
set(ENABLE_OPENMP OFF CACHE BOOL "")

set(BUILD_DOCS OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

set(VARIORUM_WITH_AMD_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_AMD_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_ARM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA_GPU ON CACHE BOOL "")

# You need to load CUDA 9.2.* to build this on Lassen
# because hwloc was built with CUDA 9.2.* and not the default CUDA version (10.1.*)
set(CMAKE_SHARED_LINKER_FLAGS "-L/usr/tce/packages/cuda/cuda-10.1.243/nvidia/targets/ppc64le-linux/lib/stubs/ -lnvidia-ml" CACHE PATH "")

# path to global hwloc install
set(HWLOC_DIR "/usr/global/tools/hwloc/blueos_3_ppc64le_ib/hwloc-1.11.10-cuda/" CACHE PATH "")

set(NVML_DIR "/usr/tce/packages/cuda/cuda-10.1.243" CACHE PATH "")
