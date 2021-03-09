.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

###################
 Quick Start Guide
###################

*********************
 Installing Variorum
*********************

The simplest way to install Variorum is using the default CMake settings (note,
the default build targets an Intel architecture).

.. code:: bash

   git clone https://github.com/LLNL/variorum.git
   cd variorum
   mkdir build
   cd build
   cmake -DCMAKE_PREFIX_INSTALL=build ../src
   make
   make install

After this completes, ``build/`` will contain a Variorum install.

For more details about building and installing Variorum, see
:doc:`BuildingVariorum`. This page provides detailed info about building
Variorum from CMake initial-cache files (i.e., host config files) for specific
hosts, and Variorum's other CMake options and spack package.

*******************************
 Accessing Low-Level Registers
*******************************

To use Variorum on Intel platforms, the `msr-safe
<https://github.com/llnl/msr-safe>`_ kernel driver must be loaded to enable
user-level read and write of allowed MSRs.

Alternately, you can use Variorum as root with the stock MSR kernel driver
loaded.

.. code:: bash

   modprobe msr

The kernel driver provides an interface to read and write MSRs on an x86
processor.

The msr-safe driver provides the following device files:

.. code:: bash

   ls /dev/cpu/<CPU>/msr_safe

The stock MSR driver provides the following device files:

.. code:: bash

   ls /dev/cpu/<CPU>/msr

For more details about accessing low-level counters and control knobs on other
architectures supported by Variorum, see :doc:`HWArchitectures`.
