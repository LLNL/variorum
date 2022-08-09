..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##############################
 Intel Discrete GPUs Overview
##############################

This page provides a detailed description of the the Intel DGPU port of Variorum.
The functionality of this port depends on Intel-specific proprietary software
stack as well as open-source software components described below. The high-level
API provided by Variorum is read-only (i.e., monitoring-only), primarily because
of the access limitations on our target platform.

**************
 Requirements
**************

The Intel DGPU port of Variorum depends on:

-  APMIDG

-  One API

- Other knobs/permissions 

We have tested our port with ... 

*********************
 Build Configuration
*********************

We provide an example CMake host config file, which defines the CMake build
variables set on our test platform (Aurora early access supercomputer at ANL):
`lassen-4.14.0-ppc64le-gcc@4.9.3-cuda@10.1.243.cmake`. (please edit)

For your build system, you will need to enable Variorum to build with ...  and
set two path variables as described below:

   -  ``VARIORUM_WITH_NVIDIA_GPU=ON``
   -  ``CMAKE_SHARED_LINKER_FLAGS``: Path to libnvidia-ml.so (prefixed with the
      '-L' flag)
   -  ``HWLOC_DIR``: Path for the CUDA-aware version of libhwloc

************
 References
************

-  Please add reference 
