// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef ARM_JUNO_R2_H_INCLUDE
#define ARM_JUNO_R2_H_INCLUDE

#include <jansson.h>

int arm_juno_r2_get_power(int long_ver);

int arm_juno_r2_get_thermals(int long_ver);

int arm_juno_r2_get_clocks(int long_ver);

int arm_juno_r2_get_frequencies(void);

int arm_juno_r2_cap_socket_frequency(int cpuid, int freq);

int arm_juno_r2_get_power_json(char **get_power_obj_str);

int arm_juno_r2_get_power_domain_info_json(char **get_domain_obj_str);

#endif
