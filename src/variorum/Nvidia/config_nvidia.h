// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_NVIDIA_H_INCLUDE
#define CONFIG_NVIDIA_H_INCLUDE

#include <inttypes.h>

uint64_t *detect_gpu_arch(void);

int set_nvidia_func_ptrs(void);

#endif
