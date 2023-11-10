// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_ARM_H_INCLUDE
#define CONFIG_ARM_H_INCLUDE

#include <inttypes.h>

uint64_t *detect_arm_arch(void);

int set_arm_func_ptrs(int idx);

#endif
