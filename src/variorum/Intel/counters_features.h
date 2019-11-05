// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef COUNTERS_FEATURES_H_INCLUDE
#define COUNTERS_FEATURES_H_INCLUDE

#include <stdint.h>
#include <stdio.h>

/// @brief Structure containing configuration data for each fixed-function
/// performance counter as encoded in IA32_PERF_GLOBAL_CTL and
/// IA32_FIXED_CTR_CTL.
struct fixed_counter
{
    /// @brief Raw value stored in enable bit field of IA32_PERF_GLOBAL_CTL,
    /// indicating availability of fixed-function performance counter.
    uint64_t *enable;
    /// @brief Raw value stored in enable bit field of IA32_FIXED_CTR_CTL,
    /// enabling counting of events occurring in the specified ring level (i.e.,
    /// none, OS, user, both).
    uint64_t *ring_level;
    /// @brief Raw value stored in AnyThread bit field of IA32_FIXED_CTR_CTL
    /// enables all threads sharing the same processor thread (hyperthreaded
    /// scenario) to increment the counter.
    uint64_t *anyThread;
    /// @brief Raw value stored in performance monitoring interrupt (PMI)
    /// enable bit field of AI32_FIXED_CTR_CTL, allowing logical processor to
    /// generate an exception when the counter overflows.
    uint64_t *pmi;
    /// @brief Raw 64-bit value stored in IA32_FIXED_CTR[0-3].
    uint64_t **value;
    /// @brief Indicator of register overflow.
    uint64_t *overflow;
};

/// @brief Structure containing general information about the fixed-function
/// performance counters on the platform.
struct fixed_counter_config
{
    /// @brief Number of fixed-function performance counters available per core
    /// (e.g., IA32_FIXED_CTR0, IA32_FIXED_CTR1, IA32_FIXED_CTR2).
    int num_counters;
    /// @brief Bit width of the fixed-function performance counters.
    int width;
};

/*****************************************/
/* Fixed Counters Performance Monitoring */
/*****************************************/

/// @brief Initialize storage for fixed-function performance counter data, and
/// store it on the heap.
///
/// @param [out] ctr0 Data for fixed-function performance counter for any
///        instructions retired.
/// @param [out] ctr1 Data for fixed-function performance counter for core
///        unhalted clock cycles.
/// @param [out] ctr2 Data for fixed-function performance counter for unhalted
///        reference clock cycles.
/// @param [in] msrs_fixed_ctrs Array storing unique MSR addresses for fixed
///        counters.
void fixed_counter_storage(struct fixed_counter **ctr0,
                           struct fixed_counter **ctr1,
                           struct fixed_counter **ctr2,
                           off_t *msrs_fixed_ctrs);

/// @brief Initialize storage for fixed-function performance counter data.
///
/// @param [out] ctr Data for fixed-function performance counters.
void init_fixed_counter(struct fixed_counter *ctr);

/// @brief Set value of IA32_FIXED_CTR_CTL and IA32_PERF_GLOBAL_CTL and reset
/// all fixed-function performance counters on all logical processors.
///
/// @param [in] ctr0 Data for fixed-function performance counter for any
///        instructions retired.
/// @param [in] ctr1 Data for fixed-function performance counter for core
///        unhalted clock cycles.
/// @param [in] ctr2 Data for fixed-function performance counter for unhalted
///        reference clock cycles.
/// @param [in] msr1 Unique MSR address.
/// @param [in] msr2 Unique MSR address.
void set_fixed_counter_ctrl(struct fixed_counter *ctr0,
                            struct fixed_counter *ctr1,
                            struct fixed_counter *ctr2,
                            off_t msr1,
                            off_t msr2);

