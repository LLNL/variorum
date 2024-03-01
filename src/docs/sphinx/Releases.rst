..
   # Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##########
 Releases
##########

Variorum is under constant development. So, we recommend using our ``dev``
branch, which contains our latest features.

********
 v0.7.0
********

06/12/2023: Major release adds support for new architectures, ARM Neoverse N1
platform and Sapphire Rapids, enables heterogeneous build support for two
architectures, adds power capping API for NVIDIA, AMD and Intel GPUs, adds get
power limit for Intel GPUs, overhauls powmon utility, updates filenames and
low-level function names to include architecture, updates logo. `v0.7.0 tarball
here <https://github.com/LLNL/variorum/archive/v0.7.0.tar.gz>`_.

********
 v0.6.0
********

09/15/2022: Major release adds support for two new architectures, Intel discrete
GPUs and AMD GPUs, adds python wrappers and a pyVariorum python module, updates
existing JSON APIs to use char* instead of json_t*, creates new
variorum_topology.h header to expose get topology APIs, moves VARIORUM_LOG from
a build variable to an environment variable, standardizes examples with -h and
-v flags. `v0.6.0 tarball here
<https://github.com/LLNL/variorum/archive/v0.6.0.tar.gz>`_.

********
 v0.5.0
********

06/22/2022: Major release adds support for new architectures, AMD CPUs and Intel
Ice Lake CPU, adds examples for integrating Variorum into OpenMP and MPI
programs, adds dependency on rankstr library for MPI examples (optional),
renames clock_speed to frequency and power_limits to power_limit throughout
Variorum API, deprecates set_and_verify API. `v0.5.0 tarball here
<https://github.com/LLNL/variorum/archive/v0.5.0.tar.gz>`_.

********
 v0.4.1
********

04/02/2021: New release adds documentation for Nvidia port, adds units to the
tabular and JSON output formats, and finishes renaming dump and print internal
functions to print and print_verbose, respectively. `v0.4.1 tarball here
<https://github.com/LLNL/variorum/archive/v0.4.1.tar.gz>`_.

********
 v0.4.0
********

03/03/2021: Updated version includes support for ARM Juno architecture,
introduction of the JSON API, and Intel support for two versions of msr-safe.
`v0.4.0 tarball here <https://github.com/LLNL/variorum/archive/v0.4.0.tar.gz>`_.

********
 v0.3.0
********

04/23/2020: Updated version includes general infrastructure updates, such as
unit tests, example integrations, and small bug fixes throughout. `v0.3.0
tarball here <https://github.com/LLNL/variorum/archive/v0.3.0.tar.gz>`_.

********
 v0.2.0
********

03/13/2020: Updated version includes support for Nvidia GPGPUs. `v0.2.0 tarball
here <https://github.com/LLNL/variorum/archive/v0.2.0.tar.gz>`_.

********
 v0.1.0
********

11/11/2019: Initial release of Variorum. `v0.1.0 tarball here
<https://github.com/LLNL/variorum/archive/v0.1.0.tar.gz>`_.
