// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER_FEATURES_CORTEX_A72_H_INCLUDE
#define POWER_FEATURES_CORTEX_A72_H_INCLUDE

#include <stdint.h>
#include <stdio.h>
#include <jansson.h>
#include <arm_util.h>

int arm_cpu_cortex_a72_get_power_data(int verbose, FILE *output);

int arm_cpu_cortex_a72_get_thermal_data(int verbose, FILE *output);

int arm_cpu_cortex_a72_get_frequencies(int chipid, FILE *output);

int arm_cpu_cortex_a72_get_clocks_data(int chipid, int verbose, FILE *output);

int arm_cpu_cortex_a72_cap_socket_frequency(int socketid, int new_freq);

int arm_cpu_cortex_a72_json_get_power_data(json_t *get_power_obj);

int arm_cpu_cortex_a72_json_get_power_domain_info(json_t *get_domain_obj);


#endif
