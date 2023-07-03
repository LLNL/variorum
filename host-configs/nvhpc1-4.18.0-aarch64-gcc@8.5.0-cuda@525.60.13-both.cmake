# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# c compiler
set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE PATH "")

set(BUILD_DOCS OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

set(VARIORUM_WITH_AMD_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_AMD_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_ARM_CPU ON CACHE BOOL "")
set(VARIORUM_WITH_IBM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA_GPU ON CACHE BOOL "")

set(CMAKE_SHARED_LINKER_FLAGS "-L/usr/local/lib -L/opt/nvidia/hpc_sdk/Linux_aarch64/22.1/cuda/11.5/targets/sbsa-linux/lib/stubs/ -lnvidia-ml" CACHE PATH "")

# path to global hwloc install
set(HWLOC_DIR "/usr/local" CACHE PATH "")

set(NVML_DIR "/opt/nvidia/hpc_sdk/Linux_aarch64/22.1/cuda/11.5/targets/sbsa-linux/" CACHE PATH "")
