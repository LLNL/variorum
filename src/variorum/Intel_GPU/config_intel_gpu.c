// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_intel_gpu.h>
#include <config_architecture.h>
#include <variorum_error.h>

uint64_t *detect_intel_gpu_arch(void)
{
    uint64_t  *model = (uint64_t *)malloc(sizeof(uint64_t));
    *model = 1;
    return model;   // implement the arch detection later if needed
}

int set_intel_gpu_func_ptrs(int idx)
{
    int err = 0;

    if (*g_platform[idx].arch_id == 1)
    {
        g_platform[idx].variorum_print_power = intel_gpu_get_power;
        g_platform[idx].variorum_print_thermals = intel_gpu_get_thermals;
        g_platform[idx].variorum_print_frequency = intel_gpu_get_clocks;
        g_platform[idx].variorum_cap_each_gpu_power_limit =
            intel_gpu_cap_each_gpu_power_limit;
        g_platform[idx].variorum_print_power_limit = intel_gpu_get_power_limit;
        g_platform[idx].variorum_print_energy = intel_gpu_get_energy;
        g_platform[idx].variorum_get_energy_json = intel_gpu_get_energy_json;
    }
    else
    {
        err = VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    initAPMIDG();
    return err;
}
