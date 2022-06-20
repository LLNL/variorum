.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

###################
 Building Variorum
###################

Variorum can be built from source with CMake or with ``spack``. Building Variorum creates the
``libvariorum`` library, the ``powmon`` monitoring tool, and variorum examples.

********************
 Build Dependencies
********************

The build dependencies for a **minimal build** with no parallel components require the following:
   -  C
   -  hwloc
   -  jansson
   -  Access to vendor-specific drivers or modules (see :doc:`HWArchitectures`).

For a **parallel build** with MPI/OpenMP, Variorum depends on:
   -  rankstr (only required for MPI/OpenMP builds)

The CMake variables (``ENABLE_MPI={ON,OFF}`` and ``ENABLE_OPENMP={ON,OFF}``) control
the building of parallel examples. If ``ENABLE_MPI=ON``, an MPI compiler is
required.


hwloc (Required)
================

`hwloc <https://www.open-mpi.org/projects/hwloc/>`_ is an open-source project
providing a portable abstraction of the hierarchical topology of modern
architectures.

Variorum leverages hwloc for detecting hardware topology. When reading/writing
a register on a particular hardware thread, hwloc can map that to the correct
physical socket.

jansson (Required)
==================

`jansson <https://digip.org/jansson/>`_ is an open-source C library for
encoding, decoding and manipulating JSON data.

Variorum leverages JANSSON to provide a JSON-based API that can retrieve power
data for external tools/software.

rankstr (Optional)
==================

`rankstr <https://github.com/ECP-VeloC/rankstr>`_ is an open-source C library
providing functions that identify unique strings across an MPI communicator.

Variorum leverages rankstr to split a communicator into subcommunicators by
hostname. The allows for a single control or monitor process in Variorum to
for example, enforce a power or frequency limit on a node or to print the
hardware counters once on a node.

********************
 Building with CMake
********************

Variorum can be built and installed as follows after cloning from GitHub:

.. code:: bash

   # install hwloc and jansson dependencies
   sudo apt-get install libhwloc15 libhwloc-dev libjansson4 libjansson-dev

   git clone https://github.com/llnl/variorum

   cd variorum
   mkdir build install
   cd build

   cmake -DCMAKE_INSTALL_PREFIX=../install ../src
   make -j8
   make install

******************
 Host Config Files
******************

To handle build options, third party library paths, etc., we rely on CMake's
initial-cache file mechanism. We call these initial-cache files ``host-config`` files,
as we typically create a file for each platform or specific hosts if necessary.
These can be passed to CMake via the ``-C`` command line option as shown below:

.. code:: bash

   cmake {other options} -C ../host-configs/{config_file}.cmake ../src

An example is provided in ``host-configs/boilerplate.cmake`` to create your own
configuration file. Example configuration files named by machine hostname, the
``SYS_TYPE`` environment variable, and platform name (via ``uname``) are also
provided in the ``host-configs`` directory. These files use standard CMake
commands. CMake ``set`` commands need to specify the root cache path as
follows:

.. code:: cmake

   set(CMAKE_VARIABLE_NAME {VALUE} CACHE PATH "")

********************
 CMake Build Options
********************

Variorum's build system supports the following CMake options:

-  ``HWLOC_DIR`` - Path to an HWLOC install.

-  ``JANSSON_DIR`` - Path to a JANSSON install.

-  ``SPHINX_EXECUTABLE`` - Path to sphinx-build binary (required for
   documentation).

-  ``VARIORUM_WITH_AMD (default=OFF)`` - Enable Variorum build for AMD architecture.

-  ``VARIORUM_WITH_NVIDIA (default=OFF)`` - Enable Variorum build for Nvidia architecture.

-  ``VARIORUM_WITH_IBM (default=OFF)`` - Enable Variorum build for IBM architecture.

-  ``VARIORUM_WITH_ARM (default=OFF)`` - Enable Variorum build for ARM architecture.

-  ``VARIORUM_WITH_INTEL (default=ON)`` - Enable Variorum build for Intel architecture.

-  ``ENABLE_FORTRAN (default=ON)`` - Enable Fortran compiler for building example
   integration with Fortran application, Fortran compiler must exist.

-  ``ENABLE_MPI (default=OFF)`` - Enable MPI compiler for building MPI examples, MPI compiler
   must exist.

-  ``ENABLE_OPENMP (default=ON)`` - Enable OpenMP extensions for building OpenMP examples.

-  ``ENABLE_WARNINGS (default=OFF)`` - Build with compiler warning flags -Wall -Wextra
   -Werror, used primarily by developers.

-  ``BUILD_DOCS (default=ON)`` - Controls if the Variorum documentation is built (when
   sphinx and doxygen are found).

-  ``BUILD_SHARED_LIBS (default=ON)`` - Controls if shared (ON) or static (OFF) libraries
   are built.

-  ``BUILD_TESTS (default=ON)`` - Controls if unit tests are built.

-  ``VARIORUM_DEBUG (default=OFF)`` - Enable Variorum debug statements, useful if values are
   not translating correctly.

-  ``VARIORUM_LOG (default=ON)`` - Enable Variorum logging statements, useful for tracking
   what code path is being taken.

-  ``USE_MSR_SAFE_BEFORE_1_5_0 (default=OFF)`` - Use msr-safe prior to v1.5.0, dependency of
   Intel architectures for accessing counters from userspace.

*********************
 Building with Spack
*********************

To install Variorum with all options (and also build all of its dependencies as
necessary) run:

.. code:: bash

   spack install variorum

The Variorum spack package provides several `variants
<http://spack.readthedocs.io/en/latest/basic_usage.html#specs-dependencies>`_
that customize the options and dependencies used to build Variorum (see table below).
Variants are enabled using ``+`` and disabled using ``~``.

   +----------------+----------------------------------------+------------------------------+
   | Variant        | Description                            | Default                      |
   +================+========================================+==============================+
   | **shared**     | Build Variorum as shared library       | ON (+shared)                 |
   +----------------+----------------------------------------+------------------------------+
   | **docs**       | Build Variorum's Documentation         | OFF (~docs)                  |
   +----------------+----------------------------------------+------------------------------+
   | **log**        | Enable Variorum's logging              | OFF (~log)                   |
   |                | infrastructure                         |                              |
   +----------------+----------------------------------------+------------------------------+
   | **build_type** | Specify build type                     | Release with Debug Info      |
   |                |                                        | (build_type=RelWithDebugInfo)|
   +----------------+----------------------------------------+------------------------------+

