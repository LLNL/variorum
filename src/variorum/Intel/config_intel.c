// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_intel.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <SandyBridge_2A.h>
#include <IvyBridge_3E.h>
#include <Broadwell_4F.h>
#include <Haswell_3F.h>
#include <KabyLake_9E.h>
#include <Skylake_55.h>

uint64_t *detect_intel_arch(void)
{
    uint64_t rax = 1;
    uint64_t rbx = 0;
    uint64_t rcx = 0;
    uint64_t rdx = 0;
    uint64_t *model = (uint64_t *) malloc(sizeof(uint64_t));

    asm volatile(
        "cpuid"
        : "=a" (rax), "=b" (rbx), "=c" (rcx), "=d" (rdx)
        : "0" (rax), "2" (rcx));

    *model = ((rax >> 12) & 0xF0) | ((rax >> 4) & 0xF);
    return model;

    // return family and model appended
    //return (((rax >> 28) & 0x0F)+((rax >> 8) & 0x0F) << 8) | ((rax >> 12) & 0xF0)+((rax >> 4) & 0xF);
}

int gpu_power_ratio_unimplemented(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    printf("GPU power ratio is not available on Intel platforms.\n");
    return -1;
}

int set_intel_func_ptrs(void)
{
    int err = 0;

    // GPU power ratio doesn't apply to Intel architectures.
    // So specify a common function that exits with a log message.
    g_platform.set_gpu_power_ratio = gpu_power_ratio_unimplemented;

    // Sandy Bridge 06_2A
    if (*g_platform.intel_arch == FM_06_2A)
    {
        g_platform.dump_power_limits = fm_06_2a_get_power_limits;
        g_platform.set_each_socket_power_limit = fm_06_2a_set_power_limits;
        g_platform.print_features = fm_06_2a_get_features;
        g_platform.dump_thermals = fm_06_2a_get_thermals;
        g_platform.dump_counters = fm_06_2a_get_counters;
        g_platform.dump_clocks = fm_06_2a_get_clocks;
        g_platform.dump_power = fm_06_2a_get_power;
        g_platform.dump_turbo = fm_06_2a_get_turbo_status;
        g_platform.enable_turbo = fm_06_2a_enable_turbo;
        g_platform.disable_turbo = fm_06_2a_disable_turbo;
        g_platform.poll_power = fm_06_2a_poll_power;
        g_platform.monitoring = fm_06_2a_monitoring;
        //g_platform.set_each_core_frequency = fm_06_2a_set_frequency;
    }
    // Ivy Bridge 06_3E
    else if (*g_platform.intel_arch == FM_06_3E)
    {
        g_platform.dump_power_limits = fm_06_3e_get_power_limits;
        g_platform.set_each_socket_power_limit = fm_06_3e_set_power_limits;
        g_platform.print_features = fm_06_3e_get_features;
        g_platform.dump_thermals = fm_06_3e_get_thermals;
        g_platform.dump_counters = fm_06_3e_get_counters;
        g_platform.dump_clocks = fm_06_3e_get_clocks;
        g_platform.dump_power = fm_06_3e_get_power;
        g_platform.dump_turbo = fm_06_3e_get_turbo_status;
        g_platform.enable_turbo = fm_06_3e_enable_turbo;
        g_platform.disable_turbo = fm_06_3e_disable_turbo;
        g_platform.poll_power = fm_06_3e_poll_power;
        g_platform.monitoring = fm_06_3e_monitoring;
        //g_platform.set_each_core_frequency = fm_06_3e_set_frequency;
    }
    // Haswell 06_3F
    else if (*g_platform.intel_arch == FM_06_3F)
    {
        g_platform.dump_power_limits = fm_06_3f_get_power_limits;
        g_platform.set_each_socket_power_limit = fm_06_3f_set_power_limits;
        g_platform.print_features = fm_06_3f_get_features;
        g_platform.dump_thermals = fm_06_3f_get_thermals;
        g_platform.dump_counters = fm_06_3f_get_counters;
        g_platform.dump_clocks = fm_06_3f_get_clocks;
        g_platform.dump_power = fm_06_3f_get_power;
        g_platform.dump_turbo = fm_06_3f_get_turbo_status;
        g_platform.enable_turbo = fm_06_3f_enable_turbo;
        g_platform.disable_turbo = fm_06_3f_disable_turbo;
        g_platform.poll_power = fm_06_3f_poll_power;
        g_platform.monitoring = fm_06_3f_monitoring;
        //g_platform.set_each_core_frequency = fm_06_3f_set_frequency;
    }
    // Broadwell 06_4F
    else if (*g_platform.intel_arch == FM_06_4F)
    {
        g_platform.dump_power_limits = fm_06_4f_get_power_limits;
        g_platform.set_each_socket_power_limit = fm_06_4f_set_power_limits;
        g_platform.print_features = fm_06_4f_get_features;
        g_platform.dump_thermals = fm_06_4f_get_thermals;
        g_platform.dump_counters = fm_06_4f_get_counters;
        g_platform.dump_clocks = fm_06_4f_get_clocks;
        g_platform.dump_power = fm_06_4f_get_power;
        g_platform.dump_turbo = fm_06_4f_get_turbo_status;
        g_platform.enable_turbo = fm_06_4f_enable_turbo;
        g_platform.disable_turbo = fm_06_4f_disable_turbo;
        g_platform.poll_power = fm_06_4f_poll_power;
        g_platform.monitoring = fm_06_4f_monitoring;
        //g_platform.set_each_core_frequency = fm_06_4f_set_frequency;
    }
    // Skylake 06_55
    else if (*g_platform.intel_arch == FM_06_55)
    {
        g_platform.dump_power_limits = fm_06_55_get_power_limits;
        g_platform.set_each_socket_power_limit = fm_06_55_set_power_limits;
        g_platform.print_features = fm_06_55_get_features;
        g_platform.dump_thermals = fm_06_55_get_thermals;
        g_platform.dump_counters = fm_06_55_get_counters;
        g_platform.dump_clocks = fm_06_55_get_clocks;
        g_platform.dump_power = fm_06_55_get_power;
        //g_platform.dump_turbo = fm_06_55_get_turbo_status;
        //g_platform.enable_turbo = fm_06_55_enable_turbo;
        //g_platform.disable_turbo = fm_06_55_disable_turbo;
        g_platform.poll_power = fm_06_55_poll_power;
        g_platform.monitoring = fm_06_55_monitoring;
        g_platform.set_each_core_frequency = fm_06_55_set_frequency;
    }
    // Kaby Lake 06_9E
    else if (*g_platform.intel_arch == FM_06_9E)
    {
        g_platform.dump_power_limits = fm_06_9e_get_power_limits;
        g_platform.set_each_socket_power_limit = fm_06_9e_set_power_limits;
        g_platform.print_features = fm_06_9e_get_features;
        g_platform.dump_thermals = fm_06_9e_get_thermals;
        g_platform.dump_counters = fm_06_9e_get_counters;
        g_platform.dump_clocks = fm_06_9e_get_clocks;
        g_platform.dump_power = fm_06_9e_get_power;
        //g_platform.dump_turbo = fm_06_9e_get_turbo_status;
        //g_platform.enable_turbo = fm_06_9e_enable_turbo;
        //g_platform.disable_turbo = fm_06_9e_disable_turbo;
        g_platform.poll_power = fm_06_9e_poll_power;
        g_platform.monitoring = fm_06_9e_monitoring;
    }
    else
    {
        return VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    return err;
}
