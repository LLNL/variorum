// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_ibm.h>
#include <config_architecture.h>
#include <Power9.h>
#include <variorum_error.h>

uint64_t *detect_ibm_arch(void)
{
    // Power8 is unimplemented. This is the current hack for Power9.
    uint64_t *model = (uint64_t *) malloc(sizeof(uint64_t));
    *model = 9;
    return model;
}

int set_ibm_func_ptrs(void)
{
    int err = 0;

    if (*g_platform.ibm_arch == POWER9)
    {
        g_platform.dump_power = p9_get_power;
        g_platform.dump_power_limits = p9_get_power_limits;
        g_platform.set_node_power_limit = p9_set_node_power_limit;
        g_platform.set_each_socket_power_limit = p9_set_socket_power_limit;
        g_platform.set_gpu_power_ratio = p9_set_gpu_power_ratio;
        g_platform.set_and_verify_node_power_limit = p9_set_and_verify_node_power_limit;
        g_platform.monitoring = p9_monitoring;
    }
    else
    {
        err = VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    return err;
}
