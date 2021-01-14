// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <ARM.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <power_features.h>

int arm_get_power(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    dump_power_data(long_ver, stdout);
    return 0;
}

int arm_get_thermals(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    dump_thermal_data(long_ver, stdout);
    return 0;
}

int arm_get_clocks(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        dump_clocks_data(iter, long_ver, stdout);
    }
    return 0;
}

int arm_get_frequencies(void)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        dump_frequencies(iter, stdout);
    }
    return 0;
}
