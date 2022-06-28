// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum_topology.h>

int main()
{
    int num_sockets, num_cores, num_threads;

    // Demonstrates usage of Variorum's advanced API for obtaining topology information.
    printf("This example demonstrates the use of variorum_topology.h API \n"
           "to obtain basic information from the underlying architecture.\n");

    num_sockets =  variorum_get_num_sockets();

    if (num_sockets > 0)
    {
        printf("Number of sockets on this hardware: %d\n", num_sockets);
    }
    else
    {
        printf("HWLOC returned an invalid number of sockets (<=0)."
               "Variorum does not handle this case. \n");
    }

    num_cores = variorum_get_num_cores();

    if (num_cores > 0)
    {
        printf("Number of cores/socket on this hardware: %d\n", num_cores);
    }
    else
    {
        printf("HWLOC returned an invalid number of cores (<=0)."
               "Variorum does not handle this case. \n");
    }

    num_threads = variorum_get_num_threads();

    if (num_threads > 0)
    {
        printf("Number of threads/socket on this hardware: %d\n", num_threads);
    }
    else
    {
        printf("HWLOC returned an invalid number of threads (<=0)."
               "Variorum does not handle this case. \n");
    }
}
