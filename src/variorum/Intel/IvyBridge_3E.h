// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef IVYBRIDGE_3E_H_INCLUDE
#define IVYBRIDGE_3E_H_INCLUDE

#include <sys/types.h>

/// @brief List of unique addresses for Ivy Bridge Family/Model 3EH.
struct ivybridge_3e_offsets
{
    /// @brief Address for MSR_PLATFORM_INFO.
    const off_t msr_platform_info;
    /// @brief Address for IA32_TIME_STAMP_COUNTER.
    const off_t ia32_time_stamp_counter;
    /// @brief Address for IA32_PERF_CTL.
    const off_t ia32_perf_ctl;
    /// @brief Address for IA32_PERF_STATUS.
    const off_t ia32_perf_status;
    /// @brief Address for IA32_THERM_INTERRUPT.
    const off_t ia32_therm_interrupt;
    /// @brief Address for IA32_THERM_STATUS.
    const off_t ia32_therm_status;
    /// @brief Address for THERM2_CTL.
    const off_t msr_therm2_ctl;
    /// @brief Address for IA32_MISC_ENABLE.
    const off_t ia32_misc_enable;
    /// @brief Address for TEMPERATURE_TARGET.
    const off_t msr_temperature_target;
    /// @brief Address for TURBO_RATIO_LIMIT.
    const off_t msr_turbo_ratio_limit;
    /// @brief Address for TURBO_RATIO_LIMIT1.
    const off_t msr_turbo_ratio_limit1;
    /// @brief Address for IA32_PACKAGE_THERM_STATUS.
    const off_t ia32_package_therm_status;
    /// @brief Address for IA32_PACKAGE_THERM_INTERRUPT.
    const off_t ia32_package_therm_interrupt;
    /// @brief Address for IA32_FIXED_CTR_CTRL.
    const off_t ia32_fixed_ctr_ctrl;
    /// @brief Address for IA32_PERF_GLOBAL_STATUS.
    const off_t ia32_perf_global_status;
    /// @brief Address for IA32_PERF_GLOBAL_CTRL.
    const off_t ia32_perf_global_ctrl;
    /// @brief Address for IA32_PERF_GLOBAL_OVF_CTRL.
    const off_t ia32_perf_global_ovf_ctrl;
    /// @brief Address for RAPL_POWER_UNIT.
    const off_t msr_rapl_power_unit;
    /// @brief Address for PKG_POWER_LIMIT.
    const off_t msr_pkg_power_limit;
    /// @brief Address for PKG_ENERGY_STATUS.
    const off_t msr_pkg_energy_status;
    /// @brief Address for PKG_PERF_STATUS.
    const off_t msr_pkg_perf_status;
    /// @brief Address for PKG_POWER_INFO.
    const off_t msr_pkg_power_info;
    /// @brief Address for DRAM_POWER_LIMIT.
    const off_t msr_dram_power_limit;
    /// @brief Address for DRAM_ENERGY_STATUS.
    const off_t msr_dram_energy_status;
    /// @brief Address for DRAM_PERF_STATUS.
    const off_t msr_dram_perf_status;
    /// @brief Address for TURBO_ACTIVATION_RATIO.
    const off_t msr_turbo_activation_ratio;
    /// @brief Address for IA32_MPERF.
    const off_t ia32_mperf;
    /// @brief Address for IA32_APERF.
    const off_t ia32_aperf;
    /// @brief Array of unique addresses for fixed counters.
    off_t ia32_fixed_counters[3];
    /// @brief Array of unique addresses for perfmon counters.
    off_t ia32_perfmon_counters[8];
    /// @brief Array of unique addresses for perfevtsel counters.
    off_t ia32_perfevtsel_counters[8];
    /// @brief Array of unique addresses for pmon evtsel.
    off_t msrs_pcu_pmon_evtsel[4];
};

int fm_06_3e_get_power_limits(int long_ver);

int fm_06_3e_set_power_limits(int package_power_limit);

int fm_06_3e_get_features(void);

int fm_06_3e_get_thermals(int long_ver);

int fm_06_3e_get_counters(int long_ver);

int fm_06_3e_get_clocks(int long_ver);

int fm_06_3e_get_power(int long_ver);

int fm_06_3e_enable_turbo(void);

int fm_06_3e_disable_turbo(void);

int fm_06_3e_get_turbo_status(void);

int fm_06_3e_poll_power(FILE *output);

int fm_06_3e_monitoring(FILE *output);

#endif
