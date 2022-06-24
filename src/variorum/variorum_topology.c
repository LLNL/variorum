// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <hwloc.h>


int variorum_get_num_sockets(void)
{
    hwloc_topology_t topology;
    int rc;

    rc = hwloc_topology_init(&topology);
    if (rc != 0)
    {
        exit(-1);
    }
    rc = hwloc_topology_load(topology);
    if (rc != 0)
    {
        exit(-1);
    }
    return hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_SOCKET);
}

int variorum_get_num_cores(void)
{
    hwloc_topology_t topology;
    int rc;

    rc = hwloc_topology_init(&topology);
    if (rc != 0)
    {
        exit(-1);
    }
    rc = hwloc_topology_load(topology);
    if (rc != 0)
    {
        exit(-1);
    }
    return hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_CORE);
}

int variorum_get_num_threads(void)
{
    hwloc_topology_t topology;
    int rc;

    rc = hwloc_topology_init(&topology);
    if (rc != 0)
    {
        exit(-1);
    }
    rc = hwloc_topology_load(topology);
    if (rc != 0)
    {
        exit(-1);
    }
    return hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PU);
}
