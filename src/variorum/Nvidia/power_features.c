// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
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

void initNVML()
{
    unsigned int d;
    /* Initialize GPU reading */
    nvmlReturn_t result = nvmlInit();
    nvmlDeviceGetCount(&m_total_unit_devices);
    m_unit_devices_file_desc = (nvmlDevice_t*) malloc(sizeof(nvmlDevice_t) * m_total_unit_devices);

    /* get handles to all devices */
    for (d = 0; d < m_total_unit_devices; ++d)
    {
        int power;

        result = nvmlDeviceGetHandleByIndex(d, &m_unit_devices_file_desc[d]);
        if (result != NVML_SUCCESS)
        {
            variorum_error_handler("Could not get GPU device handle", VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        }

        /* check to see whether we can read power */
        result = nvmlDeviceGetPowerUsage(m_unit_devices_file_desc[d], (unsigned int *)&power);
        if (result != NVML_SUCCESS)
        {
            //This needs to print an error with diagnostics and exit, but for now
            //it just prints an error and proceeds
            variorum_error_handler("Could not read power/initialize NVML", VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        }
    }
}

void shutdownNVML()
{
    nvmlShutdown();
}

void dump_power_data(FILE *writedest, int verbose)
{
    unsigned int power;
    int m_num_package = 2;
    int gpus_per_socket = m_total_unit_devices / m_num_package;
    double value = 0.0;
    int device_index = 1;
    int d;
    char hostname[1024];
    static int init_output = 0;

    gethostname(hostname, 1024);

    //Iterate over all GPU device handles populated at init and print power
    for (device_index = 0; device_index < 2; device_index++)
    {
        for (d = device_index * gpus_per_socket; d < (device_index + 1) * gpus_per_socket; ++d)
        {
            nvmlDeviceGetPowerUsage(m_unit_devices_file_desc[d], &power);
            value = 0.0f;
            value += (double)power * 0.001;

            if (verbose)
            {
                fprintf(writedest, "_GPU_POWER_USAGE Host: %s, Socket: %d, Device ID: %d, Power: %lf\n",
                        hostname, device_index, d, value);
            }
            else
            {
                if (!init_output)
                {
                    fprintf(writedest, "_GPU_POWER_USAGE | Host | Socket | Device | Power\n");
                    init_output = 1;
                }
                fprintf(writedest, "_GPU_POWER_USAGE | %s | %d | %d | %lf\n",
                        hostname, device_index, d, value);
            }
        }
    }
}

void dump_thermal_data(FILE *writedest, int verbose)
{
    unsigned int gpu_temp;
    int m_num_package = 2;
    int gpus_per_socket = m_total_unit_devices / m_num_package;
    int device_index = 1;
    int d;
    char hostname[1024];
    static int init_output = 0;

    gethostname(hostname, 1024);

    /* Iterate over all GPU device handles populated at init and print temperature (SM) */
    for (device_index = 0; device_index < 2; device_index++)
    {
        for (d = device_index * gpus_per_socket; d < (device_index + 1) * gpus_per_socket; ++d)
        {
            nvmlDeviceGetTemperature(m_unit_devices_file_desc[d], NVML_TEMPERATURE_GPU, &gpu_temp);

            if (verbose)
            {
                fprintf(writedest, "_GPU_TEMPERATURE Host: %s, Socket:%d, Device ID: %d, Temperature: %ld\n",
                        hostname, device_index, d, gpu_temp);
            }
            else
            {
                if (!init_output)
                {
                    fprintf(writedest, "_GPU_TEMPERATURE | Host | Socket | Device | Temperature\n");
                    init_output = 1;
                }
                fprintf(writedest, "_GPU_TEMPERATURE | %s | %d | %d | %ld\n",
                        hostname, device_index, d, gpu_temp);
            }
        }
    }

    /*!@todo: Print GPU memory temperature */
}

void dump_power_limits(FILE *writedest, int verbose)
{
    unsigned int power_limit;
    int m_num_package = 2;
    int gpus_per_socket = m_total_unit_devices / m_num_package;
    double value = 0.0;
    int device_index = 1;
    int d;
    char hostname[1024];
    static int init_output = 0;

    gethostname(hostname, 1024);

    /* Iterate over all GPU device handles populated at init and print GPU power limit */
    for (device_index = 0; device_index < 2; device_index++)
    {
        for (d = device_index * gpus_per_socket; d < (device_index + 1) * gpus_per_socket; ++d)
        {
            nvmlDeviceGetPowerManagementLimit(m_unit_devices_file_desc[d], &power_limit);
            value = (double) power_limit * 0.001f;

            if (verbose)
            {
                fprintf(writedest, "_GPU_POWER_LIMIT Host: %s, Socket:%d, Device ID: %d, Power limit: %0.3lf\n",
                        hostname, device_index, d, value);
            }
            else
            {
                if (!init_output)
                {
                    fprintf(writedest, "_GPU_POWER_LIMIT | Host | Socket | Device | PowerLimit\n");
                    init_output = 1;
                }
                fprintf(writedest, "_GPU_POWER_LIMIT | %s | %d | %d | %0.3lf\n",
                        hostname, device_index, d, value);
            }
        }
    }
    /*!@todo: Seperate interface for default power limits? */
}

void dump_clocks_data(FILE *writedest, int verbose)
{
    unsigned int gpu_clock;
    int m_num_package = 2;
    int gpus_per_socket = m_total_unit_devices / m_num_package;
    int device_index = 1;
    int d;
    char hostname[1024];
    static int init_output = 0;

    gethostname(hostname, 1024);

    /* Iterate over all GPU device handles populated at init and print GPU clock */
    for (device_index = 0; device_index < 2; device_index++)
    {
        for (d = device_index * gpus_per_socket; d < (device_index + 1) * gpus_per_socket; ++d)
        {
            nvmlDeviceGetClock(m_unit_devices_file_desc[d], NVML_CLOCK_SM, NVML_CLOCK_ID_CURRENT, &gpu_clock);

            if (verbose)
            {
                fprintf(writedest, "_GPU_CLOCKS Host: %s, Socket:%d, Device ID: %d, GPU Clock: %d\n",
                        hostname, device_index, d, gpu_clock);
            }
            else
            {
                if (!init_output)
                {
                    fprintf(writedest, "_GPU_CLOCKS | Host | Socket | Device | Clock\n");
                    init_output = 1;
                }
                fprintf(writedest, "_GPU_CLOCKS | %s | %d | %d | %d\n",
                        hostname, device_index, d, gpu_clock);
            }
        }
    }
}

void dump_gpu_utilization(FILE *writedest, int verbose)
{
    nvmlUtilization_t util;
    char hostname[1024];
    int m_num_package = 2;
    int gpus_per_socket = m_total_unit_devices / m_num_package;
    int device_index = 1;
    int d;
    static int init_output = 0;

    gethostname(hostname, 1024);

    /* Iterate over all GPU device handles populated at init and print GPU SM and memory utilization */
    for (device_index = 0; device_index < 2; device_index++)
    {
        for (d = device_index * gpus_per_socket; d < (device_index + 1) * gpus_per_socket; ++d)
        {
            nvmlDeviceGetUtilizationRates(m_unit_devices_file_desc[d], &util);

            if (verbose)
            {
                fprintf(writedest, "_GPU_UTILIZATION Host: %s, Socket: %d, Device ID: %d, GPU Utilization (%): SM: %d, Memory: %d\n",
                        hostname, device_index, d, util.gpu, util.memory);
            }
            else
            {
                if (!init_output)
                {
                    fprintf(writedest, "_GPU_UTILIZATION | Host | Socket | Device | SMUtil | MemUtil\n");
                    init_output = 1;
                }
                fprintf(writedest, "_GPU_UTILIZATION | %s | %d | %d | %d | %d\n",
                        hostname, device_index, d, util.gpu, util.memory);
            }
        }
    }
}
