// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
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
    int ret = 0;

    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    ret = get_power_data(long_ver, stdout);

    return ret;
}

int arm_get_thermals(int long_ver)
{
    int ret = 0;

    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    ret = get_thermal_data(long_ver, stdout);

    return ret;
}

int arm_get_clocks(int long_ver)
{
    int ret = 0;
    unsigned iter = 0;
    unsigned nsockets;

    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        ret = get_clocks_data(iter, long_ver, stdout);
    }

    return ret;
}

int arm_get_frequencies(void)
{
    int ret = 0;
    unsigned iter = 0;
    unsigned nsockets;

    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    variorum_get_topology(&nsockets, NULL, NULL);
    for (iter = 0; iter < nsockets; iter++)
    {
        ret = get_frequencies(iter, stdout);
    }

    return ret;
}

int arm_cap_socket_frequency(int cpuid, int freq)
{
    int ret = 0;
    unsigned nsockets;

    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    variorum_get_topology(&nsockets, NULL, NULL);
    if (cpuid < 0 || cpuid >= (int)nsockets)
    {
        fprintf(stdout, "The specified CPU ID does not exist\n");
        return -1;
    }
    ret = cap_socket_frequency(cpuid, freq);

    return ret;
}

int arm_get_power_json(char **get_power_obj_str)
{
    int ret = 0;

    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    json_t *get_power_obj = json_object();

    ret = json_get_power_data(get_power_obj);

    *get_power_obj_str = json_dumps(get_power_obj, 0);
    json_decref(get_power_obj);

    return ret;
}

int arm_get_power_domain_info_json(char **get_domain_obj_str)
{
    int ret = 0;

    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    json_t *get_domain_obj = json_object();

    ret = json_get_power_domain_info(get_domain_obj);

    *get_domain_obj_str = json_dumps(get_domain_obj, 0);
    json_decref(get_domain_obj);

    return ret;
}
