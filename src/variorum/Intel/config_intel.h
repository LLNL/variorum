// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_INTEL_H_INCLUDE
#define CONFIG_INTEL_H_INCLUDE

#include <inttypes.h>

uint64_t *detect_intel_arch(void);

int set_intel_func_ptrs(void);

int gpu_power_ratio_unimplemented();

#endif
