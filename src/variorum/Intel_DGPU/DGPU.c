// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <DGPU.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <power_features.h>

int intel_dgpu_get_power(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_data(iter, long_ver, stdout);
    }
    return 0;
}

int intel_dgpu_get_thermals(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        get_thermal_data(iter, long_ver, stdout);
    }
    return 0;
}

int intel_dgpu_get_clocks(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        get_clocks_data(iter, long_ver, stdout);
    }
    return 0;
}
