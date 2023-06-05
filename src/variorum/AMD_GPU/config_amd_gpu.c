// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_amd_gpu.h>
#include <config_architecture.h>
#include <power_features.h>
#include <instinctGPU.h>
#include <variorum_error.h>

uint64_t *detect_amd_gpu_arch(void)
{
    // Don't know what the architecture scheme is for AMD GPUs.
    uint64_t *model = (uint64_t *) malloc(sizeof(uint64_t));
    *model = 1;
    return model;
}

int set_amd_gpu_func_ptrs(int idx)
{
    int err = 0;

    if (*g_platform[idx].arch_id == AMD_INSTINCT)
    {
        /* Initialize monitoring interfaces */
        g_platform[idx].variorum_print_power           = amd_gpu_instinct_get_power;
        g_platform[idx].variorum_print_thermals        = amd_gpu_instinct_get_thermals;
        g_platform[idx].variorum_print_frequency       = amd_gpu_instinct_get_clocks;
        g_platform[idx].variorum_print_power_limit     =
            amd_gpu_instinct_get_power_limit;
        g_platform[idx].variorum_print_gpu_utilization =
            amd_gpu_instinct_get_gpu_utilization;
        /* Initialize control interfaces */
        g_platform[idx].variorum_cap_each_gpu_power_limit   =
            amd_gpu_instinct_cap_each_gpu_power_limit;
    }
    else
    {
        err = VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    return err;
}
