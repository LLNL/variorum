.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

###################
 Building Variorum
###################

Variorum uses CMake for its build system. Building Variorum creates the
variorum library and the ``powmon`` monitoring tool.

The CMake variables (``ENABLE_MPI={ON,OFF}`` and ``ENABLE_OPENMP={ON,OFF}``) control
the building of parallel examples. If ``ENABLE_MPI=ON``, an MPI compiler is
required.

********************
 Build Dependencies
********************

The build dependencies for a ``minimal build`` with no parallel components require the following:
   -  C
   -  hwloc
   -  jansson
   -  Access to vendor-specific drivers or modules (see :doc:`HWArchitectures`).

For a ``parallel build`` with MPI/OpenMP, Variorum depends on:
   -  rankstr (only required for MPI/OpenMP builds)

*****************
 Getting Started
*****************

Clone the Variorum repo from GitHub:

.. code:: bash

   git clone https://github.com/llnl/variorum

Build, test, and install Variorum:

.. code:: bash

   mkdir build
   cd build
   cmake ../src
   make -j 8
   make install

***************
 Build Options
***************

Variorum's build system supports the following CMake options:

-  **ENABLE_FORTRAN** - Enable fortran compiler for building example
   integration with fortran application, fortran compiler must exist *(default
   = ON)*.

-  **ENABLE_MPI** - Enable MPI compiler for building MPI examples, MPI compiler
   must exist *(default = OFF)*.

-  **ENABLE_OPENMP** - Enable OpenMP extensions for building OpenMP examples,
   *(default = ON)*.

-  **ENABLE_WARNINGS** - Build with compiler warning flags -Wall -Wextra
   -Werror, used primarily by developers *(default = OFF)*.

-  **HWLOC_DIR** - Path to an HWLOC install.

-  **JANSSON_DIR** - Path to a JANSSON install.

-  **SPHINX_EXECUTABLE** - Path to sphinx-build binary *(required for
   documentation)*.

-  **BUILD_DOCS** - Controls if the Variorum documentation is built (when
   sphinx and doxygen are found ) *(default = ON)*.

-  **BUILD_SHARED_LIBS** - Controls if shared (ON) or static (OFF) libraries
   are built *(default = ON)*.

-  **BUILD_TESTS** - Controls if unit tests are built *(default = ON)*.

-  **VARIORUM_DEBUG** - Enable Variorum debug statements, useful if values are
   not translating correctly *(default = OFF)*.

-  **VARIORUM_LOG** - Enable Variorum logging statements, useful for tracking
   what code path is being taken *(default = ON)*.

-  **VARIORUM_WITH_AMD** - (not yet implemented) Enable Variorum build for AMD
   architecture *(default = OFF)*.

-  **VARIORUM_WITH_NVIDIA** - Enable Variorum build for Nvidia architecture
   *(default = OFF)*.

-  **VARIORUM_WITH_IBM** - Enable Variorum build for IBM architecture *(default
   = OFF)*.

-  **VARIORUM_WITH_ARM** - Enable Variorum build for ARM architecture *(default
   = OFF)*.

-  **VARIORUM_WITH_INTEL** - Enable Variorum build for Intel architecture
   *(default = ON)*.

-  **USE_MSR_SAFE_BEFORE_1_5_0** - Use msr-safe prior to v1.5.0, dependency of
   Intel architectures for accessing counters from userspace *(default = OFF)*.

*******************
 Host Config Files
*******************

To handle build options, third party library paths, etc., we rely on CMake's
initial-cache file mechanism.

.. code:: bash

   cmake -C config_file.cmake ../src

We call these initial-cache files *host-config* files, since we typically
create a file for each platform or specific hosts if necessary.

There are example config files named by machine hostname, the SYS_TYPE
environment variable, and platform name (via *uname*) in the ``host-configs``
directory at the root of the Variorum repo. These can be passed to CMake via
the `-C` command line option.

.. code:: bash

   cmake {other options} -C ../host-configs/{config_file}.cmake ../src

These files use standard CMake commands. CMake *set* commands need to specify
the root cache path as follows:

.. code:: cmake

   set(CMAKE_VARIABLE_NAME {VALUE} CACHE PATH "")

It is possible to create your own configure file, and a boilerplate example is
provided in `host-configs/boilerplate.cmake`

*********************
 Building with Spack
*********************

To install Variorum with all options (and also build all of its dependencies as
necessary) run:

.. code:: bash

   spack install variorum

The Variorum spack package provides several `variants
<http://spack.readthedocs.io/en/latest/basic_usage.html#specs-dependencies>`_
that customize the options and dependencies used to build Variorum:

   +----------------+----------------------------------------+----------------------------------------------+
   | Variant        | Description                            | Default                                      |
   +================+========================================+==============================================+
   | **shared**     | Build Variorum as shared library       | ON (+shared)                                 |
   +----------------+----------------------------------------+----------------------------------------------+
   | **docs**       | Build Variorum's Documentation         | OFF (~docs)                                  |
   +----------------+----------------------------------------+----------------------------------------------+
   | **log**        | Enable Variorum's logging              | OFF (~log)                                  |
   |                | infrastructure                         |                                              |
   +----------------+----------------------------------------+----------------------------------------------+
   | **build_type** | Specify build type                     | RelWithDebugInfo                             |
   |                |                                        | (build_type=RelWithDebugInfo)                |
   +----------------+----------------------------------------+----------------------------------------------+

Variants are enabled using ``+`` and disabled using ``~``. For example, to
build Variorum with the minimum set of options (and dependencies) run:

.. code:: bash

   spack install variorum
