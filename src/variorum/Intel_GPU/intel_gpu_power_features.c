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

#ifdef LIBJUSTIFY_FOUND
#include <cprintf.h>
#endif

static unsigned m_total_unit_devices;
static unsigned m_gpus_per_socket;
static char m_hostname[1024];
static double *m_initial_energy_for_gpu;
static int *m_init_energy;

void releaseInitialEnergyForGPU()
{
    free(m_initial_energy_for_gpu);
    free(m_init_energy);
}

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

    static int init = 0;
    if (!init)
    {
        m_initial_energy_for_gpu = (double *) malloc(sizeof(double) *
                                   m_total_unit_devices);
        m_init_energy = (int *) calloc(m_num_package, sizeof(int));
        atexit(releaseInitialEnergyForGPU);
        init = 1;
    }

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
#ifdef LIBJUSTIFY_FOUND
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
#ifdef LIBJUSTIFY_FOUND
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
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output,
                     "%s %s %d %d %lf\n", "_INTEL_GPU_POWER_USAGE", m_hostname, chipid, d, value);
#else
            fprintf(output,
                    "%s %s %d %d %lf\n", "_INTEL_GPU_POWER_USAGE", m_hostname, chipid, d, value);
#endif
        }
    }
#ifdef LIBJUSTIFY_FOUND
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
#ifdef LIBJUSTIFY_FOUND
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
#ifdef LIBJUSTIFY_FOUND
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
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output,
                     "%s %s %d %d %.1lf\n", "_INTEL_GPU_TEMPERATURE", m_hostname, chipid, d, temp_C);
#else
            fprintf(output, "_INTEL_GPU_TEMPERATURE %s %d %d %.1lf\n",
                    m_hostname, chipid, d, temp_C);
#endif
        }
    }
#ifdef LIBJUSTIFY_FOUND
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
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output,
                     "_INTEL_GPU_CLOCKS Host: %s, Socket: %d, DeviceID: %d, GPU_Clock: %d MHz\n",
                     m_hostname, chipid, d, (int)freq_MHz);
#else
            fprintf(output,
                    "_INTEL_GPU_CLOCKS Host: %s, Socket: %d, DeviceID: %d, GPU_Clock: %d MHz\n",
                    m_hostname, chipid, d, (int)freq_MHz);
#endif
        }
        else
        {
            if (!init_output)
            {
#ifdef LIBJUSTIFY_FOUND
                cfprintf(output, "%s %s %s %s %s\n", "_INTEL_GPU_CLOCKS", "Host", "Socket",
                         "DeviceID", "GPU_Clock_MHz");
#else

                fprintf(output, "%s %s %s %s %s\n", "_INTEL_GPU_CLOCKS", "Host", "Socket",
                        "DeviceID", "GPU_Clock_MHz");
#endif
                init_output = 1;
            }
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %d %d %d\n",
                     "_INTEL_GPU_CLOCKS", m_hostname, chipid, d, (int)freq_MHz);
#else
            fprintf(output, "%s %s %d %d %d\n",
                    "_INTEL_GPU_CLOCKS", m_hostname, chipid, d, (int)freq_MHz);
#endif
        }
    }
#ifdef LIBJUSTIFY_FOUND
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
#ifdef LIBJUSTIFY_FOUND
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
#ifdef LIBJUSTIFY_FOUND
                cfprintf(output, "%s %s %s %s %s",
                         "_INTEL_GPU_POWER_LIMIT", "Host", "Socket", "DeviceID", "GPU_Power_limit_mW");
#else
                fprintf(output, "%s %s %s %s %s",
                        "_INTEL_GPU_POWER_LIMIT", "Host", "Socket", "DeviceID", "GPU_Power_limit_mW");
#endif
                init_output = 1;
            }
#ifdef LIBJUSTIFY_FOUND
            cprintf(output, "%s %s %d %d %d\n",
                    "_INTEL_GPU_POWER_LIMIT", m_hostname, chipid, d, current_powerlimit_mwatts);
