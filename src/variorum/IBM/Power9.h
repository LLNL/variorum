// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER9_H_INCLUDE
#define POWER9_H_INCLUDE

#include <jansson.h>
#include <pthread.h>

#include <variorum_timers.h>

struct thread_args
{
    unsigned long sample_interval;
    unsigned long energy_acc;
};

int ibm_cpu_p9_get_power(
    int long_ver
);

int ibm_cpu_p9_get_energy(
    int long_ver
);

int ibm_cpu_p9_get_power_limits(
    int long_ver
);

int ibm_cpu_p9_cap_socket_power_limit(
    int long_ver
);

int ibm_cpu_p9_cap_and_verify_node_power_limit(
    int pcap_new
);

int ibm_cpu_p9_cap_gpu_power_ratio(
    int gpu_power_ratio
);

int ibm_cpu_p9_monitoring(
    FILE *output
);

int ibm_cpu_p9_get_power_json(
    json_t *get_power_obj
);

int ibm_cpu_p9_get_node_power_domain_info_json(
    char **get_domain_obj_str
);

int ibm_cpu_p9_get_node_thermal_json(
    json_t *get_thermal_obj
);

int ibm_cpu_p9_get_node_frequency_json(
    json_t *get_frequency_obj_json
);

/* Sampling functions for get_energy thread implementation */
void *power_measurement(
    void *arg
);

unsigned long take_measurement(
    int fd
);

int ibm_cpu_p9_get_node_energy_json(
    json_t *get_energy_obj
);

#endif
