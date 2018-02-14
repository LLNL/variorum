#ifndef VARIORUM_H_INCLUDE
#define VARIORUM_H_INCLUDE

/// @brief Test for memory leaks.
int tester(void);

/// @brief Collect power limits and energy usage for both the package and DRAM
/// domains.
int poll_power(FILE *output);

/********************/
/* Set Power Limits */
/********************/
/// @brief Set identical power limits to all sockets within the node.
///
/// @param [in] socket_power_limit Desired power limit for each socket.
///
/// @return Error code.
int set_each_socket_power_limit(int socket_power_limit);

///// @brief Set a power limit to the node.
/////
///// @param [in] node_power_limit Desired power limit for the node.
/////
///// @return Error code.
//int set_node_power_limit(int node_power_limit);
//
///// @brief Set a power limit to the node domain.
/////
///// @param [in] gpu_power_limit Desired power limit for each GPU.
/////
///// @return Error code.
//int set_each_gpu_power_limit(int gpu_power_limit);

/************************/
/* Print/Dump Functions */
/************************/
/// @brief Print power limits for all known domains in long format.
///
/// @return Error code.
int print_power_limits(void);

/// @brief Print power limits for all known domains in CSV format.
///
/// @return Error code.
int dump_power_limits(void);

/// @brief Print thermal data in long format.
///
/// @return Error code.
int print_thermals(void);

/// @brief Print thermal data in CSV format.
///
/// @return Error code.
int dump_thermals(void);

/// @brief Print performance counter data in long format.
///
/// @return Error code.
int print_counters(void);

/// @brief Print performance counter data in CSV format.
///
/// @return Error code.
int dump_counters(void);

/// @brief Print energy and power usage data in long format.
///
/// @return Error code.
int print_power(void);

/// @brief Print energy and power usage data in CSV format.
///
/// @return Error code.
int dump_power(void);

///// @brief Print desired frequency setting (i.e., upper bound on the operating
///// frequency; the processor may grant up to this frequency) in long format.
/////
///// @return Error code.
//int print_cap_frequency(void);
//
///// @brief Print desired frequency setting (i.e., upper bound on the operating
///// frequency; the processor may grant up to this frequency) in CSV format.
/////
///// @return Error code.
//int dump_cap_frequency(void);

/// @brief Print current operating frequency (APERF/MPERF and PERF_STATUS) in
//  long format.
///
/// @return Error code.
int print_clock_speed(void);

/// @brief Print current operating frequency (APERF/MPERF and PERF_STATUS) in
/// CSV format.
///
/// @return Error code.
int dump_clock_speed(void);

///// @todo Print discrete frequencies from P0 to Pn?
/////
///// @return Error code.
//int print_available_frequencies(void);

/// @brief Print if hyperthreading is enabled or disabled.
///
/// @return Error code.
int dump_hyperthreading(void);

///// @todo Print discrete frequencies in turbo range (i.e., max turbo ratio)?
/////
///// @return Error code.
//int print_turbo(void);

/// @brief Print architecture toplogy in long format.
///
/// @return Error code.
int print_topology(void);

/// @brief Print list of features available on a particular architecture.
///
/// @return Error code.
int print_features(void);

/// @brief Print if turbo is enabled or disabled.
///
/// @return Error code.
int dump_turbo(void);

///***************************/
///* Enable/Disable Features */
///***************************/
///// @brief Enable turbo feature.
/////
///// @return Error code.
int enable_turbo(void);
//
///// @brief Disable turbo feature.
/////
///// @return Error code.
int disable_turbo(void);

///* Do we need these? */
//int dump_cap_package_frequency(void);
//int dump_available_frequencies(void);


#endif
