// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <intel_gpu_power_features.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <variorum_timers.h>
#include <libapmidg.h>

static unsigned m_total_unit_devices;
static unsigned m_gpus_per_socket;
static char m_hostname[1024];

void initAPMIDG(void)
{
    int verbose = 0;
    unsigned m_num_package;
    apmidg_init(verbose);

    m_total_unit_devices = apmidg_getndevs();

    /* Collect number of packages and GPUs per package */
    variorum_get_topology(&m_num_package, NULL, NULL, P_INTEL_GPU_IDX);
    m_gpus_per_socket = m_total_unit_devices / m_num_package;

    /* Save hostname */
    gethostname(m_hostname, sizeof(m_hostname));
}

void shutdownAPMIDG(void)
{
    apmidg_finish();
}

void get_power_data(int chipid, int verbose, FILE *output)
{
    int d;
    static int init_output = 0;

    //Iterate over all GPU device handles for this socket and print power
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        int pi = 0; // only report the global power domain now
        double value = apmidg_readpoweravg(d, pi);

        if (verbose)
        {
            fprintf(output,
                    "_INTEL_GPU_POWER_USAGE Host: %s, Socket: %d, DeviceID: %d, Power: %lf W\n",
                    m_hostname, chipid, d, value);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_INTEL_GPU_POWER_USAGE Host Socket DeviceID Power_W\n");
                init_output = 1;
            }
            fprintf(output, "_INTEL_GPU_POWER_USAGE %s %d %d %lf\n",
                    m_hostname, chipid, d, value);
        }
    }
}

void get_thermal_data(int chipid, int verbose, FILE *output)
{
    int d;
    static int init_output = 0;

    /* Iterate over all GPU device handles populated at init and print temperature (SM) */
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        double temp_C;
        int ti = 0; // only report the first temp sensor now
        apmidg_readtemp(d, ti, &temp_C);


        if (verbose)
        {
            fprintf(output,
                    "_INTLE_GPU_TEMPERATURE Host: %s, Socket: %d, DeviceID: %d, Temperature: %.1lf C\n",
                    m_hostname, chipid, d, temp_C);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_INTEL_GPU_TEMPERATURE Host Socket DeviceID Temperature_C\n");
                init_output = 1;
            }
            fprintf(output, "_INTEL_GPU_TEMPERATURE %s %d %d %.1lf\n",
                    m_hostname, chipid, d, temp_C);
        }
    }
}

void get_clocks_data(int chipid, int verbose, FILE *output)
{
    int d;
    static int init_output = 0;

    /* Iterate over all GPU device handles and print GPU clock */
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        double freq_MHz;
        int fi = 0; // only report the first freq domain now

        apmidg_readfreq(d, fi, &freq_MHz);

        if (verbose)
        {
            fprintf(output,
                    "_INTEL_GPU_CLOCKS Host: %s, Socket: %d, DeviceID: %d, GPU_Clock: %d MHz\n",
                    m_hostname, chipid, d, (int)freq_MHz);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_INTEL_GPU_CLOCKS Host Socket DeviceID GPU_Clock_MHz\n");
                init_output = 1;
            }
            fprintf(output, "_INTEL_GPU_CLOCKS %s %d %d %d\n",
                    m_hostname, chipid, d, (int)freq_MHz);
        }
    }
}

void cap_each_gpu_power_limit(int chipid, unsigned int powerlimit)
{
    int powerlimit_mwatts = powerlimit * 1000;
    int d;

    //Iterate over all GPU device handles for this socket and print power
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        int pi = 0; // check the power domain
        int current_powerlimit_mwatts = 0;
        apmidg_setpwrlim(d, pi, powerlimit_mwatts);
        apmidg_getpwrlim(d, pi, &current_powerlimit_mwatts);


        if (powerlimit_mwatts != current_powerlimit_mwatts)
        {
            variorum_error_handler("Could not set the specified GPU power limit",
                                   VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }
    }
}

void get_power_limit_data(int chipid, int verbose, FILE *output)
{
    int d;
    static int init_output = 0;

    /* Iterate over all GPU device handles and print GPU clock */
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        int current_powerlimit_mwatts = 0;
        int pi = 0; // only report the global power domain

        apmidg_getpwrlim(d, pi, &current_powerlimit_mwatts);

        if (verbose)
        {
            fprintf(output,
                    "_INTEL_GPU_POWER_LIMIT Host: %s, Socket: %d, DeviceID: %d, GPU_Power_limit: %d mW\n",
                    m_hostname, chipid, d, current_powerlimit_mwatts);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output,
                        "_INTEL_GPU_POWER_LIMIT Host Socket DeviceID GPU_Power_limit_mW\n");
                init_output = 1;
            }
            fprintf(output, "_INTEL_GPU_POWER_LIMIT %s %d %d %d\n",
                    m_hostname, chipid, d, current_powerlimit_mwatts);
        }
    }
}
