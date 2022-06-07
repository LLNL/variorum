// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER_FEATURES_H_INCLUDE
#define POWER_FEATURES_H_INCLUDE

#include <stdint.h>
#include <stdio.h>

#include <nvml.h>

unsigned m_total_unit_devices;
nvmlDevice_t *m_unit_devices_file_desc;
unsigned m_gpus_per_socket;
char m_hostname[1024];

void initNVML(void);

void shutdownNVML(void);

void dump_power_data(int chipid, int verbose, FILE *output);

void dump_thermal_data(int chipid, int verbose, FILE *output);

void dump_power_limits(int chipid, int verbose, FILE *output);

void dump_clocks_data(int chipid, int verbose, FILE *output);

void dump_gpu_utilization(int chipid, int verbose, FILE *output);

void get_power_data(int chipid, int verbose, FILE *output);

void get_thermal_data(int chipid, int verbose, FILE *output);

void get_clocks_data(int chipid, int verbose, FILE *output);

void get_power_limits(int chipid, int verbose, FILE *output);

void get_gpu_utilization(int chipid, int verbose, FILE *output);

#endif
