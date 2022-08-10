..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##############################
 Intel Discrete GPUs Overview
##############################

This page provides a detailed description of the the Intel Discrete
GPU port of Variorum.  The functionality of this port depends on
Intel-specific proprietary software stack as well as open-source
software components described below. The high-level API provided by
Variorum is currently read-only (i.e., monitoring-only), primarily
because of the access limitations on our target platform.

**************
 Requirements
**************

The Intel Discrete GPU port of Variorum depends on:

-  APMIDG 0.3.0 or later

-  One API 2022.06.30.002 or later

We have tested our port with Aurora early access systems with Intel
ATS GPUs.

Note: at this point, monitoring power of Intel Discrete GPU requires
no additional permission. However, the permission default setting may
change in a future driver release. Please consult your sysadmin.


*********************
 Build Configuration
*********************

We provide an example CMake host config file, which defines the CMake build
variables set on our test platform (Aurora early access supercomputer at ANL):
`arcticus-apmidg-oneapi.cmake`.

For your build system, you will need to enable Variorum to build with INTEL_GPU set two path variables as described below:

   -  ``VARIORUM_WITH_INTEL_GPU=ON``
   -  ``APMDIG_DIR``: Path to libapmidg.so (prefixed with the
      '-L' flag)


************
 References
************

-  `APMDIG library <https://github.com/anlsys/apmidg>`_
