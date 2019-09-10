.. # Copyright 2019 Lawrence Livermore National Security, LLC and other
.. # Hatchet Project Developers. See the top-level LICENSE file for details.
.. #
.. # SPDX-License-Identifier: MIT


.. _quick_start:

Quick Start Guide
=================

Installing Variorum
-------------------

.. code:: bash

    git clone https://lc.llnl.gov/bitbucket/scm/var/variorum.git
    cd variorum
    mkdir build
    cd build
    cmake -DCMAKE_PREFIX_INSTALL=build ../src
    make
    make install

After this completes, ``build/`` will contain a Variorum install.
