// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <amd_apu_power_features.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <variorum_timers.h>
#include <sys/time.h>

#ifdef LIBJUSTIFY_FOUND
#include <cprintf.h>
#endif

#include <rocm-core/rocm_version.h>

void get_energy_data(int chipid, int total_sockets, int verbose, FILE *output)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    int gpus_per_socket;
    char hostname[1024];
    static int init = 0;
    static struct timeval start;
    struct timeval now;

    gethostname(hostname, 1024);

    ret = rsmi_init(0);
    if (ret != RSMI_STATUS_SUCCESS)
    {
        variorum_error_handler("Could not initialize RSMI",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
        exit(-1);
    }

    ret = rsmi_num_monitor_devices(&num_devices);
    if (ret != RSMI_STATUS_SUCCESS)
    {
        variorum_error_handler("Could not get number of GPU devices",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
    }

    gpus_per_socket = num_devices / total_sockets;

    if (!init)
    {
        init = 1;
        gettimeofday(&start, NULL);
        if (verbose == 0)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %s %s %s %s\n",
                     "_AMD_APU_ENERGY", "Host", "Socket", "DeviceID",
                     "Energy_uJ", "Timestamp_sec");
#else
            fprintf(output,
                    "_AMD_APU_ENERGY Host Socket DeviceID Energy_uJ Timestamp_sec\n");
#endif
        }
    }

    gettimeofday(&now, NULL);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
    {
        uint64_t energy_counter = 0;
        float energy_timestamp = 0.0;

        // Get cumulative energy counter in microjoules
        ret = rsmi_dev_energy_count_get(i, &energy_counter, &energy_timestamp);

        // Handle case where energy is not supported on some devices
        if ((ret != RSMI_STATUS_SUCCESS) && (ret != RSMI_STATUS_NOT_SUPPORTED))
        {
            variorum_error_handler("RSMI energy API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        // Only print if we got valid data
        if (ret == RSMI_STATUS_SUCCESS)
        {
            if (verbose == 1)
            {
#ifdef LIBJUSTIFY_FOUND
                cfprintf(output, "%s: %s, %s: %d, %s: %d, %s: %lu, %s: %lf sec\n",
                         "_AMD_APU_ENERGY", hostname, "Socket", chipid,
                         "DeviceID", i, "Energy_uJ", energy_counter, "Timestamp",
                         (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#else
                fprintf(output,
                        "_AMD_APU_ENERGY Host: %s, Socket: %d, DeviceID: %d,"
                        " Energy: %lu uJ, Timestamp: %lf sec\n",
                        hostname, chipid, i, energy_counter,
                        (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
            }
            else
            {
#ifdef LIBJUSTIFY_FOUND
                cfprintf(output, "_AMD_APU_ENERGY %s %d %d %lu %lf\n",
                         hostname, chipid, i, energy_counter,
                         (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#else
                fprintf(output, "_AMD_APU_ENERGY %s %d %d %lu %lf\n",
                        hostname, chipid, i, energy_counter,
                        (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
            }
        }
    }

#ifdef LIBJUSTIFY_FOUND
    cflush();
#endif

    ret = rsmi_shut_down();
    if (ret != RSMI_STATUS_SUCCESS)
    {
        variorum_error_handler("Could not shutdown RSMI",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
    }
}

void get_energy_json(int chipid, int total_sockets, json_t *output)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    int gpus_per_socket;
    char socketID[16];
    char deviceID[32];

    snprintf(socketID, 16, "socket_%d", chipid);

    ret = rsmi_init(0);
    if (ret != RSMI_STATUS_SUCCESS)
    {
        variorum_error_handler("Could not initialize RSMI",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
    }

    ret = rsmi_num_monitor_devices(&num_devices);
    if (ret != RSMI_STATUS_SUCCESS)
    {
        variorum_error_handler("Could not get number of GPU devices",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
    }

    gpus_per_socket = num_devices / total_sockets;

    json_t *socket_obj = json_object_get(output, socketID);
    if (socket_obj == NULL)
    {
        socket_obj = json_object();
        json_object_set_new(output, socketID, socket_obj);
    }

    json_t *gpu_obj = json_object();
    json_object_set_new(socket_obj, "energy_apu_uJ", gpu_obj);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
    {
        uint64_t energy_counter = 0;
        float energy_timestamp = 0.0;

        ret = rsmi_dev_energy_count_get(i, &energy_counter, &energy_timestamp);

        // Only add to JSON if we got valid data
        if (ret == RSMI_STATUS_SUCCESS)
        {
            snprintf(deviceID, 32, "device_%d_energy_uJ", i);
            json_object_set_new(gpu_obj, deviceID, json_integer(energy_counter));

            // Also store the timestamp from RSMI
            snprintf(deviceID, 32, "device_%d_energy_timestamp", i);
            json_object_set_new(gpu_obj, deviceID, json_real(energy_timestamp));
        }
    }

    ret = rsmi_shut_down();
    if (ret != RSMI_STATUS_SUCCESS)
    {
        variorum_error_handler("Could not shutdown RSMI",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
    }
}

// Power monitoring - reuse AMD GPU implementations
void get_power_data(int chipid, int total_sockets, int verbose, FILE *output)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    int gpus_per_socket;
    char hostname[1024];
    static int init = 0;
    static struct timeval start;
    struct timeval now;

    gethostname(hostname, 1024);

    ret = rsmi_init(0);
    if (ret != RSMI_STATUS_SUCCESS)
    {
        variorum_error_handler("Could not initialize RSMI",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
        exit(-1);
    }

    ret = rsmi_num_monitor_devices(&num_devices);
    if (ret != RSMI_STATUS_SUCCESS)
    {
        variorum_error_handler("Could not get number of APU devices",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
    }

    gpus_per_socket = num_devices / total_sockets;

    if (!init)
    {
        init = 1;
        gettimeofday(&start, NULL);
        if (verbose == 0)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %s %s %s %s\n",
                     "_AMD_APU_POWER_USAGE", "Host", "Socket", "DeviceID",
                     "Power", "Timestamp_sec");
#else
            fprintf(output,
                    "_AMD_APU_POWER_USAGE Host Socket DeviceID Power Timestamp_sec\n");
#endif
        }
    }

    gettimeofday(&now, NULL);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
    {
        uint64_t pwr_val = 0;
        double pwr_val_flt = -1.0;

#ifndef ROCM_VERSION_MAJOR
#error "ROCM_VERSION_MAJOR not defined"
#elif ROCM_VERSION_MAJOR <= 5
        ret = rsmi_dev_power_ave_get(i, 0, &pwr_val);
#else
        RSMI_POWER_TYPE pwr_type = RSMI_AVERAGE_POWER;
        ret = rsmi_dev_power_get(i, &pwr_val, &pwr_type);
#endif

        if ((ret != RSMI_STATUS_SUCCESS) && (ret != RSMI_STATUS_NOT_SUPPORTED))
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        pwr_val_flt = (double)(pwr_val / (1000 * 1000)); // Convert to Watts.

        if (ret == RSMI_STATUS_SUCCESS)
        {
            if (verbose == 1)
            {
#ifdef LIBJUSTIFY_FOUND
                cfprintf(output, "%s: %s, %s: %d, %s: %d, %s: %0.2lf, %s: %lf sec\n",
                         "_AMD_APU_POWER_USAGE", hostname, "Socket", chipid,
                         "DeviceID", i, "Power", pwr_val_flt, "Timestamp",
                         (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#else
                fprintf(output,
                        "_AMD_APU_POWER_USAGE Host: %s, Socket: %d, DeviceID: %d,"
                        " Power: %0.2lf W, Timestamp: %lf sec\n",
                        hostname, chipid, i, pwr_val_flt,
                        (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
            }
            else
            {
#ifdef LIBJUSTIFY_FOUND
                cfprintf(output, "_AMD_APU_POWER_USAGE %s %d %d %0.2lf %lf\n",
                         hostname, chipid, i, pwr_val_flt,
                         (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#else
                fprintf(output, "_AMD_APU_POWER_USAGE %s %d %d %0.2lf %lf\n",
                        hostname, chipid, i, pwr_val_flt,
                        (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
            }
        }
    }

#ifdef LIBJUSTIFY_FOUND
    cflush();
#endif

    ret = rsmi_shut_down();
    if (ret != RSMI_STATUS_SUCCESS)
    {
        variorum_error_handler("Could not shutdown RSMI",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
    }
}

void get_json_power_data(json_t *output, int nsockets)
{
    // Placeholder - full implementation would mirror AMD_GPU pattern
    // For now, just iterate over sockets and call get_power_data conceptually
}

void get_thermals_data(int chipid, int total_sockets, int verbose, FILE *output)
{
    // Placeholder - implementation would mirror AMD_GPU thermal monitoring
}

void get_thermals_json(int chipid, int total_sockets, json_t *output)
{
    // Placeholder - implementation would mirror AMD_GPU thermal monitoring
}

void get_clocks_data(int chipid, int total_sockets, int verbose, FILE *output)
{
    // Placeholder - implementation would mirror AMD_GPU clock monitoring
}

void get_clocks_json(int chipid, int total_sockets, json_t *output)
{
    // Placeholder - implementation would mirror AMD_GPU clock monitoring
}

void get_power_limit_data(int chipid, int total_sockets, int verbose,
                          FILE *output)
{
    // Placeholder - implementation would mirror AMD_GPU power cap monitoring
}

void cap_each_gpu_power_limit(int chipid, int total_sockets,
                               unsigned int powerlimit)
{
    // Placeholder - implementation would mirror AMD_GPU power cap setting
}

void get_gpu_utilization_data(int chipid, int total_sockets, int verbose,
                               FILE *output)
{
    // Placeholder - implementation would mirror AMD_GPU utilization monitoring
}

void get_gpu_utilization_data_json(int chipid, int total_sockets, json_t *output)
{
    // Placeholder - implementation would mirror AMD_GPU utilization monitoring
}
