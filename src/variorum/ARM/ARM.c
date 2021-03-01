// Copyright 2021 Lawrence Livermore National Security, LLC and other
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
    int ret;
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    ret = dump_power_data(long_ver, stdout);
    return ret;
}

int arm_get_thermals(int long_ver)
{
    int ret;
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    ret = dump_thermal_data(long_ver, stdout);
    return ret;
}

int arm_get_clocks(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    unsigned iter = 0;
    unsigned nsockets;
    int ret;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        ret = dump_clocks_data(iter, long_ver, stdout);
    }
    return ret;
}

int arm_get_frequencies(void)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    unsigned iter = 0;
    unsigned nsockets;
    int ret;
    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        ret = dump_frequencies(iter, stdout);
    }
    return ret;
}

int arm_set_socket_frequency(int cpuid, int freq)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    unsigned iter = 0;
    unsigned nsockets;
    variorum_get_topology(&nsockets, NULL, NULL);
    if (cpuid < 0 || cpuid >= nsockets)
    {
        fprintf(stdout, "The specified CPU ID does not exist\n");
        return -1;
    }
    int ret = set_socket_frequency(cpuid, freq);
    return ret;
}
