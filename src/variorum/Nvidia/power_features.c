// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <power_features.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <variorum_timers.h>

void initNVML(void)
{
    unsigned int d;
    unsigned m_num_package;
    /* Initialize GPU reading */
    nvmlReturn_t result = nvmlInit();
    nvmlDeviceGetCount(&m_total_unit_devices);
    m_unit_devices_file_desc = (nvmlDevice_t *) malloc(sizeof(
                                   nvmlDevice_t) * m_total_unit_devices);

    /* Populate handles to all devices. This assumes block-mapping
     * between packages and GPUs */
    for (d = 0; d < m_total_unit_devices; ++d)
    {
        result = nvmlDeviceGetHandleByIndex(d, &m_unit_devices_file_desc[d]);
        if (result != NVML_SUCCESS)
        {
            variorum_error_handler("Could not get GPU device handle",
                                   VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }
    }

    /* Collect number of packages and GPUs per package */
    variorum_get_topology(&m_num_package, NULL, NULL);
    m_gpus_per_socket = m_total_unit_devices / m_num_package;

    /* Save hostname */
    gethostname(m_hostname, 1024);
}

void shutdownNVML(void)
{
    nvmlShutdown();
}

void dump_power_data(int chipid, int verbose, FILE *output)
{
    unsigned int power;
    double value = 0.0;
    int d;
    static int init_output = 0;

    //Iterate over all GPU device handles for this socket and print power
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        nvmlDeviceGetPowerUsage(m_unit_devices_file_desc[d], &power);
        value = (double)power * 0.001f;

        if (verbose)
        {
            fprintf(output,
                    "_GPU_POWER_USAGE Host: %s, Socket: %d, Device_ID: %d, Power: %lf W\n",
                    m_hostname, chipid, d, value);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_GPU_POWER_USAGE Host Socket Device Power\n");
                init_output = 1;
            }
            fprintf(output, "_GPU_POWER_USAGE %s %d %d %lf\n",
                    m_hostname, chipid, d, value);
        }
    }
}

void dump_thermal_data(int chipid, int verbose, FILE *output)
{
    unsigned gpu_temp;
    int d;
    static int init_output = 0;

    /* Iterate over all GPU device handles populated at init and print temperature (SM) */
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        nvmlDeviceGetTemperature(m_unit_devices_file_desc[d], NVML_TEMPERATURE_GPU,
                                 &gpu_temp);

        if (verbose)
        {
            fprintf(output,
                    "_GPU_TEMPERATURE Host: %s, Socket: %d, Device_ID: %d, Temperature: %u C\n",
                    m_hostname, chipid, d, gpu_temp);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_GPU_TEMPERATURE Host Socket Device Temperature\n");
                init_output = 1;
            }
            fprintf(output, "_GPU_TEMPERATURE %s %d %d %d\n",
                    m_hostname, chipid, d, gpu_temp);
        }
    }

    /*!@todo: Print GPU memory temperature */
}

void dump_power_limits(int chipid, int verbose, FILE *output)
{
    unsigned int power_limit;
    double value = 0.0;
    int d;
    static int init_output = 0;

    /* Iterate over all GPU device handles populated at init and print GPU power limit */
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        nvmlDeviceGetPowerManagementLimit(m_unit_devices_file_desc[d], &power_limit);
        value = (double) power_limit * 0.001f;

        if (verbose)
        {
            fprintf(output,
                    "_GPU_POWER_LIMIT Host: %s, Socket: %d, Device_ID: %d, Power_Limit: %0.3lf W\n",
                    m_hostname, chipid, d, value);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_GPU_POWER_LIMIT Host Socket Device PowerLimit\n");
                init_output = 1;
            }
            fprintf(output, "_GPU_POWER_LIMIT %s %d %d %0.3lf\n",
                    m_hostname, chipid, d, value);
        }
    }
    /*!@todo: Seperate interface for default power limits? */
}

void dump_clocks_data(int chipid, int verbose, FILE *output)
{
    unsigned int gpu_clock;
    int d;
    static int init_output = 0;

    /* Iterate over all GPU device handles and print GPU clock */
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        nvmlDeviceGetClock(m_unit_devices_file_desc[d], NVML_CLOCK_SM,
                           NVML_CLOCK_ID_CURRENT, &gpu_clock);

        if (verbose)
        {
            fprintf(output,
                    "_GPU_CLOCKS Host: %s, Socket: %d, Device_ID: %d, GPU_Clock: %d MHz\n",
                    m_hostname, chipid, d, gpu_clock);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_GPU_CLOCKS Host Socket Device Clock\n");
                init_output = 1;
            }
            fprintf(output, "_GPU_CLOCKS %s %d %d %d\n",
                    m_hostname, chipid, d, gpu_clock);
        }
    }
}

void dump_gpu_utilization(int chipid, int verbose, FILE *output)
{
    nvmlUtilization_t util;
    int d;
    static int init_output = 0;

    /* Iterate over all GPU device handles and print GPU SM and memory utilization */
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        nvmlDeviceGetUtilizationRates(m_unit_devices_file_desc[d], &util);

        if (verbose)
        {
            fprintf(output,
                    "_GPU_UTILIZATION Host: %s, Socket: %d, Device_ID: %d, SM_Utilization: %d%, Memory_Utilization: %d%\n",
                    m_hostname, chipid, d, util.gpu, util.memory);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_GPU_UTILIZATION Host Socket Device SMUtil MemUtil\n");
                init_output = 1;
            }
            fprintf(output, "_GPU_UTILIZATION %s %d %d %d %d\n",
                    m_hostname, chipid, d, util.gpu, util.memory);
        }
    }
}
