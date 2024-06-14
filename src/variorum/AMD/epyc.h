// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef EPYC_H_INCLUDE
#define EPYC_H_INCLUDE

#include <jansson.h>

int amd_cpu_epyc_get_power(
    int long_ver
);

int amd_cpu_epyc_get_power_limits(
    int long_ver
);

int amd_cpu_epyc_set_socket_power_limit(
    int pcap_new
);

int amd_cpu_epyc_set_and_verify_best_effort_node_power_limit(
    int pcap_new
);

int amd_cpu_epyc_print_energy(
    int long_ver
);

int amd_cpu_epyc_print_boostlimit(
    int long_ver
);

int amd_cpu_epyc_set_each_core_boostlimit(
    int boostlimit
);

//int amd_cpu_epyc_set_and_verify_core_boostlimit(
//    int core,
//    unsigned int boostlimit
//);

int amd_cpu_epyc_set_socket_boostlimit(
    int socket,
    int boostlimit
);

int amd_cpu_epyc_get_power_json(
    json_t *get_power_obj
);

int amd_cpu_epyc_get_node_power_domain_info_json(
    char **get_domain_obj_str
);

int amd_cpu_epyc_get_json_boostlimit(
    json_t *get_clock_obj_json
);

#endif
