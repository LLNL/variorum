..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##############
 ARM Overview
##############

This implementation of the ARM port of Variorum supports the Arm Juno r2 SoC.
The Arm Juno r2 platform is a big.LITTLE cluster with Cortex-A72 (big) and
Cortex-A53 (LITTLE) clusters (also called processors), respectively. It also has
an Arm Mali GP-GPU. We have tested the ARM functionality of Variorum on Linaro
supported Linux.

**************
 Requirements
**************

This version of the ARM port of Variorum depends on the Linux Hardware
Monitoring (hwmon) subsystem for access to the telemetry and control interfaces
on the tested ARM platform. The standardized data interfaces provided by the
hwmon framework enable a generic ARM implementation of Variorum.

************************************************
 Monitoring and Control Through Sysfs Interface
************************************************

The built-in monitoring interface on the Arm Juno r2 board is implemented by the
on-board FPGA. Since this interface is not universally available on most Arm
implementations, we leverage the standard Linux sysfs interface for monitoring
and control. The following subsections provide the specific metrics that are
monitored on Arm Juno r2:

Power telemetry
===============

The sysfs interface provides a file for each of the following Advanced
Peripheral Bus (APB) energy meter registers:

   -  SYS_POW_SYS : ``/sys/class/hwmon/hwmon0/power1_input``
   -  SYS_POW_A72 : ``/sys/class/hwmon/hwmon0/power2_input``
   -  SYS_POW_A53 : ``/sys/class/hwmon/hwmon0/power3_input``
   -  SYS_POW_GPU : ``/sys/class/hwmon/hwmon0/power4_input``

Instantaneous power is recorded in bits 0-23 and reported in microwatts by the
sysfs interface. To improve readability of the verbose output Variorum converts
power into milliwatts before reporting. All registers are updated every 100
microseconds.

Memory power telemetry is not available on this platform.

Thermal telemetry
=================

The sysfs interface provides a file for thermal telemetry for each system
component through the GetSensorValue command in CSS System Control and Power
Interface (SCPI).

   -  SoC temperature: ``/sys/class/hwmon/hwmon0/temp1_input``
   -  big temperature: ``/sys/class/hwmon/hwmon0/temp2_input``
   -  LITTLE temperature: ``/sys/class/hwmon/hwmon0/temp3_input``
   -  GPU temperature: ``/sys/class/hwmon/hwmon0/temp4_input``

Instantaneous temperatures are reported in degree Celsius.

Clocks telemetry
================

Clocks are collected by the sysfs interface using the GetClockValue command in
SCPI. A separate ``policy*/`` subdirectory is provided for the big and LITTLE
cluster.

   -  big clocks: ``/sys/devices/system/cpu/cpufreq/policy0/scaling_cur_freq``
   -  LITTLE clocks:
      ``/sys/devices/system/cpu/cpufreq/policy1/scaling_cur_freq``

Frequencies are reported by the sysfs interface in KHz. Variorum reports the
clocks in MHz to keep it consistent with the clocks reported for other supported
architectures.

Frequency control
=================

The sysfs interface uses the SetClockValue SCPI command to set processor
frequency for the following user-facing interfaces:

   -  big clocks: ``/sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed``
   -  LITTLE clocks:
      ``/sys/devices/system/cpu/cpufreq/policy1/scaling_setspeed``

New frequency is specified in KHz to these interfaces. Variorum takes the new
frequency as input in MHz and performs this conversion internally.

If you run into an error accessing the sysfs interface, this could be due to an
the specified frequency value or the set governor. The sysfs interface only
accepts valid values for frequencies as output by
``policy*/scaling_available_frequencies``. Also, the specified frequency is only
applied when the governor in ``policy*/scaling_governor`` is set to `userspace`.

************
 References
************

-  `Arm Juno r2 technical reference
   <https://developer.arm.com/documentation/100114/0200/>`_
-  `hwmon sysfs interface
   <https://www.kernel.org/doc/Documentation/hwmon/sysfs-interface>`_
-  `hwmon documentation
   <http://blog.foool.net/wp-content/uploads/linuxdocs/hwmon.pdf>`_
-  `Energy Monitoring on Juno
   <https://community.arm.com/developer/tools-software/oss-platforms/w/docs/482/energy-monitoring-on-juno>`_
