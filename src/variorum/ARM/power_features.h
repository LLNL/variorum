// Copyright 2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER_FEATURES_H_INCLUDE
#define POWER_FEATURES_H_INCLUDE

#include <stdint.h>
#include <stdio.h>

unsigned m_num_package;
char m_hostname[1024];

void init_arm(void);

void shutdown_arm(void);

int get_power_data(int verbose, FILE *output);

int get_thermal_data(int verbose, FILE *output);

int get_frequencies(int chipid, FILE *output);

int get_clocks_data(int chipid, int verbose, FILE *output);

int cap_socket_frequency(int socketid, int new_freq);

#endif