#else
            fprintf(output, "%s %s %d %d %d\n",
                    "_INTEL_GPU_POWER_LIMIT", m_hostname, chipid, d, current_powerlimit_mwatts);
#endif
        }
    }
#ifdef LIBJUSTIFY_FOUND
    cflush();
#endif
}

void get_energy_data(int chipid, int verbose, FILE *output)
{
    uint64_t energy_uj;
    double value = 0.0;
    int d;
    static int init_output = 0;

    //Iterate over all GPU device handles for this socket and print power
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        int pi = 0; // only report the global power domain
        apmidg_readenergy(d, pi, &energy_uj, NULL);
        if (!m_init_energy[chipid])
        {
            m_initial_energy_for_gpu[d] = (double)energy_uj * 1.e-6;
            value = 0;
        }
        else
        {
            value = (double)energy_uj * 1.e-6;
            value -= m_initial_energy_for_gpu[d];
        }

        if (verbose)
        {
            fprintf(output, "%s: %s, %s: %d, %s: %d, %s: %lf J\n",
                    "_INTEL_GPU_ENERGY_USAGE Host", m_hostname,
                    "Socket", chipid,
                    "DeviceID", d, "Energy", value);
        }
        else
        {
            if (!init_output)
            {
#ifdef LIBJUSTIFY_FOUND
                cfprintf(output, "%s %s %s %s %s\n",
                         "_INTEL_GPU_ENERGY_USAGE", "Host",
                         "Socket", "DeviceID", "Energy");
#else
                fprintf(output, "%s %s %s %s %s\n",
                        "_INTEL_GPU_ENERGY_USAGE", "Host",
                        "Socket", "DeviceID", "Energy");
#endif
                init_output = 1;
            }
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %d %d %lf\n",
                     "_INTEL_GPU_ENERGY_USAGE", m_hostname, chipid, d, value);
#else
            fprintf(output, "%s %s %d %d %lf\n",
                    "_INTEL_GPU_ENERGY_USAGE", m_hostname, chipid, d, value);

#endif
        }
    }
    m_init_energy[chipid] = 1;
}

void get_energy_json(int chipid, json_t *get_energy_obj)
{
    uint64_t energy_uj;
    double value = 0.0;
    double total_energy_gpu = 0.0;
    int d;
    static size_t devIDlen = 24; // Long enough to avoid format truncation.
    char devID[devIDlen];
    char socket_id[12];
    snprintf(socket_id, 12, "socket_%d", chipid);

    json_object_set_new(get_energy_obj, "num_gpus_per_socket",
                        json_integer(m_gpus_per_socket));

    //try to find socket object in node object, set new object if not found
    json_t *socket_obj = json_object_get(get_energy_obj, socket_id);
    if (socket_obj == NULL)
    {
        socket_obj = json_object();
        json_object_set_new(get_energy_obj, socket_id, socket_obj);
    }

    //create new json object for GPU
    json_t *gpu_obj = json_object();
    json_object_set_new(socket_obj, "energy_gpu_joules", gpu_obj);

    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        int pi = 0; // only report the global power domain
        apmidg_readenergy(d, pi, &energy_uj, NULL);
        if (!m_init_energy[chipid])
        {
            m_initial_energy_for_gpu[d] = (double)energy_uj * 1.e-6;
            value = 0;
        }
        else
        {
            value = (double)energy_uj * 1.e-6;
            value -= m_initial_energy_for_gpu[d];
        }
        snprintf(devID, devIDlen, "GPU_%d", d);
        json_object_set_new(gpu_obj, devID, json_real(value));
        total_energy_gpu += value;
    }

    m_init_energy[chipid] = 1;

    // If we have an existing CPU object with power_node_watts, update its value.
    if (json_object_get(get_energy_obj, "energy_node_joules") != NULL)
    {
        double energy_node;
        energy_node = json_real_value(json_object_get(get_energy_obj,
                                      "energy_node_joules"));
        json_object_set(get_energy_obj, "energy_node_joules",
                        json_real(energy_node + total_energy_gpu));
    }
}
