// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_intel.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <Intel_06_2A.h>
#include <Intel_06_2D.h>
#include <Intel_06_3E.h>
#include <Intel_06_4F.h>
#include <Intel_06_3F.h>
#include <Intel_06_9E.h>
#include <Intel_06_55.h>
#include <Intel_06_6A.h>
#include <Intel_06_8F.h>

uint64_t *detect_intel_arch(void)
{
    uint64_t rax = 1;
    uint64_t rbx = 0;
    uint64_t rcx = 0;
    uint64_t rdx = 0;
    uint64_t *model = (uint64_t *) malloc(sizeof(uint64_t));

    asm volatile(
        "cpuid"
        : "=a"(rax), "=b"(rbx), "=c"(rcx), "=d"(rdx)
        : "0"(rax), "2"(rcx));

    *model = ((rax >> 12) & 0xF0) | ((rax >> 4) & 0xF);
    return model;

    // return family and model appended
    //return (((rax >> 28) & 0x0F)+((rax >> 8) & 0x0F) << 8) | ((rax >> 12) & 0xF0)+((rax >> 4) & 0xF);
}

int gpu_power_ratio_unimplemented(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }
    if (long_ver == 0 || long_ver == 1)
    {
        variorum_error_handler("GPU power ratio is unavailable on Intel platforms",
                               VARIORUM_ERROR_FEATURE_NOT_AVAILABLE,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
    }
    return 0;
}

/* 02/25/2019 SB
 *    If implementation is identical, have function pointer use the same function.
 *    If it is different, implement a new function.
 */
