.. ############################################################################
.. # Copyright (c) 2015-2017, Lawrence Livermore National Security, LLC.
.. #
.. # Produced at the Lawrence Livermore National Laboratory
.. #
.. # LLNL-CODE-716457
.. #
.. # All rights reserved.
.. #
.. # This file is part of Conduit.
.. #
.. # For details, see: http://software.llnl.gov/alpine/.
.. #
.. # Please also read alpine/LICENSE
.. #
.. # Redistribution and use in source and binary forms, with or without
.. # modification, are permitted provided that the following conditions are met:
.. #
.. # * Redistributions of source code must retain the above copyright notice,
.. #   this list of conditions and the disclaimer below.
.. #
.. # * Redistributions in binary form must reproduce the above copyright notice,
.. #   this list of conditions and the disclaimer (as noted below) in the
.. #   documentation and/or other materials provided with the distribution.
.. #
.. # * Neither the name of the LLNS/LLNL nor the names of its contributors may
.. #   be used to endorse or promote products derived from this software without
.. #   specific prior written permission.
.. #
.. # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
.. # AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
.. # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
.. # ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
.. # LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY
.. # DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
.. # DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
.. # OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
.. # HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
.. # STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
.. # IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
.. # POSSIBILITY OF SUCH DAMAGE.
.. #
.. ############################################################################


Building Variorum
=================

Overview
--------

Variorum uses CMake for its build system.
Building Variorum creates the libvariorum library.

The build dependencies for a minimal build require the following:

    * C++
    * hwloc


Build Dependencies
------------------

hwloc (Required)
""""""""""""""""


Getting Started
---------------
Clone the Variorum repo from GitHub:

.. code:: bash

    git clone https://lc.llnl.gov/bitbucket/scm/var/libvariorum.git


Build, test, and install Variorum:

.. code:: bash

    mkdir build
    cd build
    cmake ../src
    make -j 8
    make install


Build Options
-------------

Variorum's build system supports the following CMake options:

* **BUILD_SHARED_LIBS** - Controls if shared (ON) or static (OFF) libraries are built. *(default = ON)*
* **ENABLE_TESTS** - Controls if unit tests are built. *(default = ON)*

* **ENABLE_DOCS** - Controls if the Variorum documentation is built (when sphinx and doxygen are found ). *(default = ON)*

* **HWLOC_DIR** - Path to an HWLOC install.

* **SPHINX_EXECUTABLE** - Path to a Sphinx binary *(required for documentation)*.

* **VARIORUM_DEBUG** - Enable Variorum debug statements *(default = OFF)*.

* **VARIORUM_LOG** - Enable Variorum log statements *(default = OFF)*.

* **VARIORUM_AMD** - Enable Variorum build for AMD architecture *(default = ON)*.

* **VARIORUM_GPU** - Enable Variorum build for GPU architecture *(default = ON)*.

* **VARIORUM_IBM** - Enable Variorum build for IBM architecture *(default = ON)*.

* **VARIORUM_INTEL** - Enable Variorum build for Intel architecture *(default = ON)*.


Host Config Files
-----------------
To handle build options, third party library paths, etc we rely on CMake's
initial-cache file mechanism.


.. code:: bash

    cmake -C config_file.cmake


We call these initial-cache files *host-config* files, since we typically
create a file for each platform or specific hosts if necessary.

There are example config files named by machine hostname, the SYS_TYPE
environment variable, and platform name (via *uname*) in the ``host-configs``
directory at the root of the Variorum repo. These can be passed to CMake via
the `-C` command line option.

.. code:: bash

    cmake {other options} -C host-configs/{config_file}.cmake ../src


These files use standard CMake commands. CMake *set* commands need to specify
the root cache path as follows:

.. code:: cmake

    set(CMAKE_VARIABLE_NAME {VALUE} CACHE PATH "")

It is  possible to create your own configure file, and a boilerplate example
is provided in `host-configs/boilerplate.cmake`


Building with Spack
-------------------

TBD
