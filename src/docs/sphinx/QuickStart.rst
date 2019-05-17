Quick Start Guide
=================

Installing Variorum
-------------------

.. code:: bash

    git clone https://lc.llnl.gov/bitbucket/scm/var/libvariorum.git
    cd libvariorum
    mkdir build
    cd build
    cmake -DCMAKE_PREFIX_INSTALL=build ../src
    make
    make install

After this completes, ``build/`` will contain a Variorum install.
