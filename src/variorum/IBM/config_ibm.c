// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
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

int set_ibm_func_ptrs(int idx)
{
    int err = 0;

    if (*g_platform[idx].arch_id == POWER9)
    {
        g_platform[idx].variorum_print_power = ibm_cpu_p9_get_power;
        g_platform[idx].variorum_print_power_limit = ibm_cpu_p9_get_power_limits;
        g_platform[idx].variorum_cap_best_effort_node_power_limit =
            ibm_cpu_p9_cap_and_verify_node_power_limit;
        g_platform[idx].variorum_cap_each_socket_power_limit =
            ibm_cpu_p9_cap_socket_power_limit;
        g_platform[idx].variorum_cap_gpu_power_ratio = ibm_cpu_p9_cap_gpu_power_ratio;
        g_platform[idx].variorum_monitoring = ibm_cpu_p9_monitoring;
        g_platform[idx].variorum_get_node_power_json = ibm_cpu_p9_get_node_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            ibm_cpu_p9_get_node_power_domain_info_json;
    }
    else
    {
        err = VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    return err;
}
