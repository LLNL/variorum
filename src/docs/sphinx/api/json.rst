..
   # Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

*********
 JSON API
*********

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
