// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_arm.h>
#include <config_architecture.h>
#include <power_features.h>
#include <power_features.h>
#include <ARM_Juno_r2.h>
#include <ARM_Neoverse_N1.h>
#include <variorum_error.h>

uint64_t *detect_arm_arch(void)
{
    // ARM systems come in various flavors. For now, we will use a default model number.
    uint64_t *model = (uint64_t *) malloc(sizeof(uint64_t));
    asm volatile(
        "mrs %0, MIDR_EL1"
        : "=r"(*model));
    return model;
}

int set_arm_func_ptrs(int idx)
{
    int err = 0;

    if (*g_platform[idx].arch_id == ARM_JUNO_R2)
    {
        /* Initialize interfaces */
        g_platform[idx].variorum_print_power                     =
            arm_juno_r2_get_power;
        g_platform[idx].variorum_print_thermals                  =
            arm_juno_r2_get_thermals;
        g_platform[idx].variorum_print_frequency                 =
            arm_juno_r2_get_clocks;
        g_platform[idx].variorum_print_available_frequencies     =
            arm_juno_r2_get_frequencies;
        g_platform[idx].variorum_cap_socket_frequency_limit      =
            arm_juno_r2_cap_socket_frequency;
        g_platform[idx].variorum_get_node_power_json             =
            arm_juno_r2_get_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            arm_juno_r2_get_power_domain_info_json;
    }
    else if (*g_platform[idx].arch_id == ARM_NEOVERSE_N1)
    {
        /* Initialize interfaces */
        g_platform[idx].variorum_print_power                     =
            arm_neoverse_n1_get_power;
        g_platform[idx].variorum_print_thermals                  =
            arm_neoverse_n1_get_thermals;
        g_platform[idx].variorum_print_frequency                 =
            arm_neoverse_n1_get_clocks;
        g_platform[idx].variorum_print_available_frequencies     =
            arm_neoverse_n1_get_frequencies;
        g_platform[idx].variorum_cap_socket_frequency_limit      =
            arm_neoverse_n1_cap_socket_frequency;
        g_platform[idx].variorum_get_node_power_json             =
            arm_neoverse_n1_get_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            arm_neoverse_n1_get_power_domain_info_json;

    }
    else
    {
        err = VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    init_arm();
    return err;
}
