// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <Volta.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <nvidia_gpu_power_features.h>

int volta_get_power(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets = 0;
#ifdef VARIORUM_WITH_NVIDIA_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_NVIDIA_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        nvidia_gpu_get_power_data(iter, long_ver, stdout);
    }
    return 0;
}

int volta_get_thermals(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets = 0;
#ifdef VARIORUM_WITH_NVIDIA_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_NVIDIA_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        nvidia_gpu_get_thermal_data(iter, long_ver, stdout);
    }
    return 0;
}

int volta_get_clocks(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets = 0;
#ifdef VARIORUM_WITH_NVIDIA_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_NVIDIA_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        nvidia_gpu_get_clocks_data(iter, long_ver, stdout);
    }
    return 0;
}

int volta_get_power_limits(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets = 0;
#ifdef VARIORUM_WITH_NVIDIA_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_NVIDIA_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        nvidia_gpu_get_power_limits_data(iter, long_ver, stdout);
    }
    return 0;
}

int volta_get_gpu_utilization(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets = 0;
#ifdef VARIORUM_WITH_NVIDIA_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_NVIDIA_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        nvidia_gpu_get_gpu_utilization_data(iter, long_ver, stdout);
    }
    return 0;
}

int volta_cap_each_gpu_power_limit(unsigned int powerlimit)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets = 0;
#ifdef VARIORUM_WITH_NVIDIA_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_NVIDIA_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        cap_each_gpu_power_limit(iter, powerlimit);
    }
    return 0;
}
