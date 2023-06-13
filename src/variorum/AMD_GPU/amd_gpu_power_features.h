// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER_FEATURES_H_INCLUDE
#define POWER_FEATURES_H_INCLUDE

#include <stdint.h>
#include <stdio.h>

#include <rocm_smi/rocm_smi.h>

void get_power_data(int chipid, int total_sockets, int verbose, FILE *output);
void get_power_limit_data(int chipid, int total_sockets, int verbose,
                          FILE *output);
void get_thermals_data(int chipid, int total_sockets, int verbose,
                       FILE *output);
void get_clocks_data(int chipid, int total_sockets, int verbose, FILE *output);
void get_gpu_utilization_data(int chipid, int total_sockets, int verbose,
                              FILE *output);
void cap_each_gpu_power_limit(int chipid, int total_sockets,
                              unsigned int powerlimit);

#endif
