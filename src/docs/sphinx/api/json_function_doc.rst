..
  # Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
  # Variorum Project Developers. See the top-level LICENSE file for details.
  #
  # SPDX-License-Identifier: MIT

**********
 JSON API
**********

Obtaining Power Comsumption
===========================

The node power json API, ``variorum_get_node_power_json(char**)`` takes in a 
(``char**``) by reference, and populates this string with a nested JSON object 
in the following format.

{
    "hostname": {
        "timestamp": (integer value),
        "power_node_watts": (real value),
        "Socket_0": {
            "power_cpu_watts": (real value),
            "power_gpu_watts": (real value),
            "power_mem_watts": (real value),"
        },
        "Socket_n": {
            ...
        }
    }
}

where n is the number of sockets - 1 in that node indexed from 0. Each nested 
layer can be parsed into an individual JSON object in the JANSSON-C library.
An example of parsing the json string can be found in the 
variorum-integration-using-json-example.

Note that on IBM Power9 platform, only the first socket (Chip-0) has the PWRSYS
sensor, which directly reports total node power. Additional, both sockets here 
report CPU, Memory and GPU Power.

on Intel microarchitectures, total node power is not reported by hardware. As a
result, total node power is estimated by adding CPU and DRAM power on both
sockets.

For GPU power, IBM Power9 reports a single value, which is the sum of power
consumed by all the GPUs on a particular socket. Our JSON object captures this
with a ``power_gpu_socket_*`` interface, and does not report individual GPU
power in the JSON object (this data is however available separately without
JSON).

On systems without GPUs, or systems without memory power information, the value
of the JSON fields is currently set to -1.0 to indicate that the GPU power or
memory power cannot be measured directly. This has been done to ensure that the
JSON object in itself stays vendor-neutral from a tools perspective. A future
extension through NVML integration will allow for this information to report
individual GPU power as well as total GPU power per socket with a
cross-architectural build, similar to Variorum's ``variorum_get_node_power()``
API.

Querying Power Domains
======================

The node power domain info JSON API ``variorum_get_node_power_domain_info_json(char**)``
takes in a string by reference and outputs node domain info in the following format.
Please note that supported features for control and measurement may differ on individual
systems, only supported features are given. unsupported features are listed under the 
unsupported section.

{
    "hostname": {
        "timestamp": (integer value),
        "control": {
            "power_node": {
                "min": (integer value),
                "max": (integer value),
                "units": (string value)
            },
            "power_gpu": {
                "min": (integer value),
                "max": (integer value),
                "units": (string value)
            }
        },
        "unsupported": [],
        "measurement": {
            "power_node": {
                "units": (string value)
            }       
            "power_cpu": {
                "units": (string value)
            }
            "power_mem": {
                "units": (string value)
            }
            "power_gpu": {
                "units": (string value)
            }
        }
    }
}

Obtaining Thermal Information
=============================

The API to obtain node thermal has the following format. It takes a string
(``char**``) by reference as input, and populates this string with a nested 
JSON object with hostname, followed by socket_{number}, followed by CPU and 
or GPU (depending on the platform, may contain only one or both), followed 
by Core and Mem for CPU. 

The ``variorum_get_thermals_json(char **)`` function returns a string type
nested JSON object. An example is provided below.

{
    "hostname": {
        "Socket_0": {
            "CPU":  {
                "CORE": {
                    "CoreSensor1":  (Integer),
                    "CoreSensori":  (Integer),
                },
                "Mem":  {
                    "DimmSensor1":  (Integer),
                    :DimmSensori":  (Integer),
                 },
            },
            "GPU":  {
                "Device1":  (Integer),
                "Device2":  (Integer),
            }
        },
        "Timestamp_CPU" : (Integer)
    }
}

The names and numbers of sensors  will vary from system to system and the
number of sockets will depend on the node.
