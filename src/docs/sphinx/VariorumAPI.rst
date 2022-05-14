.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##############
 Variorum API
##############

Variorum supports vendor-neutral power and energy management through its
rich API. Please refer to the top-level API, as well as the specific descritptions of the JSON API and the Best Effort Power Capping API. The JSON API allows system software interacting with Variorum to obtain data in a portable, vendor-neutral manner. 

*************
Top-level API
*************

The top-level API for Variorum is shown in the `variorum.h` header file:

.. literalinclude:: ../../variorum/variorum.h
   :language: c

**********
 JSON API
**********

The current JSON API depends on the JANSSON-C library and has a vendor-neutral
format. The API has been tested on Intel, IBM and ARM
architectures.

Obtaining Power Consumption
===========================

The API to obtain node power has the following format. It takes
a ``json_t`` object by reference as input, and populates this JSON object with
CPU, memory, GPU (when available), and total node power. The total node power
is estimated as a summation of available domains if it is not directly reported
by the underlying architecture (such as Intel).

The ``variorum_get_node_power_json(json_t *)`` includes the JSON object with
the following keys:

* hostname (string value)
* timestamp (integer value)
* power_node (real value)
* power_cpu_watts_socket* (real value)
* power_mem_watts_socket* (real value)
* power_gpu_watts_socket* (real value)

The "*" here refers to Socket ID. While more than one socket is supported, our
test systems had only 2 sockets. Note that on IBM Witherspoon platform, only
the first socket (Chip-0) has the PWRSYS sensor, which directly reports total
node power. Both sockets here report CPU, Memory and GPU power. On Intel
Broadwell, total node power is not reported by hardware, thus total node power
is estimated by adding CPU and DRAM power on both sockets. For GPU power, IBM
Witherspoon reports a single value, which is the sum of power consumed by all
the GPUs on a particular socket. Our JSON object captures this with a
``power_gpu_socket_*`` interface, and does not report individual GPU power in
the JSON object (this data is however available separately without JSON). On
Intel Broadwell, on our system without GPUs, this value is currently set to
-1.0 to indicate that the GPU power value cannot be measured directly through
MSRs. This has been done to ensure that the JSON object in itself is
vendor-neutral from a tools perspective. A future extension through NVML
integration in the JSON object will allow for this information to report
individual GPU power as well as total GPU power per socket with a
cross-architectural build, similar to Variorum's ``variorum_get_node_power()``
API.


Querying Power Domains
======================

The API for querying power domains allows users to query Variorum to obtain
information about domains that can be measured and controlled on a certain
architecture. It also includes information on the units of measurement
and control, as well as information on the minimum and maximum values for
setting the controls (control_range). If a certain domain is unsupported, it is
marked as such.

The query API, ``variorum_get_node_power_domain_info_json(json_t *)``, accepts
a JSON object by reference and  includes the following vendor-neutral keys:

* hostname (string value)
* timestamp (integer value)
* measurement (comma-separated string value)
* control (comma-separated string value)
* unsupported (comma-separated string value)
* measurement_units (comma-separated string value)
* control_units (comma-separated string value)
* control_range (comma-separated string value)

***************************
 Best Effort Power Capping
***************************

We support setting best effort node power limits in a vendor-neutral manner.
This interface has been developed from the point of view of higher-level tools
that utilize Variorum on diverse architectures and need to make node-level
decisions. When the underlying hardware does not directly support a node-level
power cap, a best-effort power cap is determined in software to provide an
easier interface for higher-level tools (e.g. Flux, Kokkos, etc).

For example, while IBM Witherspoon inherently provides the ability to set a
node-level power cap in watts in hardware through its OPAL infrastructure,
Intel architectures currently do not support a direct node level power cap
through MSRs. Instead, on Intel architectures, fine-grained CPU and DRAM level
power caps can be dialed in using MSRs. Note that IBM Witherspoon does not
provide fine-grained capping for CPU and DRAM level, but allows for a
power-shifting ratio between the CPU and GPU components on a socket (see `IBM
documentation <https://variorum.readthedocs.io/en/latest/IBM.html>`_).

Our API, ``variorum_cap_best_effort_node_power_limit()``, allows us to set a
best effort power cap on Intel architectures by taking the input power cap
value, and uniformly distributing it across sockets as CPU power caps.
Currently, we do not set memory power caps, but we plan to develop better
techniques for best-effort software capping in the future.