/// @brief Initialize storage for performance global control and fixed-function
/// performance control data, and store it on the heap.
///
/// There are plans to use a struct to make the indirection less crazy.
///
/// @param [out] perf_ctrl Data for controlling counting of each performance
///        counter.
/// @param [out] fixed_ctrl Pointer to data for controlling the operations of a
///        fixed-function performance counter.
/// @param [in] msr_perf_global_ctrl Unique MSR address for MSR_PERF_GLOBAL_CTRL.
/// @param [in] msr_fixed_counter_ctrl Unique MSR address for MSR_FIXED_COUNTER_CTRL.
void fixed_counter_ctrl_storage(uint64_t ***perf_ctrl,
                                uint64_t ***fixed_ctrl,
                                off_t msr_perf_global_ctrl,
                                off_t msr_fixed_counter_ctrl);

/// @brief Enable fixed-function counters by setting enable bit in
/// IA32_FIXED_CTR_CTL.
///
/// @param [in] msrs_fixed_ctrs Array storing unique MSR addresses for fixed
///        counters.
/// @param [in] msr1 Unique MSR address.
/// @param [in] msr2 Unique MSR address.
void enable_fixed_counters(off_t *msrs_fixed_ctrs,
                           off_t msr1,
                           off_t msr2);

/// @brief Disable fixed-function counters by clearing enable bit in
/// IA32_FIXED_CTR_CTL.
///
/// @param [in] msrs_fixed_ctrs Array storing unique MSR addresses for fixed
///        counters.
/// @param [in] msr1 Unique MSR address.
/// @param [in] msr2 Unique MSR address.
void disable_fixed_counters(off_t *msrs_fixed_ctrs,
                            off_t msr1,
                            off_t msr2);

void dump_fixed_counter_data(FILE *writedest,
                             off_t *msrs_fixed_ctrs,
                             off_t perf_global,
                             off_t fixed_ctr_ctrl);

void print_fixed_counter_data(FILE *writedest,
                              off_t *msrs_fixed_ctrs,
                              off_t msr_perf_global_ctrl,
                              off_t msr_fixed_counter_ctrl);

void dump_perfmon_counter_data(FILE *writedest,
                               off_t *msrs_perfevtsel_ctrs,
                               off_t *msrs_perfmon_ctrs);

void print_perfmon_counter_data(FILE *writedest,
                                off_t *msrs_perfevtsel_ctrs,
                                off_t *msrs_perfmon_ctrs);

void dump_all_counter_data(FILE *writedest,
                           off_t *msrs_fixed_ctrs,
                           off_t msr_perf_global_ctrl,
                           off_t msr_fixed_counter_ctrl,
                           off_t *msrs_perfevtsel_ctrs,
                           off_t *msrs_perfmon_ctrs,
                           off_t *msrs_pcu_pmon_evtsel,
                           off_t *msrs_pcu_pmon_ctrs);

void print_all_counter_data(FILE *writedest,
                            off_t *msrs_fixed_ctrs,
                            off_t msr_perf_global_ctrl,
                            off_t msr_fixed_counter_ctrl,
                            off_t *msrs_perfevtsel_ctrs,
                            off_t *msrs_perfmon_ctrs,
                            off_t *msrs_pcu_pmon_evtsel,
                            off_t *msrs_pcu_pmon_ctrs);

/*************************************/
/* Programmable Performance Counters */
/*************************************/

/// @brief Set a performance event select counter to the same event on all
/// logical processors.
///
/// @param [in] cmask Count multiple event occurrences per cycle.
/// @param [in] flags Toggle additional options, such as user mode vs. OS mode,
///        enable PMI upon counter overflow, invert cmask, edge detection of
///        event occurrence, etc.
/// @param [in] umask Condition to be detected by the event logic unit.
/// @param [in] eventsel Unique event logic unit identifier.
/// @param [in] pmcnum Unique performance event select counter identifier.
/// @param [in] msrs_perfevtsel_ctrs Array of unique addresses for
///        PERFEVTSEL_CTRS.
void set_all_pmc_ctrl(uint64_t cmask,
                      uint64_t flags,
                      uint64_t umask,
                      uint64_t eventsel,
                      int pmcnum,
                      off_t *msrs_perfevtsel_ctrs);

int cpuid_num_pmc(void);

