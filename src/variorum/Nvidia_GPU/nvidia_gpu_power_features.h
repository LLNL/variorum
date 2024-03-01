// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER_FEATURES_H_INCLUDE
#define POWER_FEATURES_H_INCLUDE

#include <jansson.h>
#include <nvml.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

extern unsigned m_total_unit_devices;
extern nvmlDevice_t *m_unit_devices_file_desc;
extern unsigned m_gpus_per_socket;
extern char m_hostname[1024];

void initNVML(
    void
);

void shutdownNVML(
    void
);

void nvidia_gpu_get_power_data(
    int chipid,
    int verbose,
    FILE *output
);

void nvidia_gpu_get_thermal_data(
    int chipid,
    int verbose,
    FILE *output
);

void nvidia_gpu_get_clocks_data(
    int chipid,
    int verbose,
    FILE *output
);

void nvidia_gpu_get_power_limits_data(
    int chipid,
    int verbose,
    FILE *output
);

void nvidia_gpu_get_gpu_utilization_data(
    int chipid,
    int verbose,
    FILE *output
);

void cap_each_gpu_power_limit(
    int chipid,
    unsigned int powerlimit
);

void nvidia_gpu_get_thermal_json(
    int chipid,
    json_t *output
);

void nvidia_gpu_get_clocks_json(
    int chipid,
    json_t *output
);

void nvidia_get_gpu_utilization_json(
    int chipid,
    json_t *get_gpu_util_obj
);


void nvidia_gpu_get_power_json(
    int chipid,
    json_t *output
);

#endif
