.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
.. # Variorum Project Developers. See the top-level LICENSE file for details.
.. #
.. # SPDX-License-Identifier: MIT


Variorum API
============

The top-level API for Variorum consists of the following calls:

.. literalinclude:: ../../variorum/variorum.h
   :language: c

JSON API
^^^^^^^^
The current JSON API depends on the JANSSON-C library and has a vendor-neutral
format. The API has been tested on Intel Broadwell and IBM Witherspoon 
architectures. The API to obtain node power has the following format. 
It takes a ``json_t`` object by reference as input, and populates this JSON object 
with CPU, memory, GPU (when available), and total node power. The total node 
power is estimated as a summation of available domains if it is not directly 
reported by the underlying architecture (such as Intel). 

Currently, only ``variorum_get_node_power_json(json_t *)`` is supported, and the 
JSON object has the following keys:

* hostname (string value)
* timestamp (integer value)
* power_node (real value) 
* power_cpu_socket_* (real value)
* power_mem_socket_* (real value)
* power_gpu_socket_* (real value) 

The "*" here refers to Socket ID. While more than one socket is supported, 
our test systems had only 2 sockets. Note that on IBM Witherspoon platform, 
only the first socket (Chip-0) has the PWRSYS sensor, which directly reports 
total node power. Both sockets here report CPU, Memory and GPU power.
On Intel Broadwell, total node power is not reported by hardware, thus total node 
power is estimated by adding CPU and DRAM power on both sockets. 
For GPU power, IBM Witherspoon reports a single value, which is the sum of power 
consumed by all the GPUs on a particular socket. Our JSON object captures this 
with a ``power_gpu_socket_*`` interface, and does not report individual GPU power
in the JSON object (this data is however available separately without JSON). 
On Intel Broadwell, on our system without GPUs, this value is currently set 
to -1.0 to indicate that the GPU power value cannot be measured directly through 
MSRs. This has been done to ensure that the JSON object in itself is 
vendor-neutral from a tools perspective. A future extension through NVML 
integration in the JSON object will allow for this information to report 
individual GPU power as well as total GPU power per socket with a 
cross-architectural build, similar to Variorum's ``variorum_get_node_power()`` API.
 
Best Effort Power Capping
^^^^^^^^^^^^^^^^^^^^^^^^^

We support setting best effort node power limits in a vendor-neutral manner. 
This interface has been developed from the point of view of higher-level tools 
that utilize Variorum on diverse architectures and need to make node-level 
decisions. When the underlying hardware does not directly support a node-level 
power cap, a best-effort power cap is determined in software to provide an 
easier interface for higher-level tools (e.g. Flux, Kokkos, etc). 

For example, while IBM Witherspoon inherently provides the ability to set a 
node-level power cap in watts in hardware through its OPAL infrastructure, 
Intel architectures currently do not support a direct node level power cap through
MSRs. Instead, on Intel architectures, fine-grained CPU and DRAM level power 
caps can be dialed in using MSRs. Note that IBM Witherspoon does not provide
fine-grained capping for CPU and DRAM level, but allows for a power-shifting
ratio between the CPU and GPU components on a socket 
(see `IBM documentation <https://variorum.readthedocs.io/en/latest/IBM.html>`_).

Our API, ``variorum_cap_best_effort_node_power_limit()``, allows us to set a 
best effort power cap on Intel architectures by taking the input power cap value, 
and uniformly distributing it across sockets as CPU power caps. Currently, we do
not set memory power caps, but we plan to develop better techniques for 
best-effort software capping in the future. 


