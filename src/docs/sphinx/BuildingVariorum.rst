.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
.. # Variorum Project Developers. See the top-level LICENSE file for details.
.. #
.. # SPDX-License-Identifier: MIT


Building Variorum
=================

This page provides details on how to build Variorum from source.

For the shortest path, see :doc:`QuickStart`.


Overview
--------

Variorum uses CMake for its build system.
Building Variorum creates the libvariorum library.

The build dependencies for a minimal build require the following:

    * C
    * hwloc
    * jansson


Build Dependencies
------------------

Variorum
^^^^^^^^

    * hwloc
    * jansson

hwloc (Required)
""""""""""""""""

No dependencies.

jansson (Required)
""""""""""""""""

No dependencies.

Getting Started
---------------
Clone the Variorum repo from GitHub:

.. code:: bash

    git clone https://lc.llnl.gov/bitbucket/scm/var/variorum.git


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

* **BUILD_DOCS** - Controls if the Variorum documentation is built (when sphinx and doxygen are found ) *(default = ON)*.

* **BUILD_SHARED_LIBS** - Controls if shared (ON) or static (OFF) libraries are built *(default = ON)*.

* **BUILD_TESTS** - Controls if unit tests are built *(default = ON)*.

* **HWLOC_DIR** - Path to an HWLOC install.

* **JANSSON_DIR** - Path to a JANSSON install.

* **SPHINX_EXECUTABLE** - Path to sphinx-build binary *(required for documentation)*.

* **VARIORUM_DEBUG** - Enable Variorum debug statements, useful if values are
  not translating correctly *(default = OFF)*.

* **VARIORUM_LOG** - Enable Variorum logging statements, useful for tracking what
  code path is being taken *(default = ON)*.

* **VARIORUM_WITH_AMD** - (not yet implemented) Enable Variorum build for AMD architecture *(default = OFF)*.

* **VARIORUM_WITH_NVIDIA** - Enable Variorum build for Nvidia architecture *(default = OFF)*.

* **VARIORUM_WITH_IBM** - Enable Variorum build for IBM architecture *(default = OFF)*.

* **VARIORUM_WITH_INTEL** - Enable Variorum build for Intel architecture *(default = ON)*.

* **USE_MSR_SAFE_BEFORE_1_5_0** - Use msr-safe prior to v1.5.0, dependency of
Intel architectures for accessing counters from userspace *(default =
ON)*.

* **ENABLE_FORTRAN** - Enable fortran compiler for building example
integration with fortran application, fortran compiler must exist *(default = ON)*.

* **ENABLE_WARNINGS** - Build with compiler warning flags -Wall -Wextra
-Werror, used primarily by developers
*(default = OFF)*.

Host Config Files
-----------------
To handle build options, third party library paths, etc., we rely on CMake's
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

To install Variorum with all options (and also build all of its dependencies as
necessary) run:

.. code:: bash

    spack install variorum

The Variorum spack package provides several `variants <http://spack.readthedocs.io/en/latest/basic_usage.html#specs-dependencies>`_
that customize the options and dependencies used to build Variorum:

 ================ ======================================== ============================
  Variant         Description                              Default
 ================ ======================================== ============================
  **shared**      Build Variorum as shared library         ON (+shared)
  **docs**        Build Variorum's Documentation           OFF (~docs)
  **log**         Enable Variorum's logging infrastructure OFF (~docs)
  **build_type**  Specify build type                       Release (build_type=Release)
 ================ ======================================== ============================

Variants are enabled using ``+`` and disabled using ``~``. For example, to
build Conduit with the minimum set of options (and dependencies) run:

.. code:: bash

    spack install variorum~docs

To build Variorum's tests run:

.. code:: bash

    spack install variorum~docs --run-tests
