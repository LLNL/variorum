// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_INTEL_GPU_H_INCLUDE
#define CONFIG_INTEL_GPU_H_INCLUDE

#include <inttypes.h>

#include <power_features.h>
#include <GPU.h>

extern uint64_t *detect_intel_gpu_arch(void);

extern int set_intel_gpu_func_ptrs(void);

#endif
