// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef INSTINCTGPU_H_INCLUDE
#define INSTINCTGPU_H_INCLUDE

int instinctGPU_get_power(int verbose);
int instinctGPU_get_power_limit(int verbose);
int instinctGPU_get_thermals(int verbose);
int instinctGPU_get_clocks(int verbose);
int instinctGPU_get_gpu_utilization(int verbose);

#endif
