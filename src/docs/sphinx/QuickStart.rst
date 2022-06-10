.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

###################
 Quick Start Guide
###################

The simplest way to install Variorum is using the default CMake settings (note,
the default build targets an Intel architecture).

.. code:: bash

   git clone https://github.com/LLNL/variorum.git
   cd variorum
   mkdir build install
   cd build
   cmake -DCMAKE_INSTALL_PREFIX=../install ../src
   make
   make install

After this completes, ``install/`` will contain a Variorum install.

For more details about building and installing Variorum, see
:doc:`BuildingVariorum`. This page provides detailed info about building
Variorum from CMake initial-cache files (i.e., host config files) for specific
hosts, and Variorum's other CMake options and spack package.

Please ensure that the dependencies for each platform are met before building 
Variorum. The include the kernel module `msr-safe` for Intel systems, `msr`, 
`amd_energy_driver` and HSMP driver for AMD sytems, OPAL firmware and sensors 
for IBM, and NVML for NVIDIA. Details of each of these can be found in the 
respective vendor pages, see :doc:`HWArchitectures`.

For beginners, the ECP Variorum Lecture Series is beneficial. Slides and recordings
can be found here: https://www.exascaleproject.org/event/variorum-class-series/



