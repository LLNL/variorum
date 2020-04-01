.. # Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
.. # Variorum Project Developers. See the top-level LICENSE file for details.
.. #
.. # SPDX-License-Identifier: MIT


Variorum Utilities
==================
Variorum provides some utilities to assist users and developers. Currently,
Variorum provides two main utilities:

* ``verify_opal.py``: A python script to verify the system environment (i.e.,
  OPAL) for IBM platforms.
* ``verify_msr_kernel.py``: A python script to verify the system environment
  (i.e., msr kernel or msr-safe kernel) for Intel.

Verify OPAL
-----------
This python script verifies that the OPAL files are present and have the
required R/W permissions on the target IBM hardware.

How do I use it?
""""""""""""""""
From the top-level Variorum directory:

.. code:: bash

   brink2@lassen11:~]$ python ./src/utilities/verify_opal.py -v
   ###################
   # IBM OPAL Access #
   ###################
   -- Check if OPAL files exist
   -- Check if /sys/firmware/opal/exports/occ_inband_sensors is accessible by user: /sys/firmware/opal/exports/occ_inband_sensors -- yes
   -- Check if /sys/firmware/opal/powercap/system-powercap/powercap-current is accessible by user: /sys/firmware/opal/powercap/system-powercap/powercap-current -- yes
   -- Check if /sys/firmware/opal/powercap/system-powercap/powercap-max is accessible by user: /sys/firmware/opal/powercap/system-powercap/powercap-max -- yes
   -- Check if /sys/firmware/opal/powercap/system-powercap/powercap-min is accessible by user: /sys/firmware/opal/powercap/system-powercap/powercap-min -- yes
   -- Check if /sys/firmware/opal/psr/cpu_to_gpu_0 is accessible by user: /sys/firmware/opal/psr/cpu_to_gpu_0 -- yes
   -- Check if /sys/firmware/opal/psr/cpu_to_gpu_8 is accessible by user: /sys/firmware/opal/psr/cpu_to_gpu_8 -- yes
   -- Check if OPAL files are accessible by user
   -- Check if OCC file is accessible by user: /sys/firmware/opal/exports/occ_inband_sensors -- yes
   -- Check if powercap-current file is accessible by user: /sys/firmware/opal/powercap/system-powercap/powercap-current -- yes
   -- Check if powercap-max file is accessible by user: /sys/firmware/opal/powercap/system-powercap/powercap-max -- yes
   -- Check if powercap-min file is accessible by user: /sys/firmware/opal/powercap/system-powercap/powercap-min -- yes
   -- Check if cpu_to_gpu0 file is accessible by user: /sys/firmware/opal/psr/cpu_to_gpu_0 -- yes
   -- Check if cpu_to_gpu8 file is accessible by user: /sys/firmware/opal/psr/cpu_to_gpu_8 -- yes
   -- Valid OPAL access

Invoke the script with python on the target IBM system. The `-v` flag enables
verbose output, which can be helpful if your programs are running to
permissions issues. The output of this script is helpful to send to the mailing
list for debugging system issues.

The last line of the output will (verbose or not) will indicate if the IBM
OPAL files exist and have the appropriate permissions.

Why Do I Need This?
"""""""""""""""""""
This is a helpful utility to run before launching any examples or tests, as it
can verify that the required environment for Variorum is configured
successfully.

Verify MSR Kernel
-----------------
This utility will check if the stock msr kernel or the msr-safe kernel are
loaded and configured correctly with the appropriate R/W permissions. It will
first check if the msr kernel is loaded and has appropriate permissions. If
this fails, then it will check if the msr-safe kernel is loaded and has
appropriate permissions.

How do I use it?
""""""""""""""""
From the top-level Variorum directory:

.. code:: bash

   brink2@quartz5:~]$ python ./src/utilities/verify_msr_kernel.py -v
   ######################
   # x86 CPU MSR Access #
   ######################
   -- Check if msr_safe kernel is loaded
   -- Check if msr_safe kernel is loaded -- yes
   -- Check if msr_safe kernel files are character devices
   -- Check if msr_safe kernel files are character devices: /dev/cpu/0/msr_safe -- yes
   -- Check if msr_safe kernel files are character devices: /dev/cpu/1/msr_safe -- yes
   -- Check if msr_safe kernel files are character devices: /dev/cpu/2/msr_safe -- yes
   -- Check if msr_safe kernel files are character devices: /dev/cpu/3/msr_safe -- yes
   ...
   -- Check if msr_safe kernel files are character devices: /dev/cpu/71/msr_safe -- yes
   -- Check if msr_safe kernel files are character devices: /dev/cpu/msr_whitelist -- yes
   -- Check if msr_safe kernel files are character devices: /dev/cpu/msr_batch -- yes
   -- Check if msr_safe kernel files are accessible by user
   -- Check if msr_safe kernel files are accessible by user: /dev/cpu/0/msr_safe -- yes
   -- Check if msr_safe kernel files are accessible by user: /dev/cpu/1/msr_safe -- yes
   -- Check if msr_safe kernel files are accessible by user: /dev/cpu/2/msr_safe -- yes
   -- Check if msr_safe kernel files are accessible by user: /dev/cpu/3/msr_safe -- yes
   ...
   -- Check if msr_safe kernel files are accessible by user: /dev/cpu/71/msr_safe -- yes
   -- Check if msr_safe kernel files are accessible by user: /dev/cpu/msr_whitelist -- yes
   -- Check if msr_safe kernel files are accessible by user: /dev/cpu/msr_batch -- yes
   -- Valid kernel loaded: msr-safe

Invoke the script with python on the target Intel system. The `-v` flag enables
verbose output, which can be helpful if your programs are running to
permissions issues. The output of this script is helpful to send to the mailing
list for debugging system issues.

The last line of the output will (verbose or not) will indicate if the msr or
msr-safe kernel is configured correctly and has the appropriate permissions.

Why Do I Need This?
"""""""""""""""""""
This is a helpful utility to run before launching any examples or tests, as it
can verify that the required environment for Variorum is configured
successfully.
