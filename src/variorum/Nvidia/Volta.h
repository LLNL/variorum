// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef VOLTA_H_INCLUDE
#define VOLTA_H_INCLUDE

int volta_get_power(int long_ver);

int volta_get_thermals(int long_ver);

int volta_get_clocks(int long_ver);

int volta_get_power_limits(int long_ver);

int volta_get_gpu_utilization(int long_ver);

#endif
