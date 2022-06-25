// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <variorum_topology.h>


int variorum_init_topology(void)
{
    int rc = -1;

    rc = hwloc_topology_init(&topology);

    if (rc == 0)
    {
        rc = hwloc_topology_load(topology);
    }

    return rc;
}

void variorum_destroy_topology(void)
{
    hwloc_topology_destroy(topology);
}

int variorum_get_num_sockets(void)
{
    int num_sockets = -1; 
    int rc = -1;

    rc = variorum_init_topology();

    if (rc == 0)
    {
        num_sockets = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_SOCKET);
        variorum_destroy_topology();
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
        variorum_destroy_topology();
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
        variorum_destroy_topology();
        return num_threads;
    }

    return rc;
}
