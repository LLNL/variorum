// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <instinctGPU.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <amd_gpu_power_features.h>

int amd_gpu_instinct_get_power(int verbose)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;

    variorum_get_topology(&nsockets, NULL, NULL, P_AMD_GPU_IDX);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}


int amd_gpu_instinct_get_power_limit(int verbose)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;

    variorum_get_topology(&nsockets, NULL, NULL, P_AMD_GPU_IDX);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_limit_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}


int amd_gpu_instinct_get_thermals(int verbose)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;

    variorum_get_topology(&nsockets, NULL, NULL, P_AMD_GPU_IDX);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_thermals_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}

int amd_gpu_instinct_get_clocks(int verbose)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;

    variorum_get_topology(&nsockets, NULL, NULL, P_AMD_GPU_IDX);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_clocks_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}

int amd_gpu_instinct_get_gpu_utilization(int verbose)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;

    variorum_get_topology(&nsockets, NULL, NULL, P_AMD_GPU_IDX);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_gpu_utilization_data(iter, nsockets, verbose, stdout);
    }
    return 0;
}

int amd_gpu_instinct_cap_each_gpu_power_limit(unsigned int powerlimit)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL, P_AMD_GPU_IDX);
    for (iter = 0; iter < nsockets; iter++)
    {
        cap_each_gpu_power_limit(iter, nsockets, powerlimit);
    }
    return 0;
}
