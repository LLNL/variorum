// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <GPU.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <intel_gpu_power_features.h>

int intel_gpu_get_power(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    unsigned iter = 0;
    unsigned nsockets;
#ifdef VARIORUM_WITH_INTEL_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_INTEL_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_data(iter, long_ver, stdout);
    }
    return 0;
}

int intel_gpu_get_thermals(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    unsigned iter = 0;
    unsigned nsockets;
#ifdef VARIORUM_WITH_INTEL_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_INTEL_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        get_thermal_data(iter, long_ver, stdout);
    }
    return 0;
}

int intel_gpu_get_clocks(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    unsigned iter = 0;
    unsigned nsockets;
#ifdef VARIORUM_WITH_INTEL_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_INTEL_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        get_clocks_data(iter, long_ver, stdout);
    }
    return 0;
}

int intel_gpu_cap_each_gpu_power_limit(unsigned int powerlimit)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;
#ifdef VARIORUM_WITH_INTEL_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_INTEL_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        cap_each_gpu_power_limit(iter, powerlimit);
    }
    return 0;
}

int intel_gpu_get_power_limit(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    unsigned iter = 0;
    unsigned nsockets;
#ifdef VARIORUM_WITH_INTEL_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_INTEL_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_limit_data(iter, long_ver, stdout);
    }
    return 0;
}

int intel_gpu_get_energy(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets = 0;
#ifdef VARIORUM_WITH_INTEL_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_INTEL_GPU_IDX);
#endif
    for (iter = 0; iter < nsockets; iter++)
    {
        get_energy_data(iter, long_ver, stdout);
    }
    return 0;
}

int intel_gpu_get_energy_json(json_t *get_energy_obj)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL, P_INTEL_GPU_IDX);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_energy_json(iter, get_energy_obj);
    }

    return 0;
}
