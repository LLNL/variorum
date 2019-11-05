// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CLOCKS_FEATURES_H_INCLUDE
#define CLOCKS_FEATURES_H_INCLUDE

#include <stdint.h>
#include <config_architecture.h>

///// @brief Structure containing data for IA32_CLOCK_MODULATION.
/////
///// There is a bit at 0 that can be used for Extended On-Demand Clock
///// Modulation Duty Cycle. It is added with the bits 3:1. When used, the
///// granularity of clock modulation duty cycle is increased to 6.25% as opposed
///// to 12.5%. To enable this, must have CPUID.06H:EAX[Bit 5] = 1. I am not sure
///// how to check that because otherwise bit 0 is reserved.
//struct clock_mod
//{
//    /// @brief Raw 64-bit value stored in IA32_CLOCK_MODULATION.
//    uint64_t raw;
//    /// @brief Enable/disable on-demand software-controlled clock modulation.
//    int duty_cycle_enable;
//    /// @brief On-demand clock modulation duty cycle.
//    int duty_cycle;
//    //   Value   | Duty Cycle
//    // 000b = 0	 |  Reserved
//    // 001b = 1	 |    12.5% (default)
//    // 010b = 2	 |    25.0%
//    // 011b = 3	 |    37.5%
//    // 100b = 4	 |    50.0%
//    // 101b = 5	 |    63.5%
//    // 110b = 6	 |    75.0%
//    // 111b = 7	 |    87.5%
//};

/// @brief Structure containing data for IA32_APERF, IA32_MPERF, and
/// IA32_TIME_STAMP_COUNTER.
struct clocks_data
{
    /// @brief Raw 64-bit value stored in IA32_APERF.
    uint64_t **aperf;
    /// @brief Raw 64-bit value stored in IA32_MPERF.
    uint64_t **mperf;
    /// @brief Raw 64-bit value stored in IA32_TIME_STAMP_COUNTER.
    uint64_t **tsc;
};

/// @brief Structure containing data for IA32_PERF_STATUS and IA32_PERF_CTL.
struct perf_data
{
    /// @brief Raw 64-bit value stored in IA32_PERF_STATUS.
    uint64_t **perf_status;
    /// @brief Raw 64-bit value stored in IA32_PERF_CTL.
    uint64_t **perf_ctl;
};

/// @brief Allocate array for storing raw register data from IA32_APERF,
/// IA32_MPERF, and IA32_TIME_STAMP_COUNTER.
///
/// There are plans to use a struct to make the indirection less crazy.
///
/// @param [in] cd Pointer to clock-related data.
/// @param [in] msr_aperf Unique MSR address for IA32_APERF.
/// @param [in] msr_mperf Unique MSR address for IA32_MPERF.
/// @param [in] msr_tsc Unique MSR address for IA32_TIME_STAMP_COUNTER.
void clocks_storage(struct clocks_data **cd,
                    off_t msr_aperf,
                    off_t msr_mperf,
                    off_t msr_tsc);

/// @brief Allocate array for storing raw register data from IA32_PERF_STATUS
/// and IA32_PERF_CTL.
///
/// @param [in] pd Pointer to perf-related data.
/// @param [in] msr_perf_status Unique MSR address for IA32_PERF_STATUS.
void perf_storage(struct perf_data **pd,
                  off_t msr_perf_status);

///// @brief Print the label for the abbreviated clocks data print out.
/////
///// @param [in] writedest File stream where output will be written to.
//void dump_clocks_data_terse_label(FILE *writedest);

/// @brief Print clocks data in CSV format.
///
/// @param [in] writedest File stream where output will be written to.
/// @param [in] msr_aperf Unique MSR address for IA32_APERF.
/// @param [in] msr_mperf Unique MSR address for IA32_MPERF.
/// @param [in] msr_tsc Unique MSR address for IA32_TIME_STAMP_COUNTER.
/// @param [in] msr_perf_status Unique MSR address for IA32_PERF_STATUS.
/// @param [in] msr_platform_info Unique MSR address for MSR_PLATFORM_INFO.
/// @param [in] control_domain Specific granularity of control.
void dump_clocks_data(FILE *writedest,
                      off_t msr_aperf,
                      off_t msr_mperf,
                      off_t msr_tsc,
                      off_t msr_perf_status,
                      off_t msr_platform_info,
                      enum ctl_domains_e control_domain);

/// @brief Print clocks data in long format.
///
/// @param [in] writedest File stream where output will be written to.
/// @param [in] msr_aperf Unique MSR address for IA32_APERF.
/// @param [in] msr_mperf Unique MSR address for IA32_MPERF.
/// @param [in] msr_tsc Unique MSR address for IA32_TIME_STAMP_COUNTER.
/// @param [in] msr_perf_status Unique MSR address for IA32_PERF_STATUS.
/// @param [in] msr_platform_info Unique MSR address for MSR_PLATFORM_INFO.
/// @param [in] control_domain Specific granularity of control.
void print_clocks_data(FILE *writedest,
                       off_t msr_aperf,
                       off_t msr_mperf,
                       off_t msr_tsc,
                       off_t msr_perf_status,
                       off_t msr_platform_info,
                       enum ctl_domains_e control_domain);

///// @brief Print current p-state.
/////
///// @param [in] writedest File stream where output will be written to.
//void dump_p_state(FILE *writedest);
//
///// @brief Request new current p-state.
/////
///// @param [in] socket Unique socket/package identifier.
/////
///// @param [in] pstate Desired p-state.
//void set_p_state(unsigned socket,
//                 uint64_t pstate);
void set_p_state(int cpu_freq_mhz,
                 enum ctl_domains_e domain,
                 off_t msr_perf_status,
                 off_t msr_perf_ctl);
//
///****************************************/
///* Software Controlled Clock Modulation */
///****************************************/
//
///// @brief Print clock modulation data.
/////
///// @param [in] s Data for clock modulation.
/////
///// @param [in] writedest File stream where output will be written to.
//void dump_clock_mod(struct clock_mod *s,
//                    FILE *writedest);
//
///// @brief Get contents of IA32_CLOCK_MODULATION.
/////
///// @param [in] socket Unique socket/package identifier.
/////
///// @param [in] core Unique core identifier.
/////
///// @param [out] s Data for clock modulation.
//void get_clock_mod(int socket,
//                   int core,
//                   struct clock_mod *s);
//
///// @brief Change value of IA32_CLOCK_MODULATION.
/////
///// @param [in] socket Unique socket/package identifier.
/////
///// @param [in] core Unique core identifier.
/////
///// @param [in] s Data for clock modulation.
//int set_clock_mod(int socket,
//                  int core,
//                  struct clock_mod *s);

#endif
