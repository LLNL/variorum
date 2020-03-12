// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef VARIORUM_H_INCLUDE
#define VARIORUM_H_INCLUDE

#include <stdio.h>

/// @brief Collect power limits and energy usage for both the package and DRAM
/// domains.
///
/// @param [in] output Location for output.
///
/// @return Error code.
int variorum_poll_power(FILE *output);

/// @brief Collect power limits and energy usage for both the package and DRAM
/// domains, fixed counters, TSC, APERF, and MPERF.
///
/// @param [in] output Location for output.
///
/// @return Error code.
int variorum_monitoring(FILE *output);

/********************/
/* Set Power Limits */
/********************/
/// @brief Set identical power limits to all sockets within the node.
///
/// @param [in] socket_power_limit Desired power limit for each socket.
///
/// @return Error code.
int variorum_set_each_socket_power_limit(int socket_power_limit);

/// @brief Set a power limit to the node and verify if it was set correctly (10ms delay).
///
/// @param [in] node_power_limit Desired power limit for the node.
///
/// @return Error code.
int variorum_set_and_verify_node_power_limit(int node_power_limit);

/// @brief Set a power limit to the node.
///
/// @param [in] node_power_limit Desired power limit for the node.
///
/// @return Error code.
int variorum_set_node_power_limit(int node_power_limit);

/// @brief Set the power shifting ratio to the GPU (uniform on both sockets).
///
/// @param [in] gpu_power_ratio Desired power ratio (percentage) for the processor and GPU.
///
/// @note Only valid on IBM P9 systems for now. Same ratio will be set on both sockets.
/////
///// @return Error code.
int variorum_set_gpu_power_ratio(int gpu_power_ratio);

/// @brief Set identical CPU frequencies to all cores within a socket.
///
/// @param [in] cpu_freq_mhz Desired CPU frequency for each core in MHz.
///
/// @return Error code.
int variorum_set_each_core_frequency(int cpu_freq_mhz);

///// @brief Set a power limit to the gpu domain.
/////
///// @param [in] gpu_power_limit Desired power limit for each GPU.
/////
///// @return Error code.
//int set_each_gpu_power_limit(int gpu_power_limit);

/*******************/
/* Print Functions */
/*******************/
/// @brief Print power limits for all known domains in long format.
///
/// @return Error code.
int variorum_print_verbose_power_limits(void);

/// @brief Print power limits for all known domains in CSV format.
///
/// @return Error code.
int variorum_print_power_limits(void);

/// @brief Print thermal data in long format.
///
/// @return Error code.
int variorum_print_verbose_thermals(void);

/// @brief Print thermal data in CSV format.
///
/// @return Error code.
int variorum_print_thermals(void);

/// @brief Print performance counter data in long format.
///
/// @return Error code.
int variorum_print_verbose_counters(void);

/// @brief Print performance counter data in CSV format.
///
/// @return Error code.
int variorum_print_counters(void);

/// @brief Print energy and power usage data in long format.
///
/// @return Error code.
int variorum_print_verbose_power(void);

/// @brief Print energy and power usage data in CSV format.
///
/// @return Error code.
int variorum_print_power(void);

/// @brief Print current operating frequency (APERF/MPERF and PERF_STATUS) in
/// long format.
///
/// @return Error code.
int variorum_print_verbose_clock_speed(void);

/// @brief Print current operating frequency (APERF/MPERF and PERF_STATUS) in
/// CSV format.
///
/// @return Error code.
int variorum_print_clock_speed(void);

/// @todo Print discrete frequencies from P0 to Pn.
///
/// @return Error code.
int variorum_print_available_frequencies(void) { return 0; };

/// @brief Print if hyperthreading is enabled or disabled.
///
/// @return Error code.
int variorum_print_hyperthreading(void);

/// @brief Print architecture topology in long format.
///
/// @return Error code.
int variorum_print_topology(void);

/// @brief Print list of features available on a particular architecture.
///
/// @return Error code.
int variorum_print_features(void);

/// @brief Print if turbo is enabled or disabled. If enabled, then print
/// discrete frequencies in turbo range (i.e., max turbo ratio).
///
/// @return Error code.
int variorum_print_turbo(void);

/// @brief Print verbose GPU streaming multi-processor and memory utilization.
///
/// @return Error code.
int variorum_print_verbose_gpu_utilization(void);

/// @brief Print CSV-formatted GPU streaming multi-processor and memory utilization.
///
/// @return Error code.
int variorum_print_gpu_utilization(void);

/***************************/
/* Enable/Disable Features */
/***************************/
/// @brief Enable turbo feature.
///
/// @return Error code.
int variorum_enable_turbo(void);

/// @brief Disable turbo feature.
///
/// @return Error code.
int variorum_disable_turbo(void);

/// @brief Test for memory leaks.
int variorum_tester(void);

///* Do we need these? */
//int print_cap_package_frequency(void);
//int print_available_frequencies(void);

///// @brief Print desired frequency setting (i.e., upper bound on the operating
///// frequency; the processor may grant up to this frequency) in long format.
/////
///// @return Error code.
//int print_verbose_cap_frequency(void);

///// @brief Print desired frequency setting (i.e., upper bound on the operating
///// frequency; the processor may grant up to this frequency) in CSV format.
/////
///// @return Error code.
//int print_cap_frequency(void);

#endif
