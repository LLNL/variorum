// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef INSTINCTGPU_H_INCLUDE
#define INSTINCTGPU_H_INCLUDE

int amd_gpu_instinct_get_power(int verbose);
int amd_gpu_instinct_get_power_limit(int verbose);
int amd_gpu_instinct_get_thermals(int verbose);
int amd_gpu_instinct_get_clocks(int verbose);
int amd_gpu_instinct_get_gpu_utilization(int verbose);
int amd_gpu_instinct_cap_each_gpu_power_limit(unsigned int powerlimit);

#endif
