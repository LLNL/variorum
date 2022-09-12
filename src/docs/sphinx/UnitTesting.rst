..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

#####################
 Variorum Unit Tests
#####################

Variorum code is regularly tested on a diverse set of architectures, including
several Intel platforms (such as Broadwell, Haswell, Skylake, Ice Lake), IBM
Power 9, ARM Juno r2, NVIDIA Volta GPUs, and AMD Instinct GPUs. Variorum's unit
tests are run externally on GitHub, as well as internally on our Livermore
Computing clusters through GitLab.

For Variorum pull requests, we use GitHub Actions for CI testing, the worflow
can be found here: `Variorum GitHub Actions
<https://github.com/LLNL/variorum/actions>`_.

Additionally, we utilize `GitLab <https://lc.llnl.gov/gitlab>`_ runners on our
internal clusters for further CI testing on known hardware architectures, such
as Quartz, Lassen, and Corona `(see clusters at LLNL)
<https://hpc.llnl.gov/hardware>`_.

Within one of our GitLab runners, we are also leveraging `Ansible
<https://www.ansible.com/>`_ to expand our CI testing across an additional set
of hardware architectures. These systems require specific requests and
permissions to gain access.

As part of Variorum's CI testing, we cover the following:

-  Verifying compliance of code format for C/C++, RST, and Python files
-  Building Variorum with different versions of ``gcc`` compiler
-  Building Variorum with different CMake build options
-  Building Variorum using `Spack mirrors
   <https://github.com/llnl/variorum-spack-mirrors/>`_
-  Running Variorum's unit tests and examples (for example, ``make test`` and
   ``variorum-print-power-example``)
