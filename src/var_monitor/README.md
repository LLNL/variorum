VAR_MONITOR
===========
This directory contains three Variorum-based power monitors. The resulting
data is written to two files:
* hostname.var_monitor.dat
* hostname.var_monitor.summary

`hostname` will change based on the node where the monitoring is occurring. The
`summary` file contains global information such as execution time. The `dat`
file contains the time sampled data in column-delimited format.

The output files are unique, so you must rename or delete the files before
running multiple tests on the same node in the same directory.

All three monitors are wrappers around some other process that will be
executing on the node and includes logic so that only one power monitor is run
per node.

var_monitor
-----------
While a target executable is running, sample power usage and power limits (and
other performance counters) for all sockets in a node at a regular interval.

The example below will sample the power usage while executing a sleep for 10
seconds:

    $ var_monitor -a "sleep 10"

The var_monitor also allows sampling of utilization. The example below will sample
utilization metrics as well as power while executing a sleep for 10 seconds:

    $ var_monitor -u -a "sleep 10"

power_wrapper_static
--------------------
Before a target execution begins, set a package-level power cap, then
sample power usage and power limits (and other performance counters) for all
sockets in a node at a regular interval.

The example below will set a package-level power limit of 100W on each socket,
then samples the power usage while executing a sleep for 10 seconds:

    $ power_wrapper_static -w 100 -a "sleep 10"

power_wrapper_dynamic
--------------------
Before a target execution begins, set a package-level power cap, then
sample power usage and power limits (and other performance counters) for all
sockets in a node at a regular interval. Additionally, the power cap is
adjusted stepwise every 500 ms.

The example below will set an initial package-level power limit of 100W on each
socket, samples the power usage and adjusts the power cap stepwise while
executing a sleep for 10 seconds:

    $ power_wrapper_dynamic -w 100 -a "sleep 10"

Notes
-----
If you launch one of the power monitors and it appears to finish successfully,
but does not produce the result files, launch the power monitoring with the
`-c` flag. This will remove any semaphores leftover in shared memory.

    $ var_monitor -c
    $ power_wrapper_static -c
    $ power_wrapper_dynamic -c
