..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

###################################
 Monitoring Binaries with Variorum
###################################

While the Variorum API allows for detailed critical path analysis of the power
profile of user applications as well as for integration with system software
such as Kokkos, Caliper, and Flux through code annotations, there are scenarios
where such annotations are not possible. In order to support such scenarios, we
provide the ``powmon`` tool, which can monitor a binary externally with Variorum
in a vendor-neutral manner. This tool can monitor an application externally
without requiring any code changes or annotations.

The ``variorum/src/powmon`` directory contains this tool, which is built along
with the regular Variorum build. While a target executable is running,
``powmon`` collects time samples of power usage, power limits, energy, thermals,
and other performance counters for all sockets in a node at a regular interval.
By default, it collects basic node-level power information, such as CPU, memory,
and GPU power, at 50ms intervals, which it reports in a CSV format. It also
supports a verbose (``-v``) mode, where additional registers and sensors are
sampled for the advanced user. The sampling rate is configurable with the ``-i``
option. As an example, the command below will sample the power usage while
executing a sleep for 10 seconds in a vendor neutral manner:

.. code:: bash

   $ powmon -a "sleep 10"

The resulting data is written to two files:

.. code:: bash

   hostname.powmon.dat
   hostname.powmon.summary

Here, ``hostname`` will change based on the node where the monitoring is
occurring. The ``summary`` file contains global information such as execution
time. The ``dat`` file contains the time sampled data, such as power, thermals,
and performance counters in a column-delimited format. The output differs on
each platform based on available counters.

``Powmon`` also supports profiling across multiple nodes with the help of
resource manager commands (such as ``srun`` or ``jsrun``) or MPI commands (such
as ``mpirun``). As shown in the example below, the user can specify the number
of nodes through ``mpirun`` and utilize ``powmon`` with their application.

.. code:: bash

   $ mpirun -np <num-nodes> ./powmon -a ./application

We also provide a set of simple plotting scripts for ``powmon``, which are
located in the ``src/powmon/scripts`` folder. The ``powmon-plot.py`` script can
generate per-node as well as aggregated (across multiple nodes) graphs for the
default version of ``powmon`` that provides node-level and CPU, GPU and memory
data. This script works across all architectures that support Variorum's JSON
API for collecting power. Additionally, for IBM sensors data, which can be
obtained with the ``powmon -v`` (verbose) option, we provide a post processing
and R script for plots.

In addition to ``powmon`` that is vendor-neutral, for Intel systems only, we
provide two other power capping tools, ``power_wrapper_static``, and
``power_wrapper_dynamic`` that allow users to set a static (or dynamic) power
cap and then monitor their binary application.

The example below will set a package-level power limit of 100W on each socket,
and then sample the power usage while executing a sleep for 10 seconds:

.. code:: bash

   $ power_wrapper_static -w 100 -a "sleep 10"

Similarly, the example below will set an initial package-level power limit of
100W on each socket, sample the power usage, and then dynamically adjust the
power cap step-wise every 500ms while executing a sleep for 10 seconds:

.. code:: bash

   $ power_wrapper_dynamic -w 100 -a "sleep 10"
