.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##################################
 Variorum Developer Documentation
##################################

Here is some information on how to extend Variorum to support additional
platforms and/or microarchitectures.

A **platform** refers to a hardware vendor, for example, Intel, IBM, or ARM. A
**microarchitecture** refers to a generation of hardware within a platform, for
example, Broadwell or Ivy Bridge for Intel Xeon processors; or Power8 and
Power9 for IBM Power processors.

***********************************************
 Steps to Add Support for a New Platform (pfm)
***********************************************

#. Create a new folder under `src/variorum/pfm`

#. Create a `config_pfm.h` and `config_pfm.c`. These two files implement the
   `detect_pfm_arch` and `set_pfm_func_ptrs` functions. The former identifies
   the new platform, and the latter sets function pointers for get/set power,
   thermals, etc. on the new platform. These function pointers refer to
   internal functions that are defined in microarchitecture-specific files (see
   next section).

#. Add a struct listing all the microarchitectures implemented on the new
   platform in `src/variorum/config_architecture.c`. Refer to the enum for
   Intel microarchitectures as an example. You may need to check for the
   platform in a few places in the `config_architecture.c` file. Examples of
   Intel and IBM are included, so these can be referred to.

#. If you need to modify front-facing APIs, add them to `variorum.h` and
   `variorum.c`.

*************************************************************
 Steps to Add Support for a New Microarchitecture (pfm_arch)
*************************************************************

#. Follow the steps listed above to create a new platform if the platform does
   not already exist in the Variorum source.

#. For each microarchitecture, add a `pfm_arch.h` and `pfm_arch.c` file, define
   the internal get/set functions for capturing power, thermal, performance
   data. These need to be added as function pointers in the platform file
   (`config_pfm.h` and `config_pfm.c` files).

#. The internal implementation will depend on the interfaces, such as sensors,
   MSRs, OPAL, IPMI, etc. If applicable, these can be re-used across
   microarchitectures (i.e., the same implementation is used for many
   microarchitectures).


*******
Example 
*******

As an example, to support additional Intel microarchitectures:
1. Under `Intel/` directory, create a `.h` and `.c` header and source file for
the respective microarchitecture. 
This will contain features specific to that microarchitecture,
which may or may not exist in previous generations.

2. Modify `Intel/config_intel.c` to set the function pointers for the
respective microarchitecture.

3. Include the new header file in `Intel/config_architecture.h`.

