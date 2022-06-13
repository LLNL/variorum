.. # Copyright 2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##############
 AMD Overview
##############

AMD platforms support in-band monitoring and control through sensors and 
machine-specific registers. AMD provides and open-source stack of its drivers
as well as its in-band library that Variorum leverages.

************
Requirements
************

Beginning with Variorum 0.5.0, AMD processors
from the AMD EPYC Milan family 19h, models 0-Fh and 30h-3Fh are supported.
This functionality has been tested on Linux distributions SLES15 and Ubuntu 18.04.
This version of depends on the AMD open-sourced software stack components
listed below:

1. EPYC™ System Management Interface In-band Library (E-SMI library) available at
   https://github.com/amd/esmi_ib_library

2. AMD Energy Driver
   https://github.com/amd/amd_energy

3. HSMP driver for power metrics
   https://github.com/amd/amd_hsmp

The E-SMI library provides the C API for user space application
of the AMD Energy Driver and the AMD HSMP modules.

The AMD Energy Driver is an out-of-tree kernel module that allows for
core and socket energy counter access through MSRs and RAPL via ``hwmon`` sys entries.
These registers are updated every millisecond and cleared on reset of the system.
Some registers of interest include:

* Power, Energy and Time Units
    - ``MSR_RAPL_POWER_UNIT/ C001_0299``: shared with all cores in the socket

* Energy consumed by each core
    - ``MSR_CORE_ENERGY_STATUS/ C001_029A``: 32-bitRO, Accumulator, core-level power reporting

* Energy consumed by Socket
    - ``MSR_PACKAGE_ENERGY_STATUS/ C001_029B``: 32-bitRO, Accumulator, socket-level power reporting, shared with all cores in socket

The Host System Management Port (HSMP) kernel module allows for *setting* of
power caps, boostlimits and PCIe access. It provides user level access to the
HSMP mailboxes implemented by the firmware in the System Management Unit (SMU).
AMD Power Control Knobs are exposed through HSMP via sysfs.

* ``amd_hsmp/cpuX/``: Directory for each possible CPU
    - ``boost_limit`` (RW): HSMP boost limit for the core in MHz

* ``amd_hsmp/socketX/``:  Directory for each possible socket
    - ``boost_limit`` (WO): Set HSMP boost limit for the socket in MHz
    - ``c0_residency`` (RO): Average % all cores are in C0 state
    - ``cclk_limit`` (RO): Most restrictive core clock (CCLK) limit in MHz
    - ``fabric_clocks`` (RO): Data fabric (FCLK) and memory (MCLK) in MHz
    - ``fabric_pstate`` (WO): Set data fabric P-state, -1 for autonomous
    - ``power`` (RO): Average socket power in milliwatts
    - ``power_limit`` (RW): Socket power limit in milliwatts
    - ``power_limit_max`` (RO): Maximum possible value for power limit in mW
    - ``proc_hot`` (RO): Socket PROC_HOT status (1 = active, 0 = inactive)
    - ``tctl`` (RO): Thermal Control value (not temperature)

We expect a similar software stack to be available on the upcoming El Capitan
supercomputer at Lawrence Livermore National Laboratory.

******************************************
 Monitoring and Control Through E-SMI API
******************************************

Variorum interfaces with AMD's E-SMI library for obtaining power and energy
information. These E-SMI APIs are described below.

The built-in monitoring interface on the AMD EPYC™ processors is implemented by
the SMU FW. All registers are updated every 1 millisecond.


Power telemetry
=================

* ``esmi_socket_power_get()``: Instantaneous power is reported in milliwatts

* ``esmi_socket_power_cap_get()`` and ``esmi_socket_power_cap_set()``: Get and Set power limit of the socket in milliwatts

* ``esmi_socket_power_cap_max_get()``: Maximum Power limit of the socket in milliwatts

Boostlimit telemetry
======================

Boostlimit is a maximum frequency a CPU can run at.

* ``esmi_core_boostlimit_get()`` and ``esmi_core_boostlimit_set()``: Get and set the current boostlimit for a given core

* ``esmi_socket_boostlimit_set()``: Set boostlimit for all the cores in the socket

Energy telemetry
==================

* ``esmi_socket_energy_get()``: Get software accumulated 64-bit energy counter for a given socket

* ``esmi_core_energy_get()``: Get software accumulated 64-bit energy counter for a given core

************
 References
************

-  `AMD EPYC™ processors Fam19h technical reference
   <https://www.amd.com/system/files/TechDocs/55898_B1_pub_0.50.zip>`_
