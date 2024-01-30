.. # Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##########
 JSON API
##########

*******************************
 Obtaining Thermal Information
*******************************

The API to obtain node thermal has the following format. It takes a string
(``char**``) by reference as input, and populates this string with a nested
JSON object with hostname, followed by socket_{number}, followed by CPU and or
GPU (depending on the platform, may contain only one or both), followed by Core
and Mem for CPU.

The ``variorum_get_thermals_json(char **)`` function returns a string type
nested JSON object. An example is provided below::

    {
        "hostname": {
            "Socket_0": {
                "CPU":  {
                    "Core": {
                        "temp_celsius_core_0":  (Integer),
                         ...
                        "temp_celsius_core_i":  (Integer),
                    },
                    "Mem":  {
                        "temp_celsius_dimm_0":  (Integer),
                         ...
                        :temp_celsius_dimm_i":  (Integer),
                     },
                },
                "GPU":  {
                    "temp_celsius_gpu_0":  (Integer),
                     ...
                    "temp_celsius_gpu_i":  (Integer),
                }
            },
            "timestamp" : (Integer)
        }
    }

Here, ``i`` is the index of the core or GPU and ``0 <= i < num_cores/GPUs``.
