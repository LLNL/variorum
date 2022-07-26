// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <Volta.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <power_features.h>

int volta_get_power(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && strnlen(val) > 0)
    {
        printf("Running %s\n", __FUNCTION__);
    }
    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_data(iter, long_ver, stdout);
    }
    return 0;
}

int volta_get_thermals(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && strnlen(val) > 0)
    {
        printf("Running %s\n", __FUNCTION__);
    }
    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        get_thermal_data(iter, long_ver, stdout);
    }
    return 0;
}

int volta_get_clocks(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && strnlen(val) > 0)
    {
        printf("Running %s\n", __FUNCTION__);
    }
    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        get_clocks_data(iter, long_ver, stdout);
    }
    return 0;
}

int volta_get_power_limits(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && strnlen(val) > 0)
    {
        printf("Running %s\n", __FUNCTION__);
    }
    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_limits(iter, long_ver, stdout);
    }
    return 0;
}

int volta_get_gpu_utilization(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && strnlen(val) > 0)
    {
        printf("Running %s\n", __FUNCTION__);
    }
    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        get_gpu_utilization(iter, long_ver, stdout);
    }
    return 0;
}
