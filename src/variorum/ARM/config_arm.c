// Copyright 2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_arm.h>
#include <config_architecture.h>
#include <power_features.h>
#include <ARM.h>
#include <variorum_error.h>

uint64_t *detect_arm_arch(void)
{
    // ARM systems come in various flavors. For now, we will use a default model number.
    uint64_t *model = (uint64_t *) malloc(sizeof(uint64_t));
    *model = ARMV8;
    return model;
}

int set_arm_func_ptrs(void)
{
    int err = 0;

    if (*g_platform.arm_arch == ARMV8)
    {
        /* Initialize interfaces */
        g_platform.variorum_dump_power                      = arm_get_power;
        g_platform.variorum_dump_thermals                   = arm_get_thermals;
        g_platform.variorum_dump_clocks                     = arm_get_clocks;
        g_platform.variorum_print_available_frequencies     = arm_get_frequencies;
        g_platform.variorum_set_socket_frequency            = arm_set_socket_frequency;
    }
    else
    {
        err = VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    init_arm();
    return err;
}
