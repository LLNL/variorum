// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef ARM_CORTEX_A72_H_INCLUDE
#define ARM_CORTEX_A72_H_INCLUDE

#include <jansson.h>

int arm_cortex_a72_get_power(int long_ver);

int arm_cortex_a72_get_thermals(int long_ver);

int arm_cortex_a72_get_clocks(int long_ver);

int arm_cortex_a72_get_frequencies(void);

int arm_cortex_a72_cap_socket_frequency(int cpuid, int freq);

int arm_cortex_a72_get_power_json(char **get_power_obj_str);

int arm_cortex_a72_get_power_domain_info_json(char **get_domain_obj_str);

#endif
