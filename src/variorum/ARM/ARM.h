// Copyright 2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef ARM_H_INCLUDE
#define ARM_H_INCLUDE

int arm_get_power(int long_ver);

int arm_get_thermals(int long_ver);

int arm_get_clocks(int long_ver);

int arm_get_frequencies(void);

int arm_cap_socket_frequency(int cpuid, int freq);

#endif
