.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##########
 Variorum
##########

Variorum is an extensible, vendor-netural library for exposing power and performance
monitoring and control capabilities of low-level hardware knobs across diverse
architectures in a user-friendly manner. Variorum provides vendor-neutral APIs such that
the user can query or control hardware knobs without needing to know the
underlying vendor's implementation (for example, machine-specific registers
or sensor interfaces). These APIs enable HPC application developers to gain a better 
understanding of power, energy, and performance through various
metrics for the devices contained within the node. Additionally, the APIs may
enable system software to control hardware knobs to optimize for a particular
goal.

Variorum focuses on ease of use and reduced integration burden in applications:

-  It includes examples which demonstrate how to use Variorum in a stand-alone
   program.
-  It includes an example performance analysis sampler that runs alongside the
   application.
-  It includes a JSON API to allow integration with higher-level system software, such as job schedulers, distributed monitoring frameworks, or applcation-level runtime systems.

****************************
 Variorum Project Resources
****************************

**Online Documentation**
https://variorum.readthedocs.io/

**Github Source Repo**
http://github.com/llnl/variorum

**Issue Tracker**
http://github.com/llnl/variorum/issues

****************
 Lead Developers
****************

-  Stephanie Brink (LLNL)
-  Aniruddha Marathe (LLNL)
-  Tapasya Patki (LLNL)
-  Barry Rountree (LLNL)
-  Kathleen Shoga (LLNL)

************************
 Variorum Documentation
************************

.. toctree::
   :maxdepth: 2
   :caption: Basics

   QuickStart
   BuildingVariorum
   Releases
   Licenses

.. toctree::
   :maxdepth: 2
   :caption: Reference

   Overview
   VariorumAPI
   Examples
   Powmon
   Utilities
   Publications

.. toctree::
   :maxdepth: 2
   :caption: Vendor Notes

   HWArchitectures

.. toctree::
   :maxdepth: 2
   :caption: Contributing

   VariorumDevel
   VariorumTools

********************
 Indices and tables
********************

-  :ref:`genindex`
-  :ref:`modindex`
-  :ref:`search`
