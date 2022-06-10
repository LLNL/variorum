.. # Copyright 2021 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

###########################
 Integrating with Variorum
###########################

Variorum is a node-level library that can be integrated easily with higher-level
system software such as schedulers and runtime systems, to create a portable
HPC PowerStack(see :doc:`PowerStack`). As part of our efforts to support a hierarchical,
dynamic and open-source portable power management stack, we have integrated Variorum
with various open-source system software. The `JSON API <https://variorum.readthedocs.io/en/latest/VariorumAPI.html#json-api>`_ 
enables Variorum to interface with higher-level system software in an portable and easy manner.

************************************
Current and Planned ECP Integrations
************************************

Current integration efforts include a `Kokkos <https://kokkos.org>`_ connector 
for power monitoring, a `Caliper <https://software.llnl.gov/Caliper/>`_ service 
for method-level power data, and a `Flux <http://flux-framework.org>`_ power management 
module for scheduling. Upcoming integration also includes developing a Variorum 
interface for `Intel's GEOPM <https://geopm.github.io>`_ and 
`Sandia's OVIS Lightweight Distributed Metric Service (LDMS) <https://github.com/ovis-hpc/ovis-wiki/wiki>`_.

Links to Variorum's integrations with each of these frameworks can be found below. 
Note that these integrations are in early development stages and are expected to
be updated to support more features and tests.

   -  Variorum Kokkos connector:
      https://github.com/kokkos/kokkos-tools/tree/develop/profiling/variorum-connector
   -  Variorum Caliper service:
      https://github.com/LLNL/Caliper/tree/master/src/services/variorum     
   -  Flux System Power Manager Module with Variorum: 
      https://github.com/rountree/flux-power-mgr

**********************************
Contributing Variorum Integrations
**********************************

In order for tools to interact with Variorum, a simple JANSSON based parser is
sufficient. The format of the JSON object has been documented, and includes
total node power, as well as CPU, Memory, and GPU power (current assumption is
that of two sockets per node). It also includes the hostname and the timestamp.

We describe a simple example of how the data from the JSON object can be
extracted from Variorum in the client tool below, and we point developers to
the Kokkos and Flux integration links shown above for more detailed examples.
We have used the JANSSON library for our purposes, but the JSON object can also
be retrieved in a similar manner by other JSON libraries and supporting tools.

.. code:: c

   #include <jansson.h>

   void parse_variorum_data()
   {
       // Define a JSON object to retrieve data from Variorum in
       json_t *power_obj = json_object();

       // Define a local variable for the value of interest. For example, the
       // client side tool may only be interested in node power, or it may be
       // collecting samples for integration.
       double power_node;

       // Define a variable to capture appropriate return code from Variorum
       int ret;

       // Call the Variorum JSON API
       ret = variorum_get_node_power_json(power_obj);
       if (ret != 0)
       {
           printf("Variorum get node power API failed.\n");
       }

       // Extract the value of interest from the JSON object by using the
       // appropriate get function. Documentation of these can be found in the
       // JANSSON library documentation.
       power_node = json_real_value(json_object_get(power_obj, "power_node"));
       printf("Node power is: %lf\n", power_node);

       // Decrement references to JSON object, required for JANSSON library.
       json_decref(power_obj);
   }
