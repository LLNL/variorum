// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <instinctGPU.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <power_features.h>

int instinctGPU_get_power(int verbose)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    int iter = 0;
    int nsockets;

    variorum_get_topology(&nsockets, NULL, NULL);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}


int instinctGPU_get_power_limit(int verbose)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    int iter = 0;
    int nsockets;

    variorum_get_topology(&nsockets, NULL, NULL);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_limit_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}


int instinctGPU_get_thermals(int verbose)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    int iter = 0;
    int nsockets;

    variorum_get_topology(&nsockets, NULL, NULL);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_thermals_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}

int instinctGPU_get_clocks(int verbose)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    int iter = 0;
    int nsockets;

    variorum_get_topology(&nsockets, NULL, NULL);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_clocks_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}

int instinctGPU_get_gpu_utilization(int verbose)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    int iter = 0;
    int nsockets;

    variorum_get_topology(&nsockets, NULL, NULL);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_gpu_utilization_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}
