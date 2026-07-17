// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <config_amd_apu.h>
#include <config_architecture.h>
#include <amd_apu_power_features.h>
#include <mi300aAPU.h>
#include <variorum_error.h>

#include <rocm_smi/rocm_smi.h>

uint64_t *detect_amd_apu_arch(void)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    uint64_t *model = (uint64_t *) malloc(sizeof(uint64_t));

    // Default to MI300A
    *model = AMD_MI300A;

    ret = rsmi_init(0);
    if (ret != RSMI_STATUS_SUCCESS)
    {
        // If RSMI fails, return MI300A as default
        return model;
    }

    ret = rsmi_num_monitor_devices(&num_devices);
    if (ret != RSMI_STATUS_SUCCESS || num_devices == 0)
    {
        rsmi_shut_down();
        return model;
    }

    // Check first device for MI300A identification
    char name[256];
    ret = rsmi_dev_name_get(0, name, 256);

    if (ret == RSMI_STATUS_SUCCESS)
    {
        // Check if device name contains "MI300A" or "MI300"
        // Convert to uppercase for case-insensitive comparison
        char name_upper[256];
        for (int i = 0; name[i] && i < 255; i++)
        {
            name_upper[i] = (name[i] >= 'a' && name[i] <= 'z') ?
                            name[i] - 32 : name[i];
            name_upper[i + 1] = '\0';
        }

        if (strstr(name_upper, "MI300A") != NULL || strstr(name_upper, "MI300") != NULL)
        {
            *model = AMD_MI300A;
        }
    }

    rsmi_shut_down();
    return model;
}

int set_amd_apu_func_ptrs(int idx)
{
    int err = 0;

    if (*g_platform[idx].arch_id == AMD_MI300A)
    {
        /* Initialize monitoring interfaces */
        g_platform[idx].variorum_print_power = amd_apu_mi300a_get_power;
        g_platform[idx].variorum_print_thermals = amd_apu_mi300a_get_thermals;
        g_platform[idx].variorum_print_frequency = amd_apu_mi300a_get_clocks;
        g_platform[idx].variorum_print_power_limit = amd_apu_mi300a_get_power_limit;
        g_platform[idx].variorum_print_gpu_utilization =
            amd_apu_mi300a_get_gpu_utilization;

        /* Initialize JSON interfaces */
        g_platform[idx].variorum_get_thermals_json = amd_apu_mi300a_get_thermals_json;
        g_platform[idx].variorum_get_frequency_json = amd_apu_mi300a_get_clocks_json;
        g_platform[idx].variorum_get_utilization_json =
            amd_apu_mi300a_get_gpu_utilization_json;
        g_platform[idx].variorum_get_power_json = amd_apu_mi300a_get_power_json;

        /* Initialize control interfaces */
        g_platform[idx].variorum_cap_each_gpu_power_limit =
            amd_apu_mi300a_cap_each_gpu_power_limit;

        /* Initialize ENERGY interfaces - NEW FOR MI300A */
        g_platform[idx].variorum_print_energy = amd_apu_mi300a_get_energy;
        g_platform[idx].variorum_get_energy_json = amd_apu_mi300a_get_energy_json;
    }
    else
    {
        err = VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    return err;
}
