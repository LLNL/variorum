.. # Copyright 2019 Lawrence Livermore National Security, LLC and other
.. # Variorum Project Developers. See the top-level LICENSE file for details.
.. #
.. # SPDX-License-Identifier: MIT


Variorum Developer Documentation
================================

Here is some information on how to extend Variorum to support additional
platforms and/or architectures.

A **platform** refers to a hardware vendor, for example, Intel, IBM, or ARM. An
**architecture** refers to a generation of hardware within a platform, for
example, Broadwell or Ivy Bridge for Intel Xeon processors; or Power8 and Power9 for IBM
Power processors.


Steps to Add Support for a New Platform (pfm)
---------------------------------------------

1. Create a new folder under `src/variorum/pfm`
2. Create a `config_pfm.h` and `config_pfm.c`. These two files implement the `detect_pfm_arch` and `set_pfm_func_ptrs` functions. The former identifies the new platform, and the latter sets function pointers for get/set power, thermals, etc. on the new platform. These function pointers refer to internal functions that are defined in architecture-specific files (see next section).
3. Add a struct listing all the architectures implemented on the new platform in `src/variorum/config_architecture.c`. Refer to the enum for Intel architectures as an example. You may need to check for the platform in a few places in the `config_architecture.c` file. Examples of Intel and IBM are included, so these can be referred to.
4. If you need to modify front-facing APIs, add them to `variorum.h` and `variorum.c`.


Steps to Add Support for a New Architecture (pfm_arch)
------------------------------------------------------

1. Follow the steps listed above to create a new platform if the platform does not already exist in the Variorum source.
2. For each architecture, add a `pfm_arch.h` and `pfm_arch.c` file, define the internal get/set functions for capturing power, thermal, performance data. These need to be added as function pointers in the platform file (`config_pfm.h` and `config_pfm.c` files).
3. The internal implementation will depend on the interfaces, such as sensors, MSRs, OPAL, IPMI, etc. If applicable, these can be re-used across architectures (i.e., the same implementation is used for many architectures).


Documentation on Supported Platforms
------------------------------------

For specifics on the platforms that we support, please see the following pages.

.. toctree::
   :maxdepth: 2

   HWArchitectures