/// @brief Structure containing data of performance event select counters.
struct perfevtsel
{
    /// @brief Raw 64-bit value stored in IA32_PERFEVTSEL0.
    uint64_t **perf_evtsel0;
    /// @brief Raw 64-bit value stored in IA32_PERFEVTSEL1.
    uint64_t **perf_evtsel1;
    /// @brief Raw 64-bit value stored in IA32_PERFEVTSEL2.
    uint64_t **perf_evtsel2;
    /// @brief Raw 64-bit value stored in IA32_PERFEVTSEL3.
    uint64_t **perf_evtsel3;
    /// @brief Raw 64-bit value stored in IA32_PERFEVTSEL4.
    uint64_t **perf_evtsel4;
    /// @brief Raw 64-bit value stored in IA32_PERFEVTSEL5.
    uint64_t **perf_evtsel5;
    /// @brief Raw 64-bit value stored in IA32_PERFEVTSEL6.
    uint64_t **perf_evtsel6;
    /// @brief Raw 64-bit value stored in IA32_PERFEVTSEL7.
    uint64_t **perf_evtsel7;
};

/// @brief Structure containing data of general-purpose performance counters.
struct pmc
{
    /// @brief Raw 64-bit value stored in IA32_PMC0.
    uint64_t **pmc0;
    /// @brief Raw 64-bit value stored in IA32_PMC1.
    uint64_t **pmc1;
    /// @brief Raw 64-bit value stored in IA32_PMC2.
    uint64_t **pmc2;
    /// @brief Raw 64-bit value stored in IA32_PMC3.
    uint64_t **pmc3;
    /// @brief Raw 64-bit value stored in IA32_PMC4.
    uint64_t **pmc4;
    /// @brief Raw 64-bit value stored in IA32_PMC5.
    uint64_t **pmc5;
    /// @brief Raw 64-bit value stored in IA32_PMC6.
    uint64_t **pmc6;
    /// @brief Raw 64-bit value stored in IA32_PMC7.
    uint64_t **pmc7;
};

/// @brief Store the performance event select counter data on the heap.
///
/// @param [out] e Data for performance event select counters.
/// @param [in] msrs_perfevtsel_ctrs Array of unique addresses for
///        PERFEVTSEL_CTRS.
void perfevtsel_storage(struct perfevtsel **e,
                        off_t *msrs_perfevtsel_ctrs);

/// @brief Allocate storage for performance counters and reset values.
///
/// @param [in] msrs_perfevtsel_ctrs Array of unique addresses for
///        PERFEVTSEL_CTRS.
/// @param [in] msrs_perfmon_ctrs Array of unique addresses for
///        PERFMON_CTRS.
/// @return 0 if successful, else -1 if no general-purpose performance counters
/// are available.
int enable_pmc(off_t *msrs_perfevtsel_ctrs,
               off_t *msrs_perfmon_ctrs);

/// @brief Set a performance event select counter on a single logical processor.
///
/// @param [in] cmask Count multiple event occurrences per cycle.
/// @param [in] flags Toggle additional options, such as user mode vs. OS mode,
///        enable PMI upon counter overflow, invert cmask, edge detection of
///        event occurrence, etc.
/// @param [in] umask Condition to be detected by the event logic unit.
/// @param [in] eventsel Unique event logic unit identifier.
/// @param [in] pmcnum Unique performance event select counter identifier.
/// @param [in] thread Unique logical processor identifier.
/// @param [in] msrs_perfevtsel_ctrs Array of unique addresses for
///        PERFEVTSEL_CTRS.
void set_pmc_ctrl_flags(uint64_t cmask,
                        uint64_t flags,
                        uint64_t umask,
                        uint64_t eventsel,
                        int pmcnum,
                        unsigned thread,
                        off_t *msrs_perfevtsel_ctrs);

/// @brief Store the general-purpose performance counter data on the heap.
///
/// @param [out] p Data for general-purpose performance counters.
/// @param [in] msrs_perfmon_ctrs Array of unique addresses for
///        PERFMON_CTRS.
void pmc_storage(struct pmc **p,
                 off_t *msrs_perfmon_ctrs);

/// @brief Reset all performance counters for each logical processor.
///
/// @param [in] msrs_perfmon_ctrs Array of unique addresses for
///        PERFMON_CTRS.
void clear_all_pmc(off_t *msrs_perfmon_ctrs);

