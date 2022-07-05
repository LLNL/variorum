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
in a vendor-neutral manner.

The ``variorum/src/powmon`` directory contains this tool, which is built along
with the regular Variorum build. While a target executable is running,
``powmon`` collects time samples of power usage, power limits, energy, thermals,
and other performance counters for all sockets in a node at a regular interval.
For example, the command below will sample the power usage while executing a
sleep for 10 seconds:

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
each platform based on available counters. Currently, Intel and IBM platforms
are supported with ``powmon``.

Additionally, we provide two other tools, ``power_wrapper_static``, and
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
