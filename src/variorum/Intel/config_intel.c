#include <stdio.h>
#include <stdlib.h>

#include <config_intel.h>
#include <config_architecture.h>
#include <IvyBridge_3E.h>
#include <Broadwell_4F.h>
#include <KabyLake_9E.h>
#include <variorum_error.h>

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

int set_intel_func_ptrs(void)
{
    int err = 0;

    // Sandy Bridge 06_2A
    if (*g_platform.intel_arch == FM_06_2A)
    {
        return VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }
    // Ivy Bridge 06_3E
    else if (*g_platform.intel_arch == FM_06_3E)
    {
        g_platform.dump_power_limits = fm_06_3e_get_power_limits;
        g_platform.set_each_package_power_limit = fm_06_3e_set_power_limits;
        g_platform.print_features = fm_06_3e_get_features;
        g_platform.dump_thermals = fm_06_3e_get_thermals;
        g_platform.dump_counters = fm_06_3e_get_counters;
        g_platform.dump_clocks = fm_06_3e_get_clocks;
        g_platform.dump_power = fm_06_3e_get_power;
    }
    // Broadwell 06_4F
    else if (*g_platform.intel_arch == FM_06_4F)
    {
        g_platform.dump_power_limits = fm_06_4f_get_power_limits;
        g_platform.set_each_package_power_limit = fm_06_4f_set_power_limits;
        g_platform.print_features = fm_06_4f_get_features;
        g_platform.dump_thermals = fm_06_4f_get_thermals;
        g_platform.dump_counters = fm_06_4f_get_counters;
        g_platform.dump_clocks = fm_06_4f_get_clocks;
        g_platform.dump_power = fm_06_4f_get_power;
    }
    // Kaby Lake 06_9E
    else if (*g_platform.intel_arch == FM_06_9E)
    {
        g_platform.dump_power_limits = fm_06_9e_get_power_limits;
        g_platform.set_each_package_power_limit = fm_06_9e_set_power_limits;
        g_platform.print_features = fm_06_9e_get_features;
        g_platform.dump_thermals = fm_06_9e_get_thermals;
        g_platform.dump_counters = fm_06_9e_get_counters;
        g_platform.dump_clocks = fm_06_9e_get_clocks;
        g_platform.dump_power = fm_06_9e_get_power;
    }
    else
    {
        return VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    return err;
}
