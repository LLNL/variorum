# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# c compiler
set(CMAKE_C_COMPILER "/usr/tce/packages/gcc-tce/gcc-10.3.1/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/tce/packages/gcc-tce/gcc-10.3.1/bin/g++" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER "/usr/tce/packages/gcc-tce/gcc-10.3.1/bin/gfortran" CACHE PATH "")

set(ENABLE_MPI OFF CACHE BOOL "")
set(ENABLE_OPENMP OFF CACHE BOOL "")

set(BUILD_DOCS OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

set(VARIORUM_DEBUG OFF CACHE BOOL "")

set(VARIORUM_WITH_AMD_CPU ON CACHE BOOL "")
set(VARIORUM_WITH_AMD_GPU ON CACHE BOOL "")
set(VARIORUM_WITH_ARM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA_GPU OFF CACHE BOOL "")

# path to e_smi_library install                                                    
set(ESMI_DIR "/usr/workspace/msr/tioga_common_setup/esmi_install/e_smi" CACHE PATH "") 

#ROCm setup
set(CMAKE_SHARED_LINKER_FLAGS "-L/opt/rocm-5.2.0/lib -lrocm_smi64" CACHE PATH "")
set(ROCM_DIR "/opt/rocm-5.2.0/" CACHE PATH "")
