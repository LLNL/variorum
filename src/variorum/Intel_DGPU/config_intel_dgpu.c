// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_intel_dgpu.h>
#include <config_architecture.h>
#include <power_features.h>
#include <variorum_error.h>
#include <DGPU.h>

uint64_t *detect_intel_dgpu_arch(void)
{
    uint64_t  *model = (uint64_t *)malloc(sizeof(uint64_t));
    *model = 1;
    return model;   // implement the arch detection later if needed
}

int set_intel_dgpu_func_ptrs(void)
{
    int err = 0;

    if (*g_platform.intel_dgpu_arch == 1)
    {
        g_platform.variorum_print_power           = intel_dgpu_get_power;
        g_platform.variorum_print_thermals        = intel_dgpu_get_thermals;
        g_platform.variorum_print_frequency       = intel_dgpu_get_clocks;
        // g_platform.variorum_print_power_limit    = ; // implement this later
    }
    else
    {
        err = VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    initAPMIDG();
    return err;
}
