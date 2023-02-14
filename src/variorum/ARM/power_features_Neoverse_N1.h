// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER_FEATURES_NEOVERSE_N1_H_INCLUDE
#define POWER_FEATURES_NEOVERSE_N1_H_INCLUDE

#include <stdint.h>
#include <stdio.h>
#include <jansson.h>
#include <arm_util.h>

#define NUM_CORES 80

int arm_cpu_neoverse_n1_get_power_data(int verbose, FILE *output);

int arm_cpu_neoverse_n1_get_thermal_data(int verbose, FILE *output);

int arm_cpu_neoverse_n1_get_clocks_data(int chipid, int verbose, FILE *output);

int arm_cpu_neoverse_n1_cap_socket_frequency(int socketid, int new_freq);

int arm_cpu_neoverse_n1_json_get_power_data(json_t *get_power_obj);

int arm_cpu_neoverse_n1_json_get_power_domain_info(json_t *get_domain_obj);


#endif
