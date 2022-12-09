# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# Only tested on JLSE login nodes:
#
# oneapi 2022.06.30.002
# apmidg 0.2.0
#
# First, login in to a JLSE login node
# $ module load oneapi apmidg jansson hwloc libiconv cmake
# $ git clone https://github.com/anlsys/variorum.git && cd variorum
# $ mkdir build && cd build
# $ cmake -C ../host-configs/arcticus-apmidg-oneapi.cmake -DCMAKE_INSTALL_PREFIX=__INST_DIR__ ../src
#
# Please contact Kazutomo Yoshii <kazutomo@mcs.anl.gov>
# if you have a question on Intel discrete GPU support.

set(ENABLE_FORTRAN OFF CACHE PATH "")

# build for specific architecture
set(VARIORUM_WITH_AMD_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_AMD_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_ARM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL_CPU OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL_GPU ON CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA_GPU OFF CACHE BOOL "")

set(ENABLE_MPI OFF CACHE BOOL "")
set(ENABLE_OPENMP OFF CACHE BOOL "")
set(ENABLE_FORTRAN OFF CACHE BOOL "")

# path to libraries
set(HWLOC_DIR "/soft/libraries/hwloc/2.3.0/" CACHE PATH "")
set(JANSSON_DIR "/soft/libraries/jansson/4.14.0/" CACHE PATH "")
set(APMIDG_DIR "/soft/tools/apmidg/" CACHE PATH "")
