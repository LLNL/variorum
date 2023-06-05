// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_arm.h>
#include <config_architecture.h>
#include "power_features_Juno_r2.h"
#include "power_features_Neoverse_N1.h"
#include <ARM_Juno_r2.h>
#include <ARM_Neoverse_N1.h>
#include <variorum_error.h>
#include <fcntl.h>
#include <unistd.h>

#define CPU_ID_SIZE 64

uint64_t *detect_arm_arch(void)
{
    char cpu0_id_str[CPU_ID_SIZE];
    char cpu1_id_str[CPU_ID_SIZE];
    uint64_t cpu0_id_val;
    uint64_t cpu1_id_val;

    /* This logic reads the IDs for the first two cores on the system.
     * On the ARM big.LITTLE system, the first two cores report the CPU IDs
     * from the big and LITTLE clusters, respectively, on the SoC.
     * On the Neoverse N1 system, the first two cores report identical
     * CPU IDs. Variorum checks the combined CPU IDs returned by the system.
     */
    char *cpu0_id_reg_path =
        "/sys/devices/system/cpu/cpu0/regs/identification/midr_el1";
    char *cpu1_id_reg_path =
        "/sys/devices/system/cpu/cpu1/regs/identification/midr_el1";
    unsigned long *model = (unsigned long *) malloc(sizeof(uint64_t));

    int cpu0_id_reg_fd = open(cpu0_id_reg_path, O_RDONLY);
    int cpu1_id_reg_fd = open(cpu1_id_reg_path, O_RDONLY);

    if (!cpu0_id_reg_fd || !cpu1_id_reg_fd)
    {
        variorum_error_handler("Error encountered in accessing CPU ID information",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }

    int cpu0_id_bytes = read(cpu0_id_reg_fd, cpu0_id_str, CPU_ID_SIZE);
    int cpu1_id_bytes = read(cpu1_id_reg_fd, cpu1_id_str, CPU_ID_SIZE);

    if (!cpu0_id_bytes || !cpu1_id_bytes)
    {
        variorum_error_handler("Error encountered in accessing CPU ID information",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }

    cpu0_id_val = strtol(cpu0_id_str, NULL, 16);
    cpu1_id_val = strtol(cpu1_id_str, NULL, 16);

    *model = (cpu0_id_val & 0x000000000000fff0) << 8;
    *model |= ((cpu1_id_val & 0x000000000000fff0) >> 4);

    close(cpu0_id_reg_fd);
    close(cpu1_id_reg_fd);

    return model;
}

int set_arm_func_ptrs(int idx)
{
    int err = 0;

    if (*g_platform[idx].arch_id == ((ARM_CORTEX_A53 << 12) | ARM_CORTEX_A72))
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
    else if (*g_platform[idx].arch_id == ((ARM_NEOVERSE_N1 << 12) |
                                          ARM_NEOVERSE_N1))
    {
        /* Initialize interfaces */
        g_platform[idx].variorum_print_power                     =
            arm_neoverse_n1_get_power;
        g_platform[idx].variorum_print_thermals                  =
            arm_neoverse_n1_get_thermals;
        g_platform[idx].variorum_print_frequency                 =
            arm_neoverse_n1_get_clocks;
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
