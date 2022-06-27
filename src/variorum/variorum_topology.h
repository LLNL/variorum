// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef VARIORUM_TOPOLOGY_H_INCLUDE
#define VARIORUM_TOPOLOGY_H_INCLUDE

#include <stdio.h>
#include <hwloc.h>

extern hwloc_topology_t topology;

int variorum_init_topology(void);

int variorum_get_num_sockets(void);

int variorum_get_num_cores(void);

int variorum_get_num_threads(void);

#endif
