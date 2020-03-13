// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER_FEATURES_H_INCLUDE
#define POWER_FEATURES_H_INCLUDE

#include <stdint.h>
#include <stdio.h>

#include <nvml.h>

unsigned int m_total_unit_devices;
nvmlDevice_t* m_unit_devices_file_desc;

void initNVML(void);

void shutdownNVML(void);

void dump_power_data(FILE *writedest, int verbose);

void dump_thermal_data(FILE *writedest, int verbose);

void dump_power_limits(FILE *writedest, int verbose);

void dump_clocks_data(FILE *writedest, int verbose);

void dump_gpu_utilization(FILE *writedest, int verbose);

#endif
