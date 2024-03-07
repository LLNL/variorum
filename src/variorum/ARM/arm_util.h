// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef ARM_UTIL_H_INCLUDE
#define ARM_UTIL_H_INCLUDE

#include <stdint.h>
#include <stdio.h>

#include <config_architecture.h>

extern unsigned int m_num_package;
extern char m_hostname[1024];

int read_file_ui64(
    const int file,
    uint64_t *val
);

int write_file_ui64(
    const int file,
    uint64_t val
);

int read_array_ui64(
    const int fd,
    uint64_t **array
);

void init_arm(
    void
);

void shutdown_arm(
    void
);

#endif
