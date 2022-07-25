// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_AMD_GPU_H_INCLUDE
#define CONFIG_AMD_GPU_H_INCLUDE

#include <inttypes.h>

uint64_t *detect_amd_gpu_arch(void);

int set_amd_gpu_func_ptrs(void);

#endif
