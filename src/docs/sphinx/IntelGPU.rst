..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##############################
 Intel Discrete GPUs Overview
##############################

This page provides a detailed description of the the Intel Discrete GPU port of
Variorum. The functionality of this port depends on Intel-specific proprietary
software stack as well as open-source software components described below. The
high-level API provided by Variorum is currently read-only (i.e.,
monitoring-only), primarily because of the access limitations on our target
platform.

**************
 Requirements
**************

The Intel Discrete GPU port of Variorum depends on:

-  APMIDG 0.3.0 or later
-  One API 2022.06.30.002 or later

We have tested our port with Aurora early access systems with Intel ATS GPUs.

Note: at this point, monitoring power of Intel Discrete GPU requires no
additional permission. However, the permission default setting may change in a
future driver release. Please consult your sysadmin.

*********************
 Build Configuration
*********************

At this point, Intel GPUs are only available through an early evaluation system;
thus, we recommend you request your account via https://www.jlse.anl.gov/ and
request access to the Aurora early access systems (EAS). Once you gain access to
EAS, type the following command to load compilers and the libraries required to
build the Intel GPU port.

.. code:: console

   $ module load cmake oneapi apmidg jansson hwloc libiconv

We provide an example CMake host config file, which defines the CMake build
variables set on our test platform (Aurora early access supercomputer at ANL):
``arcticus-apmidg-oneapi.cmake``.

If you want to build variorum on other systems besides JLSE nodes, please
install all compiler and library dependencies, and then you will need to enable
Variorum to build with INTEL_GPU and set three path variables as described
below:

   -  ``VARIORUM_WITH_INTEL_GPU=ON``
   -  ``APMDIG_DIR``: Path to libapmidg.so (prefixed with the '-L' flag)
   -  ``HWLOC_DIR``: Path to libhwloc.so
   -  ``JANSSON_DIR``: Path to libjansson.so

********************
 Device Enumeration
********************

The Intel GPU port enumerates the system GPU devices at initialization in the
``initAPMIDG()`` method, which internally obtains the number of Intel GPU
devices via ``apmidg_getndevs()``. The number of GPUs per socket is simply the
number of available GPUs divided by the number of CPU sockets returned by
``variorum_get_topology()``.

*****************************************************
 Telemetry Collection Through APMIDG Query Interface
*****************************************************

The Intel GPU port of Variorum leverages the device and unit query APIs provided
by APMIDG to collect per-GPU telemetry or subdomain telemetry if available. The
text below describes the specific Variorum APIs, the corresponding APMIDG APIs,
and the post-processing (if any) performed by Variorum before presenting the
data to the caller.

Power telemetry
===============

Variorum provides two APIs for power telemetry from the GPU devices:

   -  Average power usage

To report the average power usage of a GPU device, Variorum leverages the
``apmidg_readpoweravg()`` API of APMIDG. The reported power is in Watts as a
floating point number.

Thermal telemetry
=================

Variorum provides an API to report instantaneous GPU device temperature in
degree Celsius and integer precision. It leverages the ``apmidg_readtemp()``
APMIDG API to report the GPU device temperature in Celsius.

Clocks telemetry
================

Variorum provides an API to report instantaneous clock speed of the Intel GPU's
execusion unit in MHz and integer precision. It leverages the
``apmidg_readfreq()`` APMIDG API to report the instantaneous clock speed.

*******************
 Control Interface
*******************

The Intel Discrete GPU port of Variorum leverages the device-level control APIs
provided by APMIDG. Variorum implements the following device control APIs using
the corresponding APMIDG APIs.

Power control
=============

In Variorum's GPU power capping API, Variorum uses the ``apmidg_setpwrlim()``
API of APMIDG which takes as input the GPU device ID, the power domain ID and
the power cap in milliwatts.

************
 References
************

-  `APMDIG library <https://github.com/anlsys/apmidg>`_
