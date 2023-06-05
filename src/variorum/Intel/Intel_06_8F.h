// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef SAPPHIRE_RAPIDS_6A_H_INCLUDE
#define SAPPHIRE_RAPIDS_6A_H_INCLUDE

#include <sys/types.h>
#include <jansson.h>

/// @brief List of unique addresses for Sapphire Rapids Family/Model 6AH.
struct sapphire_rapids_6a_offsets
{
    /// @brief Address for MSR_PLATFORM_INFO.
    off_t msr_platform_info;
    /// @brief Address for IA32_TIME_STAMP_COUNTER.
    off_t ia32_time_stamp_counter;
    /// @brief Address for RAPL_POWER_UNIT.
    off_t msr_rapl_power_unit;
    /// @brief Address for PKG_POWER_LIMIT.
    off_t msr_pkg_power_limit;
    /// @brief Address for PKG_ENERGY_STATUS.
    off_t msr_pkg_energy_status;
    /// @brief Address for PKG_POWER_INFO.
    off_t msr_pkg_power_info;
    /// @brief Address for DRAM_POWER_LIMIT.
    off_t msr_dram_power_limit;
    /// @brief Address for DRAM_ENERGY_STATUS.
    off_t msr_dram_energy_status;
    /// @brief Address for DRAM_POWER_INFO.
    off_t msr_dram_power_info;
    /// @brief Address for IA32_FIXED_CTR_CTRL.
    off_t ia32_fixed_ctr_ctrl;
    /// @brief Address for IA32_PERF_GLOBAL_STATUS.
    off_t ia32_perf_global_status;
    /// @brief Address for IA32_PERF_GLOBAL_CTRL.
    off_t ia32_perf_global_ctrl;
    /// @brief Address for IA32_MPERF.
    off_t ia32_mperf;
    /// @brief Address for IA32_APERF.
    off_t ia32_aperf;
    /// @brief Array of unique addresses for fixed counters.
    off_t ia32_fixed_counters[3];

};

int fm_06_8f_get_power_limits(int long_ver);

int fm_06_8f_get_power(int long_ver);

int fm_06_8f_get_features(void);

int fm_06_8f_get_node_power_json(char **get_power_obj_str);

int fm_06_8f_get_node_power_domain_info_json(char **get_domain_obj_str);

int fm_06_8f_monitoring(FILE *output);
#endif
