.. # Copyright 2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

#################
 NVIDIA Overview
#################

This page provides a detailed description of the the NVIDIA port of Variorum.
The functionality of this port depends on NVIDIA-specific proprietary software
stack as well as open-source software components described below. The
high-level API provided by Variorum is read-only (i.e., monitoring-only),
primarily because of the access limitations on our target platform.

*********************
 Build Configuration
*********************

We provide an example CMake host config file, which defines the CMake build
variables set on our test platform (Lassen supercomputer at LLNL):
`lassen-4.14.0-ppc64le-gcc@4.9.3-cuda@10.1.243.cmake`.

For your build system, you will need to modify two CMake variables:

   -  ``CMAKE_SHARED_LINKER_FLAGS``: Path to libnvidia-ml.so (prefixed with the
      '-L' flag)
   -  ``HWLOC_DIR``: Path for the CUDA-aware version of libhwloc

You should also enable Variorum to build for NVIDIA's architecture:
``VARIORUM_WITH_NVIDIA=ON``

**************
 Dependencies
**************

The NVIDIA port of Variorum depends on:

-  NVIDIA Management Library (NVML) for access to the telemetry and control
   interfaces. NVML provides standardized interfaces to the NVIDIA GPU devices
   enumerated by the proprietary NVIDIA device driver as ``/dev/nvidia[0-9]*``.

-  The latest version of the CUDA development toolkit, which delivers the
   headers for NVML.

-  CUDA-enabled build of the Portable Hardware Locality (hwloc) library to
   enumerate the GPU devices and their mappings to the host CPUs.

To successfully use the Variorum port of NVIDIA, please verify that the
``LD_LIBRARY_PATH`` environment variable has paths for both the CUDA library
and the CUDA-enabled hwloc library installed on the system. Also make sure that
access to the NVIDIA devices (``/dev/nvidia*``) through the NVIDIA driver are
set correctly for the user. This can be verified by running the `nvidia-smi`
command line tool.

We have tested our NVIDIA port with CUDA 9.2 and CUDA-enabled build of hwloc
1.11.10. The NVIDIA port has been tested on the Tesla GPU architecture (NVIDIA
Volta SM200).

********************
 Device Enumeration
********************

The NVIDIA port enumerates the system GPU devices and populates global GPU
device handles at initialization in the `initNVML()` method using the
`nvmlDeviceGetCount()` and `nvmlDeviceGetHandleByIndex()` NVML query APIs,
respectively. It then queries the number of CPUs using Variorum's internal
routine to query system topology which uses the CUDA-enabled hwloc. Based on
this information, it calculates the number of GPU devices associated with each
CPU assuming sequential device assignment on the system. This method also
initializes the internal state of NVML using the `nvmlInit()` API.

The device handles are stored in data structures of type `nvmlDevice_t` defined
in NVML. A device handle provides the logical-to-physical mapping between the
sequential device IDs and system device handles maintained by NVML internally
at state initialization. All NVML query and command APIs require the device
handles to perform the specified operation on the device. While the high-level
Variorum APIs operate over all devices, the internal routines in the NVIDIA
port use CPU ID to perform operations on the associated GPUs.

***************************************************
 Telemetry Collection Through NVML Query Interface
***************************************************

The NVIDIA port of Variorum leverages the device and unit query APIs provided
by NVML to collect per-GPU telemetry. The text below describes the specific
Variorum APIs, the corresponding NVML APIs, and the post-processing (if any)
performed by Variorum before presenting the data to the caller.

Power telemetry
===============

Variorum provides two APIs for power telemetry from the GPU devices:

   -  Average power usage
   -  Current power limit

To report the average power usage of a GPU device, Variorum leverages the
`nvmlDeviceGetPowerUsage()` API of NVML. The reported power is in Watts as an
integer.

To report the power limit assigned to a GPU device, Variorum leverages the
`nvmlDeviceGetPowerManagementLimit()` API of NVML. The reported power limit is
in Watts as an integer.

Thermal telemetry
=================

Variorum provides an API to report instantaneous GPU device temperature in
degree Celsius and integer precision. It leverages the
`nvmlDeviceGetTemperature()` NVML API to report the GPU device temperature.

Clocks telemetry
================

Variorum provides an API to report instantaneous Streaming Multi-processor (SM)
clock speed in MHz and integer precision. It leverages the
`nvmlDeviceGetClock()` NVML API to report the instantaneous SM clock speed.

Device utilization
==================

Variorum provides an API to report the instantaneous device utilization as a
percentage of time (samples) for which the GPU was in use (i.e., GPU occupancy
rate) in a fixed time window. It leverages the
`nvmlDeviceGetUtilizationRates()` API of NVML to report the device utilization
rate as a percentage in integer precision.

************
 References
************

-  `NVML API Reference <https://docs.nvidia.com/deploy/nvml-api/index.html>`_
