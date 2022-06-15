.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

###################
 Quick Start Guide
###################

The simplest way to install Variorum is using the default CMake settings (note,
the default build targets an Intel architecture). Building Variorum requires the
``hwloc`` and ``jansson`` libraries.

.. code:: bash

   sudo apt-get install libhwloc15 libhwloc-dev libjansson4 libjansson-dev

   git clone https://github.com/LLNL/variorum.git

   cd variorum
   mkdir build install
   cd build

   cmake -DCMAKE_INSTALL_PREFIX=../install ../src
   make
   make install

After this completes, ``install/`` will contain a Variorum install.

Note that ``HWLOC_DIR`` and ``JANSSON_DIR`` may need to be set correctly if 
installing from scratch. Variorum also supports ``host-config`` files, which make
the build process easier by setting all necessary paths in one place. Details on
creating and using host configuration files can be found `here <https://variorum.readthedocs.io/en/2022_update_docs/BuildingVariorum.html#host-config-files>`_. 

For more details about building and installing Variorum, see
:doc:`BuildingVariorum`, which provides detailed information about building
Variorum from CMake initial-cache files (i.e., host config files) for specific
hosts, and Variorum's other CMake options and spack package.

Please ensure that the dependencies for each platform are met before building 
Variorum. The include the kernel module ``msr-safe`` for Intel systems, ``msr``, 
``amd_energy_driver`` and HSMP driver for AMD sytems, OPAL firmware and sensors 
for IBM, and NVML for NVIDIA. Details of each of these can be found in the 
respective vendor pages, see :doc:`HWArchitectures`.

For beginners, the ECP Variorum Lecture Series is beneficial. Slides and recordings
can be found here: https://www.exascaleproject.org/event/variorum-class-series/



