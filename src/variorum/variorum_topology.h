// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef VARIORUM_TOPOLOGY_H_INCLUDE
#define VARIORUM_TOPOLOGY_H_INCLUDE

#include <stdio.h>
#include <hwloc.h>

extern hwloc_topology_t topology;

/// @brief Initialize hwloc topology object.
///
/// @return 0 if initialization is successful, otherwise -1
int variorum_init_topology(void);

/// @brief Get number of sockets on the hardware platform.
///
/// @return Number of sockets, otherwise -1
int variorum_get_num_sockets(void);

/// @brief Get number of cores on the hardware platform.
///
/// @return Number of cores, otherwise -1
int variorum_get_num_cores(void);

/// @brief Get number of threads on the hardware platform.
///
/// @return Number of threads, otherwise -1
int variorum_get_num_threads(void);

#endif
