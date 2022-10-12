// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_IBM_H_INCLUDE
#define CONFIG_IBM_H_INCLUDE

#include <inttypes.h>

uint64_t *detect_ibm_arch(void);

int set_ibm_func_ptrs(int idx);

#endif
