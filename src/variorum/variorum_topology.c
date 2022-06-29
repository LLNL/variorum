// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <hwloc.h>

#include <variorum_topology.h>

hwloc_topology_t topology;

int variorum_init_topology(void)
{
    int rc;
    static int init_variorum_init_topology = 0;

    if (!init_variorum_init_topology)
    {
        init_variorum_init_topology = 1;

        rc = hwloc_topology_init(&topology);
        if (rc != 0)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc topology initialization error. "
                    "Exiting.", __FILE__, __LINE__);
            exit(-1);
        }

        rc = hwloc_topology_load(topology);
        if (rc != 0)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc topology load error. "
                    "Exiting.", __FILE__, __LINE__);
            exit(-1);
        }
    }

    // Initialization is successful if we reach this point.
    return 0;
}

int variorum_get_num_sockets(void)
{
    int num_sockets = -1;
    int rc = -1;

    rc = variorum_init_topology();

    if (rc == 0)
    {
        num_sockets = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_SOCKET);
        return num_sockets;
    }

    return rc;
}

int variorum_get_num_cores(void)
{
    int num_cores = -1;
    int rc = -1;

    rc = variorum_init_topology();

    if (rc == 0)
    {
        num_cores = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_CORE);
        return num_cores;
    }

    return rc;
}

int variorum_get_num_threads(void)
{
    int num_threads = -1;
    int rc = -1;

    rc = variorum_init_topology();

    if (rc == 0)
    {
        num_threads = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PU);
        return num_threads;
    }

    return rc;
}