int set_intel_func_ptrs(int idx)
{
    int err = 0;

    // GPU power ratio doesn't apply to Intel architectures.
    // So specify a common function that exits with a log message.
    g_platform[idx].variorum_cap_gpu_power_ratio = gpu_power_ratio_unimplemented;

    // Sandy Bridge 06_2A
    if (*g_platform[idx].arch_id == FM_06_2A)
    {
        g_platform[idx].variorum_print_power_limit =
            intel_cpu_fm_06_2a_get_power_limits;
        g_platform[idx].variorum_cap_each_socket_power_limit =
            intel_cpu_fm_06_2a_cap_power_limits;
        g_platform[idx].variorum_print_features = intel_cpu_fm_06_2a_get_features;
        g_platform[idx].variorum_print_thermals = intel_cpu_fm_06_2a_get_thermals;
        g_platform[idx].variorum_print_counters = intel_cpu_fm_06_2a_get_counters;
        g_platform[idx].variorum_print_frequency = intel_cpu_fm_06_2a_get_clocks;
        g_platform[idx].variorum_print_power = intel_cpu_fm_06_2a_get_power;
        g_platform[idx].variorum_print_turbo = intel_cpu_fm_06_2a_get_turbo_status;
        g_platform[idx].variorum_enable_turbo = intel_cpu_fm_06_2a_enable_turbo;
        g_platform[idx].variorum_disable_turbo = intel_cpu_fm_06_2a_disable_turbo;
        g_platform[idx].variorum_poll_power = intel_cpu_fm_06_2a_poll_power;
        g_platform[idx].variorum_monitoring = intel_cpu_fm_06_2a_monitoring;
        //g_platform[idx].variorum_cap_each_core_frequency =
        //    intel_cpu_fm_06_2a_cap_frequency;
        g_platform[idx].variorum_get_node_power_json =
            intel_cpu_fm_06_2a_get_node_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            intel_cpu_fm_06_2a_get_node_power_domain_info_json;
        g_platform[idx].variorum_cap_best_effort_node_power_limit =
            intel_cpu_fm_06_2a_cap_best_effort_node_power_limit;
        g_platform[idx].variorum_print_available_frequencies =
            intel_cpu_fm_06_2a_get_frequencies;
        g_platform[idx].variorum_get_frequency_json =
            intel_cpu_fm_06_2a_get_clocks_json;
    }
    else if (*g_platform[idx].arch_id == FM_06_2D)
    {
        g_platform[idx].variorum_print_power_limit =
            intel_cpu_fm_06_2d_get_power_limits;
        g_platform[idx].variorum_cap_each_socket_power_limit =
            intel_cpu_fm_06_2d_cap_power_limits;
        g_platform[idx].variorum_print_features = intel_cpu_fm_06_2d_get_features;
        g_platform[idx].variorum_print_thermals = intel_cpu_fm_06_2d_get_thermals;
        g_platform[idx].variorum_print_counters = intel_cpu_fm_06_2d_get_counters;
        g_platform[idx].variorum_print_frequency = intel_cpu_fm_06_2d_get_clocks;
        g_platform[idx].variorum_print_power = intel_cpu_fm_06_2d_get_power;
        g_platform[idx].variorum_print_turbo = intel_cpu_fm_06_2d_get_turbo_status;
        g_platform[idx].variorum_enable_turbo = intel_cpu_fm_06_2d_enable_turbo;
        g_platform[idx].variorum_disable_turbo = intel_cpu_fm_06_2d_disable_turbo;
        g_platform[idx].variorum_poll_power = intel_cpu_fm_06_2d_poll_power;
        g_platform[idx].variorum_monitoring = intel_cpu_fm_06_2d_monitoring;
        //g_platform[idx].variorum_cap_each_core_frequency =
        //    intel_cpu_fm_06_2d_cap_frequency;
        g_platform[idx].variorum_get_node_power_json =
            intel_cpu_fm_06_2d_get_node_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            intel_cpu_fm_06_2d_get_node_power_domain_info_json;
        g_platform[idx].variorum_cap_best_effort_node_power_limit =
            intel_cpu_fm_06_2d_cap_best_effort_node_power_limit;
        g_platform[idx].variorum_print_available_frequencies =
            intel_cpu_fm_06_2d_get_frequencies;
        g_platform[idx].variorum_get_frequency_json =
            intel_cpu_fm_06_2d_get_clocks_json;
    }
    // Ivy Bridge 06_3E
    else if (*g_platform[idx].arch_id == FM_06_3E)
    {
        g_platform[idx].variorum_print_power_limit =
            intel_cpu_fm_06_3e_get_power_limits;
        g_platform[idx].variorum_cap_each_socket_power_limit =
            intel_cpu_fm_06_3e_cap_power_limits;
        g_platform[idx].variorum_print_features = intel_cpu_fm_06_3e_get_features;
        g_platform[idx].variorum_print_thermals = intel_cpu_fm_06_3e_get_thermals;
        g_platform[idx].variorum_print_counters = intel_cpu_fm_06_3e_get_counters;
        g_platform[idx].variorum_print_frequency = intel_cpu_fm_06_3e_get_clocks;
        g_platform[idx].variorum_print_power = intel_cpu_fm_06_3e_get_power;
        g_platform[idx].variorum_print_turbo = intel_cpu_fm_06_3e_get_turbo_status;
        g_platform[idx].variorum_enable_turbo = intel_cpu_fm_06_3e_enable_turbo;
        g_platform[idx].variorum_disable_turbo = intel_cpu_fm_06_3e_disable_turbo;
        g_platform[idx].variorum_poll_power = intel_cpu_fm_06_3e_poll_power;
        g_platform[idx].variorum_monitoring = intel_cpu_fm_06_3e_monitoring;
        //g_platform[idx].variorum_cap_each_core_frequency =
        //    intel_cpu_fm_06_3e_cap_frequency;
        g_platform[idx].variorum_get_node_power_json =
            intel_cpu_fm_06_3e_get_node_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            intel_cpu_fm_06_3e_get_node_power_domain_info_json;
        g_platform[idx].variorum_cap_best_effort_node_power_limit =
            intel_cpu_fm_06_3e_cap_best_effort_node_power_limit;
        g_platform[idx].variorum_print_available_frequencies =
            intel_cpu_fm_06_3e_get_frequencies;
        g_platform[idx].variorum_get_frequency_json =
            intel_cpu_fm_06_3e_get_clocks_json;
    }
    // Haswell 06_3F
    else if (*g_platform[idx].arch_id == FM_06_3F)
    {
        g_platform[idx].variorum_print_power_limit =
            intel_cpu_fm_06_3f_get_power_limits;
        g_platform[idx].variorum_cap_each_socket_power_limit =
            intel_cpu_fm_06_3f_cap_power_limits;
        g_platform[idx].variorum_print_features = intel_cpu_fm_06_3f_get_features;
        g_platform[idx].variorum_print_thermals = intel_cpu_fm_06_3f_get_thermals;
        g_platform[idx].variorum_print_counters = intel_cpu_fm_06_3f_get_counters;
        g_platform[idx].variorum_print_frequency = intel_cpu_fm_06_3f_get_clocks;
        g_platform[idx].variorum_print_power = intel_cpu_fm_06_3f_get_power;
        g_platform[idx].variorum_print_turbo = intel_cpu_fm_06_3f_get_turbo_status;
        g_platform[idx].variorum_enable_turbo = intel_cpu_fm_06_3f_enable_turbo;
        g_platform[idx].variorum_disable_turbo = intel_cpu_fm_06_3f_disable_turbo;
        g_platform[idx].variorum_poll_power = intel_cpu_fm_06_3f_poll_power;
        g_platform[idx].variorum_monitoring = intel_cpu_fm_06_3f_monitoring;
        //g_platform[idx].variorum_cap_each_core_frequency =
        //    intel_cpu_fm_06_3f_cap_frequency;
        g_platform[idx].variorum_get_node_power_json =
            intel_cpu_fm_06_3f_get_node_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            intel_cpu_fm_06_3f_get_node_power_domain_info_json;
        g_platform[idx].variorum_cap_best_effort_node_power_limit =
            intel_cpu_fm_06_3f_cap_best_effort_node_power_limit;
        g_platform[idx].variorum_print_available_frequencies =
            intel_cpu_fm_06_3f_get_frequencies;
        g_platform[idx].variorum_get_frequency_json =
            intel_cpu_fm_06_3f_get_clocks_json;
    }
    // Broadwell 06_4F
    else if (*g_platform[idx].arch_id == FM_06_4F)
    {
        g_platform[idx].variorum_print_power_limit =
            intel_cpu_fm_06_4f_get_power_limits;
        g_platform[idx].variorum_cap_each_socket_power_limit =
            intel_cpu_fm_06_4f_cap_power_limits;
        g_platform[idx].variorum_print_features = intel_cpu_fm_06_4f_get_features;
        g_platform[idx].variorum_print_thermals = intel_cpu_fm_06_4f_get_thermals;
        g_platform[idx].variorum_print_counters = intel_cpu_fm_06_4f_get_counters;
        g_platform[idx].variorum_print_frequency = intel_cpu_fm_06_4f_get_clocks;
        g_platform[idx].variorum_print_power = intel_cpu_fm_06_4f_get_power;
        g_platform[idx].variorum_print_turbo = intel_cpu_fm_06_4f_get_turbo_status;
        g_platform[idx].variorum_enable_turbo = intel_cpu_fm_06_4f_enable_turbo;
        g_platform[idx].variorum_disable_turbo = intel_cpu_fm_06_4f_disable_turbo;
        g_platform[idx].variorum_poll_power = intel_cpu_fm_06_4f_poll_power;
        g_platform[idx].variorum_monitoring = intel_cpu_fm_06_4f_monitoring;
        //g_platform[idx].variorum_cap_each_core_frequency =
        //    intel_cpu_fm_06_4f_cap_frequency;
        g_platform[idx].variorum_get_node_power_json =
            intel_cpu_fm_06_4f_get_node_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            intel_cpu_fm_06_4f_get_node_power_domain_info_json;
        g_platform[idx].variorum_cap_best_effort_node_power_limit =
            intel_cpu_fm_06_4f_cap_best_effort_node_power_limit;
        g_platform[idx].variorum_print_available_frequencies =
            intel_cpu_fm_06_4f_get_frequencies;
        g_platform[idx].variorum_get_frequency_json =
            intel_cpu_fm_06_4f_get_clocks_json;

    }
    // Skylake 06_55
    else if (*g_platform[idx].arch_id == FM_06_55)
    {
        g_platform[idx].variorum_print_power_limit =
            intel_cpu_fm_06_55_get_power_limits;
        g_platform[idx].variorum_cap_each_socket_power_limit =
            intel_cpu_fm_06_55_cap_power_limits;
        g_platform[idx].variorum_print_features = intel_cpu_fm_06_55_get_features;
        g_platform[idx].variorum_print_thermals = intel_cpu_fm_06_55_get_thermals;
        g_platform[idx].variorum_print_counters = intel_cpu_fm_06_55_get_counters;
        g_platform[idx].variorum_print_frequency = intel_cpu_fm_06_55_get_clocks;
        g_platform[idx].variorum_print_power = intel_cpu_fm_06_55_get_power;
        //g_platform[idx].variorum_print_turbo = intel_cpu_fm_06_55_get_turbo_status;
        //g_platform[idx].variorum_enable_turbo = intel_cpu_fm_06_55_enable_turbo;
        //g_platform[idx].variorum_disable_turbo = intel_cpu_fm_06_55_disable_turbo;
        g_platform[idx].variorum_poll_power = intel_cpu_fm_06_55_poll_power;
        g_platform[idx].variorum_monitoring = intel_cpu_fm_06_55_monitoring;
        g_platform[idx].variorum_get_node_power_json =
            intel_cpu_fm_06_55_get_node_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            intel_cpu_fm_06_55_get_node_power_domain_info_json;
        g_platform[idx].variorum_cap_best_effort_node_power_limit =
            intel_cpu_fm_06_55_cap_best_effort_node_power_limit;
        g_platform[idx].variorum_cap_each_core_frequency_limit =
            intel_cpu_fm_06_55_cap_frequency;
        g_platform[idx].variorum_print_available_frequencies =
            intel_cpu_fm_06_55_get_frequencies;
        g_platform[idx].variorum_get_frequency_json =
            intel_cpu_fm_06_55_get_clocks_json;
    }
    // Kaby Lake 06_9E
    else if (*g_platform[idx].arch_id == FM_06_9E)
    {
        g_platform[idx].variorum_print_power_limit =
            intel_cpu_fm_06_9e_get_power_limits;
        g_platform[idx].variorum_cap_each_socket_power_limit =
            intel_cpu_fm_06_9e_cap_power_limits;
        g_platform[idx].variorum_print_features = intel_cpu_fm_06_9e_get_features;
        g_platform[idx].variorum_print_thermals = intel_cpu_fm_06_9e_get_thermals;
        g_platform[idx].variorum_print_counters = intel_cpu_fm_06_9e_get_counters;
        g_platform[idx].variorum_print_frequency = intel_cpu_fm_06_9e_get_clocks;
        g_platform[idx].variorum_print_power = intel_cpu_fm_06_9e_get_power;
        //g_platform[idx].variorum_print_turbo = intel_cpu_fm_06_9e_get_turbo_status;
        //g_platform[idx].variorum_enable_turbo = intel_cpu_fm_06_9e_enable_turbo;
        //g_platform[idx].variorum_disable_turbo = intel_cpu_fm_06_9e_disable_turbo;
        g_platform[idx].variorum_poll_power = intel_cpu_fm_06_9e_poll_power;
        g_platform[idx].variorum_monitoring = intel_cpu_fm_06_9e_monitoring;
        g_platform[idx].variorum_get_node_power_json =
            intel_cpu_fm_06_9e_get_node_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            intel_cpu_fm_06_9e_get_node_power_domain_info_json;
        g_platform[idx].variorum_cap_best_effort_node_power_limit =
            intel_cpu_fm_06_9e_cap_best_effort_node_power_limit;
        g_platform[idx].variorum_print_available_frequencies =
            intel_cpu_fm_06_9e_get_frequencies;
        g_platform[idx].variorum_get_frequency_json =
            intel_cpu_fm_06_9e_get_clocks_json;
    }
    // Ice Lake 06_6A
    else if (*g_platform[idx].arch_id == FM_06_6A)
    {
        g_platform[idx].variorum_print_power_limit =
            intel_cpu_fm_06_6a_get_power_limits;
        g_platform[idx].variorum_print_features = intel_cpu_fm_06_6a_get_features;
        g_platform[idx].variorum_print_power = intel_cpu_fm_06_6a_get_power;
    }
    // Sapphire Rapids 06_8F
    else if (*g_platform[idx].arch_id == FM_06_8F)
    {
        g_platform[idx].variorum_print_power_limit = fm_06_8f_get_power_limits;
        g_platform[idx].variorum_print_features = fm_06_8f_get_features;
        g_platform[idx].variorum_print_power = fm_06_8f_get_power;
        g_platform[idx].variorum_get_node_power_json =
            fm_06_8f_get_node_power_json;
        g_platform[idx].variorum_get_node_power_domain_info_json =
            fm_06_8f_get_node_power_domain_info_json;
        g_platform[idx].variorum_monitoring = fm_06_8f_monitoring;
    }
    else
    {
        return VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    return err;
}
