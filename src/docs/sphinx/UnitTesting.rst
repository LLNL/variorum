..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

#####################
 Variorum Unit Tests
#####################

Variorum code is regularly tested on a diverse set of architectures, including 
several Intel platforms (such as Broadwell, Haswell, Skylake, Ice Lake), IBM Power 9, 
ARM Juno r2, NVIDIA Volta GPUs, and AMD Instinct GPUs. These tests are run externally
on GitHub, as well as internally on our Livermore Computing clusters. 

For the Variorum pull requests, we use GitHub Actions for CI testing, the details
of which can be found here: 

-  `Variorum GitHub Actions <https://github.com/LLNL/variorum/actions>`_

Additionally, on our internal clusters, we utilize `GitLab <https://lc.llnl.gov/gitlab>`_ 
runners for further CI testing on known hardware architectures, such as the
Quartz, Lassen and Corona `clusters at LLNL <https://hpc.llnl.gov>`_.

Within one of our GitLab runners, we are also leveraging `Ansible
<https://www.ansible.com/>`_ to further expand our CI testing across an
additional set of hardware architectures. These systems require specific
requests and permissions to gain access.

As part of Variorum's CI testing, we cover the following:

-  Code formatting compliance for C/C++, RST, and Python files
-  Building variorum with various compilers
-  Running variorum's unit tests

