// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>

#include <Volta.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <nvidia_gpu_power_features.h>
#include <jansson.h>

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

int volta_get_thermals_json(json_t *get_thermal_obj)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL, P_NVIDIA_GPU_IDX);

    for (iter = 0; iter < nsockets; iter++)
    {
        nvidia_gpu_get_thermal_json(iter, get_thermal_obj);
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

int volta_get_clocks_json(json_t *get_clock_obj_json)
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
        nvidia_gpu_get_clocks_json(iter, get_clock_obj_json);
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

int volta_get_gpu_utilization_json(char **get_gpu_util_obj_str)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    json_t *get_util_obj = json_object();
    unsigned iter = 0;
    unsigned nsockets;
#ifdef VARIORUM_WITH_NVIDIA_GPU
    variorum_get_topology(&nsockets, NULL, NULL, P_NVIDIA_GPU_IDX);
#endif

    for (iter = 0; iter < nsockets; iter++)
    {
        nvidia_get_gpu_utilization_json(iter, get_util_obj);
    }
    *get_gpu_util_obj_str = json_dumps(get_util_obj, JSON_INDENT(4));
    json_decref(get_util_obj);
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

int volta_get_power_json(json_t *get_power_obj)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL, P_NVIDIA_GPU_IDX);

    for (iter = 0; iter < nsockets; iter++)
    {
        nvidia_gpu_get_power_json(iter, get_power_obj);
    }

    return 0;
}

