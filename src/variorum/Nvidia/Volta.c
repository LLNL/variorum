// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
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
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    dump_power_data(stdout, long_ver);
    return 0;
}

int volta_get_thermals(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    dump_thermal_data(stdout, long_ver);
    return 0;
}

int volta_get_clocks(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    dump_clocks_data(stdout, long_ver);
    return 0;
}

int volta_get_power_limits(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    dump_power_limits(stdout, long_ver);
    return 0;
}

int volta_get_gpu_utilization(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    dump_gpu_utilization(stdout, long_ver);
    return 0;
}
