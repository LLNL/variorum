// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef VARIORUM_H_INCLUDE
#define VARIORUM_H_INCLUDE

#include <stdio.h>
#include <jansson.h>

/// @brief Collect power limits and energy usage for both the package and DRAM
/// domains.
///
/// @supparch
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @param [in] output Location for output.
///
/// @return Error code.
int variorum_poll_power(FILE *output);

/// @brief Collect power limits and energy usage for both the package and DRAM
/// domains, fixed counters, TSC, APERF, and MPERF.
///
/// @supparch
/// - IBM P9
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @param [in] output Location for output.
///
/// @return Error code.
int variorum_monitoring(FILE *output);

/*****************/
/* Cap Functions */
/*****************/
/// @brief Cap the power limits for all sockets within the node.
///
/// @param [in] socket_power_limit Desired power limit for each socket.
///
/// @supparch
/// - IBM P9
/// - AMD EPYC Milan
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @return Error code.
int variorum_cap_each_socket_power_limit(int socket_power_limit);

/// @brief Cap the power limit of the node.
///
/// @supparch
/// - IBM P9
/// - AMD EPYC Milan
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @param [in] node_power_limit Desired power limit for the node.
///
/// @return Error code.
int variorum_cap_best_effort_node_power_limit(int node_power_limit);

/// @brief Cap the power shifting ratio for the GPU (uniform on both sockets).
///
/// @param [in] gpu_power_ratio Desired power ratio (percentage).
/// for the processor and GPU.
///
/// @supparch
/// - IBM P9 (same ratio on both sockets)
///
/// @note Only valid on IBM P9 systems for now. Same ratio on both sockets.
///
/// @return Error code.
int variorum_cap_gpu_power_ratio(int gpu_power_ratio);

/// @brief Cap the CPU frequency for all cores within a socket.
///
/// @supparch
/// - AMD EPYC Milan
/// - Intel Skylake
///
/// @param [in] cpu_freq_mhz Desired CPU frequency for each core in MHz.
///
/// @return Error code.
int variorum_cap_each_core_frequency_limit(int cpu_freq_mhz);

///// @brief Cap the power limit of the GPU domain.
/////
///// @param [in] gpu_power_limit Desired power limit for each GPU.
/////
///// @return Error code.
//int cap_each_gpu_power_limit(int gpu_power_limit);

/// @brief Cap the frequency of the target processor.
///
/// @supparch
/// - ARM Juno v2
/// - AMD EPYC Milan
///
/// @param [in] socketid Target socket ID.
/// @param [in] socket_freq_mhz Desired socket frequency in MHz.
///
/// @return Error code.
int variorum_cap_socket_frequency_limit(int socketid, int socket_freq_mhz);

/*******************/
/* Print Functions */
/*******************/
/// @brief Print power limits for all known domains in long format.
///
/// @return Error code.
int variorum_print_verbose_power_limit(void);

/// @brief Print power limits for all known domains in CSV format.
///
/// @supparch
/// - IBM P9
/// - AMD EPYC Milan
/// - NVIDIA Volta
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
/// - Intel Ice Lake
///
/// @return Error code.
int variorum_print_power_limit(void);

/// @brief Print thermal data in long format.
///
/// @return Error code.
int variorum_print_verbose_thermals(void);

/// @brief Print thermal data in CSV format.
///
/// @supparch
/// - ARM Juno v2
/// - NVIDIA Volta
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @return Error code.
int variorum_print_thermals(void);

/// @brief Print performance counter data in long format.
///
/// @return Error code.
int variorum_print_verbose_counters(void);

/// @brief Print performance counter data in CSV format.
///
/// @supparch
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @return Error code.
int variorum_print_counters(void);

/// @brief Print energy and power usage data in long format.
///
/// @return Error code.
int variorum_print_verbose_power(void);

/// @brief Print energy and power usage data in CSV format.
///
/// @supparch
/// - IBM P9
/// - ARM Juno v2
/// - AMD EPYC Milan
/// - NVIDIA Volta
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
/// - Intel Ice Lake
///
/// @return Error code.
int variorum_print_power(void);

/// @brief Print current operating frequency (APERF/MPERF and PERF_STATUS) in
/// long format.
///
/// @return Error code.
int variorum_print_verbose_frequency(void);

/// @brief Print current operating frequency (APERF/MPERF and PERF_STATUS) in
/// CSV format.
///
/// @supparch
/// - ARM Juno v2
/// - AMD EPYC Milan
/// - NVIDIA Volta
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @return Error code.
int variorum_print_frequency(void);

/// @brief Print if hyperthreading is enabled or disabled.
///
/// @return Error code.
int variorum_print_hyperthreading(void);

/// @brief Print architecture topology in long format.
void variorum_print_topology(void);

/// @brief Print list of features available on a particular architecture.
///
/// @supparch
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
/// - Intel Ice Lake
///
/// @return Error code.
int variorum_print_features(void);

/// @brief Print if turbo is enabled or disabled. If enabled, then print
/// discrete frequencies in turbo range (i.e., max turbo ratio).
///
/// @supparch
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
///
/// @return Error code.
int variorum_print_turbo(void);

/// @brief Print verbose GPU streaming multi-processor and memory utilization.
///
/// @return Error code.
int variorum_print_verbose_gpu_utilization(void);

/// @brief Print CSV-formatted GPU streaming multi-processor and memory utilization.
///
/// @supparch
/// - NVIDIA Volta
///
/// @return Error code.
int variorum_print_gpu_utilization(void);

/// @brief Print list of available frequencies from p-states, turbo, AVX, etc. ranges.
///
/// @supparch
/// - ARM Juno v2
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @return Error code.
int variorum_print_available_frequencies(void);

/// @brief Print if core and socket energy is available.
///
/// @supparch
/// - AMD EPYC Milan
///
/// @return Error code.
int variorum_print_energy(void);

/***************************/
/* Enable/Disable Features */
/***************************/
/// @brief Enable turbo feature.
///
/// @supparch
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
///
/// @return Error code.
int variorum_enable_turbo(void);

/// @brief Disable turbo feature.
///
/// @supparch
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
///
/// @return Error code.
int variorum_disable_turbo(void);

/****************/
/* JSON Support */
/****************/
/// @brief Populate json_t object parameter with total node power.
///
/// @supparch
/// - IBM P9
/// - ARM Juno v2
/// - AMD EPYC Milan
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @return Error code.
int variorum_get_node_power_json(json_t *get_power_obj);

/// @brief Populate json_t object parameter with measurable power domains.
///
/// @supparch
/// - IBM P9
/// - ARM Juno v2
/// - AMD EPYC Milan
/// - Intel Sandy Bridge
/// - Intel Ivy Bridge
/// - Intel Haswell
/// - Intel Broadwell
/// - Intel Skylake
/// - Intel Kaby Lake
///
/// @return Error code.
int variorum_get_node_power_domain_info_json(json_t *get_domain_obj);

/***********/
/* Testing */
/***********/
/// @brief Test for memory leaks.
int variorum_tester(void);

#endif
