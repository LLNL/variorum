// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_nvidia.h>
#include <config_architecture.h>
#include <power_features.h>
#include <Volta.h>
#include <variorum_error.h>

uint64_t *detect_gpu_arch(void)
{
    // Don't know what the architecture scheme is for Nvidia. This is the current hack for Volta.
    uint64_t *model = (uint64_t *) malloc(sizeof(uint64_t));
    *model = 1;
    return model;
}

int set_nvidia_func_ptrs(void)
{
    int err = 0;

    if (*g_platform.nvidia_arch == VOLTA)
    {
        /* Initialize monitoring interfaces */
        g_platform.variorum_print_power           = volta_get_power;
        g_platform.variorum_print_thermals        = volta_get_thermals;
        g_platform.variorum_print_frequency       = volta_get_clocks;
        g_platform.variorum_print_power_limit     = volta_get_power_limits;
        g_platform.variorum_print_gpu_utilization = volta_get_gpu_utilization;
    }
    else
    {
        err = VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    initNVML();
    return err;
}
