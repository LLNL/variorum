// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER9_H_INCLUDE
#define POWER9_H_INCLUDE

#include <jansson.h>

int p9_get_power(int long_ver);

int p9_get_power_limits(int long_ver);

int p9_cap_socket_power_limit(int long_ver);

int p9_cap_and_verify_node_power_limit(int pcap_new);

int p9_cap_gpu_power_ratio(int gpu_power_ratio);

int p9_monitoring(FILE *output);

int p9_get_node_power_json(char **get_power_obj_str);

int p9_get_node_power_domain_info_json(char **get_domain_obj_str);

#endif
