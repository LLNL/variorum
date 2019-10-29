// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef POWER9_H_INCLUDE
#define POWER9_H_INCLUDE

int p9_get_power(int long_ver);

int p9_get_power_limits(int long_ver);

int p9_set_socket_power_limit(int pcap_new);

int p9_set_node_power_limit(int pcap_new);

int p9_set_and_verify_node_power_limit(int pcap_new);

int p9_set_gpu_power_ratio(int gpu_power_ratio);

int p9_monitoring(FILE *output);

#endif
