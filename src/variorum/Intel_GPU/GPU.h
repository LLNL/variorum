// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef INTEL_GPU_H_INCLUDE
#define INTEL_GPU_H_INCLUDE

#include <jansson.h>

extern int intel_gpu_get_power(
    int long_ver
);

extern int intel_gpu_get_thermals(
    int long_ver
);

extern int intel_gpu_get_clocks(
    int long_ver
);

extern int intel_gpu_cap_each_gpu_power_limit(
    unsigned int powerlimit
);

extern int intel_gpu_get_power_limit(
    int long_ver
);


extern int intel_gpu_get_energy(
    int long_ver
);

extern int intel_gpu_get_energy_json(
    json_t *get_energy_obj_str
);

#endif
