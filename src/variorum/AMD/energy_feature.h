// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <linux/types.h>
#include <stdint.h>
#include <sys/types.h>

struct rapl_units
{
    /// @brief Raw 64-bit value stored in MSR_RAPL_POWER_UNIT.
    uint64_t msr_rapl_power_unit;
    /// @brief Energy status units (ESU) based on the multiplier 1/(2^ESU) (in
    /// Joules). ESU is encoded in bits 12:8 of MSR_RAPL_POWER_UNIT.
    double joules;
};

struct EPYC_19h_offsets
{
    const off_t msr_rapl_power_unit;
    const off_t msr_core_energy_stat;
    const off_t msr_pkg_energy_stat;
};

struct rapl_data
{
    uint64_t **core_bits;
    double *core_joules;
};

int print_energy_data(FILE *writedest, off_t msr_rapl_unit,
                      off_t msr_core_energy_status);
