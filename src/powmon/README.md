POWMON
======
This directory contains three libmsr-based power monitors.

powmon
------
Samples and prints power consumption and allocation per socket for
systems with two sockets

power_wrapper_static
--------------------
Samples and prints power consumption and allocation per socket for systems with
two sockets after setting a power cap

power_wrapper_dynamic
--------------------
Samples and prints power consumption and allocation per socket for systems with
two sockets and adjusts the cap stepwise every 500 ms


Notes
-----
Each monitor includes a usage string. All three monitors are wrappers around
some other process that will be executing on the node and include logic so that
only one monitor is run per node.
