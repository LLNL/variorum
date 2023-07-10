// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <nvidia_gpu_power_features.h>
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
            variorum_error_handler("Could not get NVIDIA GPU device handle",
                                   VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }
    }

#ifdef VARIORUM_WITH_NVIDIA_GPU
    /* Collect number of packages and GPUs per package */
    variorum_get_topology(&m_num_package, NULL, NULL, P_NVIDIA_GPU_IDX);
#endif
    m_gpus_per_socket = m_total_unit_devices / m_num_package;

    /* Save hostname */
    gethostname(m_hostname, 1024);
}

void shutdownNVML(void)
{
    nvmlShutdown();
}

void nvidia_gpu_get_power_data(int chipid, int verbose, FILE *output)
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
                    "_NVIDIA_GPU_POWER_USAGE Host: %s, Socket: %d, DeviceID: %d, Power: %lf W\n",
                    m_hostname, chipid, d, value);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_NVIDIA_GPU_POWER_USAGE Host Socket DeviceID Power_W\n");
                init_output = 1;
            }
            fprintf(output, "_NVIDIA_GPU_POWER_USAGE %s %d %d %lf\n",
                    m_hostname, chipid, d, value);
        }
    }
}

void nvidia_gpu_get_thermal_data(int chipid, int verbose, FILE *output)
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
                    "_NVIDIA_GPU_TEMPERATURE Host: %s, Socket: %d, DeviceID: %d, Temperature: %u C\n",
                    m_hostname, chipid, d, gpu_temp);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_NVIDIA_GPU_TEMPERATURE Host Socket DeviceID Temperature_C\n");
                init_output = 1;
            }
            fprintf(output, "_NVIDIA_GPU_TEMPERATURE %s %d %d %d\n",
                    m_hostname, chipid, d, gpu_temp);
        }
    }

    /*!@todo: Print GPU memory temperature */
}

void nvidia_gpu_get_thermal_json(int chipid, json_t *output)
{
    unsigned gpu_temp;
    int d;

    json_t *socket_obj = json_object_get(output, m_hostname);
    if (socket_obj == NULL)
    {
        socket_obj = json_object();
        json_object_set_new(output, m_hostname, socket_obj);
    }

    char socket_id[12];
    snprintf(socket_id, 12, "Socket_%d", chipid);

    json_t *gpu_obj = json_object();
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {

        nvmlDeviceGetTemperature(m_unit_devices_file_desc[d], NVML_TEMPERATURE_GPU,
                                 &gpu_temp);
        char device_id[12];
        snprintf(device_id, 12, "Device_%d", d);
        json_object_set_new(gpu_obj, device_id, json_integer(gpu_temp));
    }

    json_object_set_new(socket_obj, socket_id, gpu_obj);
}

void nvidia_gpu_get_power_limits_data(int chipid, int verbose, FILE *output)
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
                    "_NVIDIA_GPU_POWER_LIMIT Host: %s, Socket: %d, DeviceID: %d, PowerLimit: %0.3lf W\n",
                    m_hostname, chipid, d, value);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_NVIDIA_GPU_POWER_LIMIT Host Socket DeviceID PowerLimit_W\n");
                init_output = 1;
            }
            fprintf(output, "_NVIDIA_GPU_POWER_LIMIT %s %d %d %0.3lf\n",
                    m_hostname, chipid, d, value);
        }
    }
    /*!@todo: Seperate interface for default power limits? */
}

void nvidia_gpu_get_clocks_data(int chipid, int verbose, FILE *output)
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
                    "_NVIDIA_GPU_CLOCKS Host: %s, Socket: %d, DeviceID: %d, GPU_Clock: %d MHz\n",
                    m_hostname, chipid, d, gpu_clock);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output, "_NVIDIA_GPU_CLOCKS Host Socket DeviceID GPU_Clock_MHz\n");
                init_output = 1;
            }
            fprintf(output, "_NVIDIA_GPU_CLOCKS %s %d %d %d\n",
                    m_hostname, chipid, d, gpu_clock);
        }
    }
}

void nvidia_gpu_get_clocks_json(int chipid, json_t *output)
{

    unsigned int gpu_clock;
    int d;

    json_t *socket_obj = json_object_get(output, m_hostname);
    if (socket_obj == NULL)
    {
        socket_obj = json_object();
        json_object_set_new(output, m_hostname, socket_obj);
    }

    char socket_id[12];
    snprintf(socket_id, 12, "Socket_%d", chipid);
    json_t *gpu_obj = json_object();

    /* Iterate over all GPU device handles and print GPU clock */
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        nvmlDeviceGetClock(m_unit_devices_file_desc[d], NVML_CLOCK_SM,
                           NVML_CLOCK_ID_CURRENT, &gpu_clock);
        char device_id[12];
        snprintf(device_id, 12, "Device_%d", d);
        json_object_set_new(gpu_obj, device_id, json_integer(gpu_clock));
    }
    json_object_set_new(socket_obj, socket_id, gpu_obj);

}

void nvidia_gpu_get_gpu_utilization_data(int chipid, int verbose, FILE *output)
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
                    "_NVIDIA_GPU_UTILIZATION Host: %s, Socket: %d, DeviceID: %d, SM_Utilization: %d%%, Memory_Utilization: %d%%\n",
                    m_hostname, chipid, d, util.gpu, util.memory);
        }
        else
        {
            if (!init_output)
            {
                fprintf(output,
                        "_NVIDIA_GPU_UTILIZATION Host Socket DeviceID SMUtil_%% MemUtil_%%\n");
                init_output = 1;
            }
            fprintf(output, "_NVIDIA_GPU_UTILIZATION %s %d %d %d %d\n",
                    m_hostname, chipid, d, util.gpu, util.memory);
        }
    }
}

void cap_each_gpu_power_limit(int chipid, unsigned int powerlimit)
{
    unsigned int powerlimit_mwatts = powerlimit * 1000;
    int d;

    //Iterate over all GPU device handles for this socket and print power
    for (d = chipid * (int)m_gpus_per_socket;
         d < (chipid + 1) * (int)m_gpus_per_socket; ++d)
    {
        if (NVML_SUCCESS != nvmlDeviceSetPowerManagementLimit(
                m_unit_devices_file_desc[d], powerlimit_mwatts))
        {
            variorum_error_handler("Could not set the specified GPU power limit",
                                   VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }
    }
}
