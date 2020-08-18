.. # Copyright 2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##############
 AMD Overview
##############

This implementation of the AMD port of Variorum supports the AMD processors
for family support refer to dependencie section. We have tested the AMD
functionality of Variorum on SLES15 and Ubuntu 18.04.

**************
 Dependencies
**************

The following software stack is supported on family 19h model 0~Fh and 30h ~ 3Fh
processors from AMD, as of April 2021.

This version of the AMD port of Variorum depends on the AMD open-sourced

1. EPYC™ System Management Interface In-band Library (E-SMI library) available at
   https://github.com/amd/esmi_ib_library

2. HSMP driver for power metrics
   https://github.com/amd/amd_hsmp

2. amd_energy driver for core and socket energy counters
   https://github.com/amd/amd_energy

******************************************
 Monitoring and Control Through E-SMI API
******************************************

The built-in monitoring interface on the AMD EPYC™ processors is implemented by
the SMU FW. The following subsections provide the specific metrics:

All registers are updated every 1 milli second.

 Power telemetry
=================

The following E-SMI APIs are used in this version of AMD port

esmi_socket_power_get()
- Instantaneous power is reported in milliwatts

esmi_socket_power_cap_get() and esmi_socket_power_cap_set()
- Get and Set power limit of the socket in milliwatts

esmi_socket_power_cap_max_get()
- Maximum Power limit of the socket in milliwatts

To improve readability of the verbose output Variorum converts power into
watts before logging.

 Boostlimit telemetry
======================

Boostlimit is a maximum frequency a CPU can run at

esmi_core_boostlimit_get() and esmi_core_boostlimit_set()
- Get and set the current boostlimit for a given core

esmi_socket_boostlimit_set()
- Set boostlimit for all the cores in the socket

 Energy telemetry
==================

esmi_socket_energy_get()
- Get software accumulated 64-bit energy counter for a given socket

esmi_core_energy_get()
- Get software accumulated 64-bit energy counter for a given core

************
 References
************

-  `AMD EPYC™ processors Fam19h technical reference
   <https://www.amd.com/system/files/TechDocs/55898_pub.zip>`_
