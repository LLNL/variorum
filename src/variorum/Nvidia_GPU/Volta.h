// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef VOLTA_H_INCLUDE
#define VOLTA_H_INCLUDE

#include <jansson.h>

int volta_get_power(
    int long_ver
);

int volta_get_thermals(
    int long_ver
);

int volta_get_clocks(
    int long_ver
);

int volta_get_power_limits(
    int long_ver
);

int volta_get_gpu_utilization(
    int long_ver
);

int volta_cap_each_gpu_power_limit(
    unsigned int powerlimit
);

int volta_get_power_json(
    json_t *get_power_obj_str
);

int volta_get_thermals_json(
    json_t *get_thermal_obj
);

int volta_get_clocks_json(
    json_t *get_clock_obj_json
);

int volta_get_gpu_utilization_json(
    char **get_gpu_util_obj_str
);

#endif
