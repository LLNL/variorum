// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_NVIDIA_H_INCLUDE
#define CONFIG_NVIDIA_H_INCLUDE

#include <inttypes.h>

#include <power_features.h>
#include <Volta.h>

uint64_t *detect_gpu_arch(void);

int set_nvidia_func_ptrs(void);

#endif