/// @brief Structure containing data of uncore performance event select
/// counters.
struct unc_perfevtsel
{
    /// @brief Raw 64-bit value stored in MSR_UNCORE_PERFEVTSEL0.
    uint64_t **c0;
    /// @brief Raw 64-bit value stored in MSR_UNCORE_PERFEVTSEL1.
    uint64_t **c1;
    /// @brief Raw 64-bit value stored in MSR_UNCORE_PERFEVTSEL2.
    uint64_t **c2;
    /// @brief Raw 64-bit value stored in MSR_UNCORE_PERFEVTSEL3.
    uint64_t **c3;
};

/// @brief Structure containing data of uncore general-purpose performance
/// counters.
struct unc_counters
{
    /// @brief Raw 64-bit value stored in MSR_UNCORE_PMC0.
    uint64_t **c0;
    /// @brief Raw 64-bit value stored in MSR_UNCORE_PMC1.
    uint64_t **c1;
    /// @brief Raw 64-bit value stored in MSR_UNCORE_PMC2.
    uint64_t **c2;
    /// @brief Raw 64-bit value stored in MSR_UNCORE_PMC3.
    uint64_t **c3;
};

///*************************************/
///* Uncore PCU Performance Monitoring */
///*************************************/

/// @brief Store the uncore performance event select counter data on the heap.
///
/// @param [out] uevt Pointer to data for uncore performance event select
///        counters.
/// @param [in] msrs_pcu_pmon_evtsel Array of unique addresses for
///        PCU_PMON_EVTSEL.
void unc_perfevtsel_storage(struct unc_perfevtsel **uevt,
                            off_t *msrs_pcu_pmon_evtsel);

/// @brief Store the uncore general-purpose performance counter data on the
/// heap.
///
/// @param [out] uc Pointer to data for uncore general-purpose performance
///        counters.
/// @param [in] msrs_pcu_pmon_ctrs Array of unique addresses for
///        PCU_PMON_CTRS.
void unc_counters_storage(struct unc_counters **uc,
                          off_t *msrs_pcu_pmon_ctrs);

/// @brief Allocate storage for uncore performance counters and reset values.
///
/// @param [in] msrs_pcu_pmon_evtsel Array of unique addresses for
///        PCU_PMON_EVTSEL.
/// @param [in] msrs_pcu_pmon_ctrs Array of unique addresses for
///        PCU_PMON_CTRS.
void enable_pcu(off_t *msrs_pcu_pmon_evtsel,
                off_t *msrs_pcu_pmon_ctrs);

/// @brief Reset all uncore performance counters for each socket.
///
/// @param [in] msrs_pcu_pmon_ctrs Array of unique addresses for
///        PCU_PMON_CTRS.
void clear_all_pcu(off_t *msrs_pcu_pmon_ctrs);

/// @brief Print out uncore performance counter data.
///
/// @param [in] writedest File stream where output will be written to.
/// @param [in] msrs_pcu_pmon_evtsel Array of unique addresses for
///        PCU_PMON_EVTSEL.
/// @param [in] msrs_pcu_pmon_ctrs Array of unique addresses for
///        PCU_PMON_CTRS.
void dump_unc_counter_data(FILE *writedest,
                           off_t *msrs_pcu_pmon_evtsel,
                           off_t *msrs_pcu_pmon_ctrs);

void print_unc_counter_data(FILE *writedest,
                            off_t *msrs_pcu_pmon_evtsel,
                            off_t *msrs_pcu_pmon_ctrs);

void get_all_power_data_fixed(FILE *writedest,
                              off_t msr_pkg_power_limit,
                              off_t msr_dram_power_limit,
                              off_t msr_rapl_unit,
                              off_t msr_package_energy_status,
                              off_t msr_dram_energy_status,
                              off_t *msrs_fixed_ctrs,
                              off_t msr_perf_global_ctrl,
                              off_t msr_fixed_counter_ctrl,
                              off_t msr_aperf,
                              off_t msr_mperf,
                              off_t msr_tsc);

#endif
