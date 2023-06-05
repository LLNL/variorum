// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef ICELAKE_6A_H_INCLUDE
#define ICELAKE_6A_H_INCLUDE

#include <sys/types.h>
#include <jansson.h>

/// @brief List of unique addresses for Ice Lake Family/Model 6AH.
struct icelake_6a_offsets
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
};

int intel_cpu_fm_06_6a_get_power_limits(int long_ver);

int intel_cpu_fm_06_6a_get_power(int long_ver);

int intel_cpu_fm_06_6a_get_features(void);

int intel_cpu_fm_06_6a_get_node_power_json(char **get_power_obj_str);

int intel_cpu_fm_06_6a_get_node_power_domain_info_json(char
        **get_domain_obj_str);

#endif
