// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef INSTINCTGPU_H_INCLUDE
#define INSTINCTGPU_H_INCLUDE

#include <jansson.h>
#include <sys/time.h>

int amd_gpu_instinct_get_power(int verbose);

int amd_gpu_instinct_get_power_limit(int verbose);

int amd_gpu_instinct_get_thermals(int verbose);

int amd_gpu_instinct_get_clocks(int verbose);

int amd_gpu_instinct_get_gpu_utilization(int verbose);

int amd_gpu_instinct_cap_each_gpu_power_limit(unsigned int powerlimit);
int amd_gpu_instinct_get_gpu_power_json(char **get_power_obj_str);

int amd_gpu_instinct_get_thermals_json(json_t *get_thermal_obj);

#endif
