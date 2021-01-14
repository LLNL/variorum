// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef ARM_H_INCLUDE
#define ARM_H_INCLUDE

int arm_get_power(int long_ver);

int arm_get_thermals(int long_ver);

int arm_get_clocks(int long_ver);

int arm_get_frequencies(void);

#endif
