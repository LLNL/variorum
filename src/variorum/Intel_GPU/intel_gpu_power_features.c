// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
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

#ifdef CPRINTF_FOUND
#include <cprintf.h>
#endif

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
#ifdef VARIORUM_WITH_INTEL_GPU
    variorum_get_topology(&m_num_package, NULL, NULL, P_INTEL_GPU_IDX);
#endif
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
#ifdef CPRINTF_FOUND
            cfprintf(output,
                     "%s: %s, %s: %d, %s: %d, %s: %lf W\n",
                     "_INTEL_GPU_POWER Host", m_hostname,
                     "Socket", chipid,
                     "DeviceID", d,
                     "Power", value);
#else
            fprintf(output,
                    "_INTEL_GPU_POWER_USAGE Host: %s, Socket: %d, DeviceID: %d, Power: %lf W\n",
                    m_hostname, chipid, d, value);
#endif
        }
        else
        {
            if (!init_output)
            {
#ifdef CPRINTF_FOUND
                cfprintf(output,
                         "%s %s %s %s %s\n", "_INTEL_GPU_POWER_USAGE", "Host", "Socket", "DeviceID",
                         "Power_W");
#else
                fprintf(output,
                        "%s %s %s %s %s\n", "_INTEL_GPU_POWER_USAGE", "Host", "Socket", "DeviceID",
                        "Power_W");
#endif
                init_output = 1;
            }
#ifdef CPRINTF_FOUND
            cfprintf(output,
                     "%s %s %d %d %lf\n", "_INTEL_GPU_POWER_USAGE", m_hostname, chipid, d, value);
#else
            fprintf(output,
                    "%s %s %d %d %lf\n", "_INTEL_GPU_POWER_USAGE", m_hostname, chipid, d, value);
#endif
        }
    }
#ifdef CPRINTF_FOUND
    cflush();
#endif
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
#ifdef CPRINTF_FOUND
            cfprintf(output,
                     "%s: %s, %s: %d, %s: %d, %s: %lf C\n",
                     "_INTEL_GPU_TEMPERATURE Host", m_hostname,
                     "Socket", chipid,
                     "DeviceID", d,
                     "Temperature", temp_C);
#else
            fprintf(output,
                    "_INTLE_GPU_TEMPERATURE Host: %s, Socket: %d, DeviceID: %d, Temperature: %.1lf C\n",
                    m_hostname, chipid, d, temp_C);
#endif
        }
        else
        {
            if (!init_output)
            {
#ifdef CPRINTF_FOUND
                cfprintf(output,
                         "%s %s %s %s %s\n", "_INTEL_GPU_TEMPERATURE", "Host", "Socket", "DeviceID",
                         "Temperature_C");
#else
                fprintf(output,
                        "%s %s %s %s %s\n", "_INTEL_GPU_TEMPERATURE", "Host", "Socket", "DeviceID",
                        "Temperature_C");

#endif
                init_output = 1;
            }
#ifdef CPRINTF_FOUND
            cfprintf(output,
                     "%s %s %d %d %.1lf\n", "_INTEL_GPU_TEMPERATURE", m_hostname, chipid, d, temp_C);
#else
            fprintf(output, "_INTEL_GPU_TEMPERATURE %s %d %d %.1lf\n",
                    m_hostname, chipid, d, temp_C);
#endif
        }
    }
#ifdef CPRINTF_FOUND
    cflush();
#endif
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
#ifdef CPRINTF_FOUND
            cfprintf(output, "%s: %s, %s: %d, %s: %d, %s: %d %s\n"
                     "_INTEL_GPU_CLOCKS Host", m_hostname, "Socket", chipid, "DeviceID", d,
                     "GPU_Clock", (int)freq_MHz, "MHz");
#else
            fprintf(output, "%s: %s, %s: %d, %s: %d, %s: %d %s\n"
                    "_INTEL_GPU_CLOCKS Host", m_hostname, "Socket", chipid, "DeviceID", d,
                    "GPU_Clock", (int)freq_MHz, "MHz");
#endif
        }
        else
        {
            if (!init_output)
            {
#ifdef CPRINTF_FOUND
                cfprintf(output, "%s %s %s %s %s\n", "_INTEL_GPU_CLOCKS", "Host", "Socket",
                         "DeviceID", "GPU_Clock_MHz");
#else

                fprintf(output, "%s %s %s %s %s\n", "_INTEL_GPU_CLOCKS", "Host", "Socket",
                        "DeviceID", "GPU_Clock_MHz");
#endif
                init_output = 1;
            }
#ifdef CPRINTF_FOUND
            cfprintf(output, "%s %s %d %d %d\n",
                     "_INTEL_GPU_CLOCKS", m_hostname, chipid, d, (int)freq_MHz);
#else
            fprintf(output, "%s %s %d %d %d\n",
                    "_INTEL_GPU_CLOCKS", m_hostname, chipid, d, (int)freq_MHz);
#endif
        }
    }
#ifdef CPRINTF_FOUND
    cflush();
#endif
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
#ifdef CPRINTF_FOUND
            cfprintf(output, "%s: %s, %s: %d, %s: %d, %s: %d %s\n",
                     "_INTEL_GPU_POWER_LIMIT Host", m_hostname, "Socket", chipid,
                     "DeviceID", d, "GPU_Power_limit", current_powerlimit_mwatts, "mW");
#else
            fprintf(output,
                    "_INTEL_GPU_POWER_LIMIT Host: %s, Socket: %d, DeviceID: %d, GPU_Power_limit: %d mW\n",
                    m_hostname, chipid, d, current_powerlimit_mwatts);
#endif

        }
        else
        {
            if (!init_output)
            {
#ifdef CPRINTF_FOUND
                cfprintf(output, "%s %s %s %s %s",
                         "_INTEL_GPU_POWER_LIMIT", "Host", "Socket", "DeviceID", "GPU_Power_limit_mW");
#else
                fprintf(output, "%s %s %s %s %s",
                        "_INTEL_GPU_POWER_LIMIT", "Host", "Socket", "DeviceID", "GPU_Power_limit_mW");
#endif
                init_output = 1;
            }
#ifdef CPRINTF_FOUND
            cprintf(output, "%s %s %d %d %d\n",
                    "_INTEL_GPU_POWER_LIMIT", m_hostname, chipid, d, current_powerlimit_mwatts);
#else
            fprintf(output, "%s %s %d %d %d\n",
                    "_INTEL_GPU_POWER_LIMIT", m_hostname, chipid, d, current_powerlimit_mwatts);
#endif
        }
    }
#ifdef CPRINTF_FOUND
    cflush();
#endif
}
