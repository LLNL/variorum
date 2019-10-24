// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CPUID_H_INCLUDE
#define CPUID_H_INCLUDE

#include <inttypes.h>

void cpuid(uint64_t leaf,
           uint64_t *rax,
           uint64_t *rbx,
           uint64_t *rcx,
           uint64_t *rdx);

#endif
