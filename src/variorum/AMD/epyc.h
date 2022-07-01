// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef EPYC_H_INCLUDE
#define EPYC_H_INCLUDE

#include <jansson.h>

int epyc_get_power(int long_ver);

int epyc_get_power_limits(int long_ver);

int epyc_set_socket_power_limit(int pcap_new);

int epyc_set_and_verify_best_effort_node_power_limit(int pcap_new);

int epyc_print_energy();

int epyc_print_boostlimit();

int epyc_set_each_core_boostlimit(int boostlimit);

//int epyc_set_and_verify_core_boostlimit(int core, unsigned int boostlimit);

int epyc_set_socket_boostlimit(int socket, int boostlimit);

int epyc_get_node_power_json(char **get_power_obj_str);

int epyc_get_node_power_domain_info_json(char **get_domain_obj_str);

#endif
