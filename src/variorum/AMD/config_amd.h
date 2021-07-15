// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_AMD_H_INCLUDE
#define CONFIG_AMD_H_INCLUDE

#include <inttypes.h>
#include <e_smi/e_smi.h>

uint64_t *detect_amd_arch(void);

int set_amd_func_ptrs(void);

#endif
