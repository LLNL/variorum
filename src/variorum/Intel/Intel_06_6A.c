// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <Intel_06_6A.h>
#include <clocks_features.h>
#include <config_architecture.h>
#include <counters_features.h>
#include <intel_power_features.h>
#include <thermal_features.h>

static struct icelake_6a_offsets msrs =
{
    .msr_platform_info            = 0xCE,
    .ia32_time_stamp_counter      = 0x10,
    .msr_rapl_power_unit          = 0x606,
    .msr_pkg_power_limit          = 0x610,
    .msr_pkg_energy_status        = 0x611,
    .msr_pkg_power_info           = 0x614,
    .msr_dram_power_limit         = 0x618,
    .msr_dram_energy_status       = 0x619,
    .msr_dram_power_info          = 0x61C,
};

int intel_cpu_fm_06_6a_get_power_limits(int long_ver)
{
    unsigned socket;
    unsigned nsockets, ncores, nthreads;
#ifdef VARIORUM_WITH_INTEL_CPU
    variorum_get_topology(&nsockets, &ncores, &nthreads, P_INTEL_CPU_IDX);
#endif

    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    for (socket = 0; socket < nsockets; socket++)
    {
        if (long_ver == 0)
        {
            print_package_power_limit(stdout, msrs.msr_pkg_power_limit,
                                      msrs.msr_rapl_power_unit, socket);
        }
        else if (long_ver == 1)
        {
            print_verbose_package_power_limit(stdout, msrs.msr_pkg_power_limit,
                                              msrs.msr_rapl_power_unit, socket);
        }
    }

    for (socket = 0; socket < nsockets; socket++)
    {
        if (long_ver == 0)
        {
            print_package_power_info(stdout, msrs.msr_pkg_power_info, socket);
        }
        else if (long_ver == 1)
        {
            print_verbose_package_power_info(stdout, msrs.msr_pkg_power_info, socket);
        }
    }

    if (long_ver == 0)
    {
        print_rapl_power_unit(stdout, msrs.msr_rapl_power_unit);
    }
    else if (long_ver == 1)
    {
        print_verbose_rapl_power_unit(stdout, msrs.msr_rapl_power_unit);
    }

    return 0;
}

int intel_cpu_fm_06_6a_get_features(void)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    fprintf(stdout, "msr_platform_info            = 0x%lx\n",
            msrs.msr_platform_info);
    fprintf(stdout, "ia32_time_stamp_counter      = 0x%lx\n",
            msrs.ia32_time_stamp_counter);
    fprintf(stdout, "msr_rapl_power_unit          = 0x%lx\n",
            msrs.msr_rapl_power_unit);
    fprintf(stdout, "msr_pkg_power_limit          = 0x%lx\n",
            msrs.msr_pkg_power_limit);
    fprintf(stdout, "msr_pkg_energy_status        = 0x%lx\n",
            msrs.msr_pkg_energy_status);
    fprintf(stdout, "msr_pkg_power_info           = 0x%lx\n",
            msrs.msr_pkg_power_info);
    fprintf(stdout, "msr_dram_power_limit         = 0x%lx\n",
            msrs.msr_dram_power_limit);
    fprintf(stdout, "msr_dram_energy_status       = 0x%lx\n",
            msrs.msr_dram_energy_status);
    fprintf(stdout, "msr_dram_power_info          = 0x%lx\n",
            msrs.msr_dram_power_info);
    return 0;
}

int intel_cpu_fm_06_6a_get_power(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    if (long_ver == 0)
    {
        print_power_data(stdout, msrs.msr_rapl_power_unit, msrs.msr_pkg_energy_status,
                         msrs.msr_dram_energy_status);
    }
    else if (long_ver == 1)
    {
        print_verbose_power_data(stdout, msrs.msr_rapl_power_unit,
                                 msrs.msr_pkg_energy_status, msrs.msr_dram_energy_status);
    }
    return 0;
}

int intel_cpu_fm_06_6a_get_node_power_json(char **get_power_obj_str)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    json_t *get_power_obj = json_object();

    json_get_power_data(get_power_obj, msrs.msr_pkg_power_limit,
                        msrs.msr_rapl_power_unit, msrs.msr_pkg_energy_status,
                        msrs.msr_dram_energy_status);

    *get_power_obj_str = json_dumps(get_power_obj, JSON_INDENT(4));
    json_decref(get_power_obj);

    return 0;
}

int intel_cpu_fm_06_6a_get_node_power_domain_info_json(char
        **get_domain_obj_str)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    json_t *get_domain_obj = json_object();

    json_get_power_domain_info(get_domain_obj, msrs.msr_pkg_power_info,
                               msrs.msr_dram_power_info, msrs.msr_rapl_power_unit,
                               msrs.msr_pkg_power_limit);

    *get_domain_obj_str = json_dumps(get_domain_obj, JSON_INDENT(4));
    json_decref(get_domain_obj);

    return 0;
}
