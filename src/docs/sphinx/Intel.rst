.. # Copyright 2019 Lawrence Livermore National Security, LLC and other
.. # Variorum Project Developers. See the top-level LICENSE file for details.
.. #
.. # SPDX-License-Identifier: MIT


Intel Overview
==============

There are two primary mechanisms in hardware for control:
1. Dynamic voltage and frequency scaling
2. Power capping, such as Running Average Power Limit (RAPL)

**DVFS**

* `cpufreq`
* `userspace` governor

**RAPL**

* Low-level register programming, supported with msr kernel module along with msr-tools
* LLNL's msr-safe provide safe and clean access from user space


Introduction to Intel's RAPL
----------------------------

Intel provides programmable, machine-specific registers for power, energy and
thermal management (power capping).

MSR Domains for server architectures:

* Package represents processor cores, caches, and other things on the socket
* DRAM represents memory components
* Other uncore registers also exist


Some Key RAPL Registers for Power Management
--------------------------------------------

* MSR_PKG_POWER_LIMIT, 0x610h, rw
    * Allows software to set package power limits

* MSR_PKG_ENERGY_STATUS, 0x611h, ro
    * Reports measured actual energy usage for package

* MSR_PKG_POWER_INFO, 0x614h, ro
    * Reports meta information for package, such as range of valid values

* MSR_DRAM_POWER_LIMIT, 0x618h, rw
    * Allows software to set memory power limits

* MSR_DRAM_ENERGY_STATUS, 0x619h, ro
    * Reports measured actual energy usage for memory

* MSR_DRAM_POWER_INFO, 0x61Ch, ro
    * Reports the DRAM domain meta information

* IA32_APERF_MSR and IA32_MPERF_MSR, 0xE7h, 0xE8h, ro
    * Define a ratio that depicts instantaneous processor frequency
