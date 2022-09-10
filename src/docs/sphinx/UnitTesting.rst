..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

#####################
 Variorum Unit Tests
#####################

We use GitHub Actions for CI testing Variorum pull requests.

-  `Variorum GitHub Actions <https://github.com/LLNL/variorum/actions>`_

Internally, we are also using `GitLab <https://lc.llnl.gov/gitlab>`_ runners on
Livermore Computing systems for further CI testing on known hardware
architectures.

Variorum's CI testing covers:

-  code formatting compliance for C/C++, RST, and Python files
-  building variorum
-  running variorum's unit tests

Within one of our GitLab runners, we are also leveraging `Ansible
<https://www.ansible.com/>`_ to further expand our CI testing across an
additional set of hardware architectures. These systems require specific
requests to gain access.
