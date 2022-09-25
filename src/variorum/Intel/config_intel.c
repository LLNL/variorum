// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
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

/* Recognized architecture and variants (09/2022)
 *
 * 06_2A    Sandy Bridge
 * 06_2D    Sandy Bridge
 * 06_3A    Ivy Bridge
 * 06_3C    Haswell
 * 06_3D    Broadwell
 * 06_3E    Ivy Bridge
 * 06_3F    Haswell
 * 06_45    Haswell
 * 06_46    Haswell
 * 06_47    Broadwell
 * 06_4E    Skylake
 * 06_4F    Broadwell
 * 06_55    Skylake, Cascade Lake, Cooper Lake
 * 06_56    Broadwell
 * 06_5E    Skylake
 * 06_66    Cannon Lake
 * 06_6A    Ice Lake
 * 06_6C    Ice Lake
 * 06_7D    Ice Lake
 * 06_7E    Ice Lake
 * 06_8C    Tiger Lake
 * 06_8D    Tiger Lake
 * 06_8E    Coffee Lake
 * 06_8F    Sapphire Rapids
 * 06_97    Alder Lake
 * 06_9E    Kaby Lake, Coffee Lake
 * 06_9A    Alder Lake
 * 06_A5    Comet Lake
 * 06_A6    Comet Lake
 * 06_A7    Rocket Lake (No MSRs listed in SDM)
 * 06_A8    Rocket Lake (No MSRs listed in SDM)
 * 06_BF    Alder Lake
 */

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
int set_intel_func_ptrs(void)
{
    int err = 0;

    // GPU power ratio doesn't apply to Intel architectures.
    // So specify a common function that exits with a log message.
    g_platform.variorum_cap_gpu_power_ratio = gpu_power_ratio_unimplemented;

    /* 06_2AH
     * Sandy Bridge
     * Intel Xeon processor E3-1200 product family;
     * 2nd Generation Intel Core i7, i5, i3 Processors 2xxx Series
     */
    if (*g_platform.intel_arch == FM_06_2A)
    {
        g_platform.variorum_print_power_limit = fm_06_2a_get_power_limits;
        g_platform.variorum_cap_each_socket_power_limit =
            fm_06_2a_cap_power_limits;
        g_platform.variorum_print_features = fm_06_2a_get_features;
        g_platform.variorum_print_thermals = fm_06_2a_get_thermals;
        g_platform.variorum_print_counters = fm_06_2a_get_counters;
        g_platform.variorum_print_frequency = fm_06_2a_get_clocks;
        g_platform.variorum_print_power = fm_06_2a_get_power;
        g_platform.variorum_print_turbo = fm_06_2a_get_turbo_status;
        g_platform.variorum_enable_turbo = fm_06_2a_enable_turbo;
        g_platform.variorum_disable_turbo = fm_06_2a_disable_turbo;
        g_platform.variorum_poll_power = fm_06_2a_poll_power;
        g_platform.variorum_monitoring = fm_06_2a_monitoring;
        //g_platform.variorum_cap_each_core_frequency =
        //    fm_06_2a_cap_frequency;
        g_platform.variorum_get_node_power_json =
            fm_06_2a_get_node_power_json;
        g_platform.variorum_get_node_power_domain_info_json =
            fm_06_2a_get_node_power_domain_info_json;
        g_platform.variorum_cap_best_effort_node_power_limit =
            fm_06_2a_cap_best_effort_node_power_limit;
        g_platform.variorum_print_available_frequencies =
            fm_06_2a_get_frequencies;
    }
    /* 06_2D
     * Sandy Bridge
     * Intel Xeon processor E5 Family based on Sandy Bridge microarchitecture,
     * Intel Core i7-39xx Processor Extreme Edition
     */
    else if (*g_platform.intel_arch == FM_06_2D)
    {
        g_platform.variorum_print_power_limit = fm_06_2d_get_power_limits;
        g_platform.variorum_cap_each_socket_power_limit = fm_06_2d_cap_power_limits;
        g_platform.variorum_print_features = fm_06_2d_get_features;
        g_platform.variorum_print_thermals = fm_06_2d_get_thermals;
        g_platform.variorum_print_counters = fm_06_2d_get_counters;
        g_platform.variorum_print_frequency = fm_06_2d_get_clocks;
        g_platform.variorum_print_power = fm_06_2d_get_power;
        g_platform.variorum_print_turbo = fm_06_2d_get_turbo_status;
        g_platform.variorum_enable_turbo = fm_06_2d_enable_turbo;
        g_platform.variorum_disable_turbo = fm_06_2d_disable_turbo;
        g_platform.variorum_poll_power = fm_06_2d_poll_power;
        g_platform.variorum_monitoring = fm_06_2d_monitoring;
        //g_platform.variorum_cap_each_core_frequency =
        //    fm_06_2d_cap_frequency;
        g_platform.variorum_get_node_power_json =
            fm_06_2d_get_node_power_json;
        g_platform.variorum_get_node_power_domain_info_json =
            fm_06_2d_get_node_power_domain_info_json;
        g_platform.variorum_cap_best_effort_node_power_limit =
            fm_06_2d_cap_best_effort_node_power_limit;
        g_platform.variorum_print_available_frequencies =
            fm_06_2d_get_frequencies;
    }
    /* 06_3A
     * Ivy Bridge
     * 3rd Generation Intel Core Processor and Intel Xeon processor E3-1200 v2
     * product family based on Ivy Bridge microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_3A)
    {
    }
    /* 06_3C
     * Haswell
     * 4th Generation Intel Core processor and Intel Xeon processor E3-1200
     * v3 product family based on Haswell microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_3C)
    {
    }
    /* 06_3D
     * Broadwell
     * Intel Core M-5xxx Processor, 5th generation Intel Core processors based on
     * Broadwell microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_3D)
    {
    }
    /* 06_3E
     * Ivy Bridge
     * Intel Xeon processor E5-2600/1600 v2 product families and Intel Xeon
     * processor E5-2400 v2 product family based on Ivy Bridge-E microarchitecture,
     * Intel Core i7-49xx Processor Extreme Edition
     * Intel Xeon processor E7-8800/4800/2800 v2 product families based on Ivy
     * Bridge-E microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_3E)
    {
        g_platform.variorum_print_power_limit = fm_06_3e_get_power_limits;
        g_platform.variorum_cap_each_socket_power_limit =
            fm_06_3e_cap_power_limits;
        g_platform.variorum_print_features = fm_06_3e_get_features;
        g_platform.variorum_print_thermals = fm_06_3e_get_thermals;
        g_platform.variorum_print_counters = fm_06_3e_get_counters;
        g_platform.variorum_print_frequency = fm_06_3e_get_clocks;
        g_platform.variorum_print_power = fm_06_3e_get_power;
        g_platform.variorum_print_turbo = fm_06_3e_get_turbo_status;
        g_platform.variorum_enable_turbo = fm_06_3e_enable_turbo;
        g_platform.variorum_disable_turbo = fm_06_3e_disable_turbo;
        g_platform.variorum_poll_power = fm_06_3e_poll_power;
        g_platform.variorum_monitoring = fm_06_3e_monitoring;
        //g_platform.variorum_cap_each_core_frequency =
        //    fm_06_3e_cap_frequency;
        g_platform.variorum_get_node_power_json =
            fm_06_3e_get_node_power_json;
        g_platform.variorum_get_node_power_domain_info_json =
            fm_06_3e_get_node_power_domain_info_json;
        g_platform.variorum_cap_best_effort_node_power_limit =
            fm_06_3e_cap_best_effort_node_power_limit;
        g_platform.variorum_print_available_frequencies =
            fm_06_3e_get_frequencies;
    }
    /* 06_3F
     * Haswell
     * Intel Xeon processor E5-4600/2600/1600 v3 product families, Intel Xeon
     * processor E7 v3 product families based on Haswell-E microarchitecture,
     * Intel Core i7-59xx Processor Extreme Edition
     */
    else if (*g_platform.intel_arch == FM_06_3F)
    {
        g_platform.variorum_print_power_limit = fm_06_3f_get_power_limits;
        g_platform.variorum_cap_each_socket_power_limit =
            fm_06_3f_cap_power_limits;
        g_platform.variorum_print_features = fm_06_3f_get_features;
        g_platform.variorum_print_thermals = fm_06_3f_get_thermals;
        g_platform.variorum_print_counters = fm_06_3f_get_counters;
        g_platform.variorum_print_frequency = fm_06_3f_get_clocks;
        g_platform.variorum_print_power = fm_06_3f_get_power;
        g_platform.variorum_print_turbo = fm_06_3f_get_turbo_status;
        g_platform.variorum_enable_turbo = fm_06_3f_enable_turbo;
        g_platform.variorum_disable_turbo = fm_06_3f_disable_turbo;
        g_platform.variorum_poll_power = fm_06_3f_poll_power;
        g_platform.variorum_monitoring = fm_06_3f_monitoring;
        //g_platform.variorum_cap_each_core_frequency =
        //    fm_06_3f_cap_frequency;
        g_platform.variorum_get_node_power_json =
            fm_06_3f_get_node_power_json;
        g_platform.variorum_get_node_power_domain_info_json =
            fm_06_3f_get_node_power_domain_info_json;
        g_platform.variorum_cap_best_effort_node_power_limit =
            fm_06_3f_cap_best_effort_node_power_limit;
        g_platform.variorum_print_available_frequencies =
            fm_06_3f_get_frequencies;
    }
    /* 06_45
     * Haswell
     * 4th Generation Intel Core processor and Intel Xeon processor E3-1200 v3
     * product family based on Haswell microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_45)
    {
    }
    /* 06_46
     * Haswell
     * 4th Generation Intel Core processor and Intel Xeon processor E3-1200 v3
     * product family based on Haswell microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_46)
    {
    }
    /* 06_47
     * Broadwell
     * 5th generation Intel Core processors, Intel Xeon processor E3-1200 v4
     * product family based on Broadwell microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_47)
    {
    }
    /* 06_4E
     * Skylake
     * 6th generation Intel Core processors and Intel Xeon processor E3-1500m v5
     * product family and E3- 1200 v5 product family based on Skylake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_4E)
    {
    }
    /* 06_4F
     * Broadwell
     * Intel Xeon processor E5 v4 Family based on Broadwell microarchitecture,
     * Intel Xeon processor E7 v4 Family, Intel Core i7-69xx Processor Extreme
     * Edition
     */
    else if (*g_platform.intel_arch == FM_06_4F)
    {
        g_platform.variorum_print_power_limit = fm_06_4f_get_power_limits;
        g_platform.variorum_cap_each_socket_power_limit =
            fm_06_4f_cap_power_limits;
        g_platform.variorum_print_features = fm_06_4f_get_features;
        g_platform.variorum_print_thermals = fm_06_4f_get_thermals;
        g_platform.variorum_print_counters = fm_06_4f_get_counters;
        g_platform.variorum_print_frequency = fm_06_4f_get_clocks;
        g_platform.variorum_print_power = fm_06_4f_get_power;
        g_platform.variorum_print_turbo = fm_06_4f_get_turbo_status;
        g_platform.variorum_enable_turbo = fm_06_4f_enable_turbo;
        g_platform.variorum_disable_turbo = fm_06_4f_disable_turbo;
        g_platform.variorum_poll_power = fm_06_4f_poll_power;
        g_platform.variorum_monitoring = fm_06_4f_monitoring;
        //g_platform.variorum_cap_each_core_frequency =
        //    fm_06_4f_cap_frequency;
        g_platform.variorum_get_node_power_json =
            fm_06_4f_get_node_power_json;
        g_platform.variorum_get_node_power_domain_info_json =
            fm_06_4f_get_node_power_domain_info_json;
        g_platform.variorum_cap_best_effort_node_power_limit =
            fm_06_4f_cap_best_effort_node_power_limit;
        g_platform.variorum_print_available_frequencies =
            fm_06_4f_get_frequencies;
    }
    /* 06_55
     * Skylake, Cascade Lake, Cooper Lake
     * Intel Xeon Processor Scalable Family based on Skylake
     * microarchitecture, 2nd generation Intel Xeon Processor Scalable Family
     * based on Cascade Lake product, and 3rd generation Intel Xeon Processor
     * Scalable Family based on Cooper Lake product
     */
    else if (*g_platform.intel_arch == FM_06_55)
    {
        g_platform.variorum_print_power_limit = fm_06_55_get_power_limits;
        g_platform.variorum_cap_each_socket_power_limit =
            fm_06_55_cap_power_limits;
        g_platform.variorum_print_features = fm_06_55_get_features;
        g_platform.variorum_print_thermals = fm_06_55_get_thermals;
        g_platform.variorum_print_counters = fm_06_55_get_counters;
        g_platform.variorum_print_frequency = fm_06_55_get_clocks;
        g_platform.variorum_print_power = fm_06_55_get_power;
        //g_platform.variorum_print_turbo = fm_06_55_get_turbo_status;
        //g_platform.variorum_enable_turbo = fm_06_55_enable_turbo;
        //g_platform.variorum_disable_turbo = fm_06_55_disable_turbo;
        g_platform.variorum_poll_power = fm_06_55_poll_power;
        g_platform.variorum_monitoring = fm_06_55_monitoring;
        g_platform.variorum_get_node_power_json =
            fm_06_55_get_node_power_json;
        g_platform.variorum_get_node_power_domain_info_json =
            fm_06_55_get_node_power_domain_info_json;
        g_platform.variorum_cap_best_effort_node_power_limit =
            fm_06_55_cap_best_effort_node_power_limit;
        g_platform.variorum_cap_each_core_frequency_limit = fm_06_55_cap_frequency;
        g_platform.variorum_print_available_frequencies =
            fm_06_55_get_frequencies;
    }
    /* 06_56
     * Broadwell
     * Intel Xeon processor D-1500 product family based on Broadwell
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_56)
    {
    }
    /* 06_5E
     * Skylake
     * 6th generation Intel Core processors and Intel Xeon processor E3-1500m v5
     * product family and E3- 1200 v5 product family based on Skylake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_5E)
    {
    }
    /* 06_66
     * Cannon Lake
     * Intel® CoreTM processors based on Cannon Lake microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_66)
    {
    }
    /* 06_6A
     * Ice Lake
     * 3rd generation Intel® Xeon® Processor Scalable Family based on Ice Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_6A)
    {
        g_platform.variorum_print_power_limit = fm_06_6a_get_power_limits;
        g_platform.variorum_print_features = fm_06_6a_get_features;
        g_platform.variorum_print_power = fm_06_6a_get_power;
    }
    /* 06_6C
     * Ice Lake
     * 3rd generation Intel® Xeon® Processor Scalable Family based on Ice Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_6C)
    {
    }
    /* 06_7D
     * Ice Lake
     * 10th generation Intel® CoreTM processors based on Ice Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_7D)
    {
    }
    /* 06_7E
     * Ice Lake
     * 10th generation Intel® CoreTM processors based on Ice Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_7E)
    {
    }
    /* 06_8C
     * Tiger Lake
     * 11th generation Intel® CoreTM processors based on Tiger Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_8C)
    {
    }
    /* 06_8D
     * Tiger Lake
     * 11th generation Intel® CoreTM processors based on Tiger Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_8D)
    {
    }
    /* 06_8E
     * Coffee Lake
     * 7th generation Intel® CoreTM processors based on Kaby Lake
     * microarchitecture, 8th and 9th generation Intel® CoreTM processors based
     * on Coffee Lake microarchitecture, Intel® Xeon® E processors based on
     * Coffee Lake microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_8E)
    {
    }
    /* 06_8F
     * Sapphire Rapids
     * Future Intel® Xeon® processors based on Sapphire Rapids microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_8F)
    {
    }
    /* 06_97
     * Alder Lake
     * 12th generation Intel® CoreTM processors supporting Alder Lake
     * performance hybrid architecture
     */
    else if (*g_platform.intel_arch == FM_06_97)
    {
    }
    /* 06_9E
     * Kaby Lake, Coffee Lake
     * 7th generation Intel® CoreTM processors based on Kaby Lake
     * microarchitecture, 8th and 9th generation Intel® CoreTM processors based
     * on Coffee Lake microarchitecture, Intel® Xeon® E processors based on
     * Coffee Lake microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_9E)
    {
        g_platform.variorum_print_power_limit = fm_06_9e_get_power_limits;
        g_platform.variorum_cap_each_socket_power_limit =
            fm_06_9e_cap_power_limits;
        g_platform.variorum_print_features = fm_06_9e_get_features;
        g_platform.variorum_print_thermals = fm_06_9e_get_thermals;
        g_platform.variorum_print_counters = fm_06_9e_get_counters;
        g_platform.variorum_print_frequency = fm_06_9e_get_clocks;
        g_platform.variorum_print_power = fm_06_9e_get_power;
        //g_platform.variorum_print_turbo = fm_06_9e_get_turbo_status;
        //g_platform.variorum_enable_turbo = fm_06_9e_enable_turbo;
        //g_platform.variorum_disable_turbo = fm_06_9e_disable_turbo;
        g_platform.variorum_poll_power = fm_06_9e_poll_power;
        g_platform.variorum_monitoring = fm_06_9e_monitoring;
        g_platform.variorum_get_node_power_json =
            fm_06_9e_get_node_power_json;
        g_platform.variorum_get_node_power_domain_info_json =
            fm_06_9e_get_node_power_domain_info_json;
        g_platform.variorum_cap_best_effort_node_power_limit =
            fm_06_9e_cap_best_effort_node_power_limit;
        g_platform.variorum_print_available_frequencies =
            fm_06_9e_get_frequencies;
    }
    /* 06_9A
     * Alder Lake
     * 12th generation Intel® CoreTM processors supporting Alder Lake
     * performance hybrid architecture
     */
    else if (*g_platform.intel_arch == FM_06_9A)
    {
    }
    /* 06_A5
     * Comet Lake
     * 10th generation Intel Core processors based on Comet Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_A5)
    {
    }
    /* 06_A6
     * Comet Lake
     * 10th generation Intel Core processors based on Comet Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_A6)
    {
    }
    /* 06_A7
     * Rocket Lake
     * 10th generation Intel Core processors based on Comet Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_A7)
    {
    }
    /* 06_A8
     * Rocket Lake
     * 10th generation Intel Core processors based on Comet Lake
     * microarchitecture
     */
    else if (*g_platform.intel_arch == FM_06_A8)
    {
    }
    /* 06_BF
     * Alder Lake
     * [No description provided in table 2-1]
     */
    else if (*g_platform.intel_arch == FM_06_BF)
    {
    }
    else
    {
        return VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    return err;
}
