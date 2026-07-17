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
        float counter_resolution = 0.0;
        uint64_t energy_timestamp = 0;

        // Get cumulative energy counter
        // energy_counter: accumulated energy in microjoules
        // counter_resolution: resolution in microjoules
        // energy_timestamp: timestamp in nanoseconds
        ret = rsmi_dev_energy_count_get(i, &energy_counter, &counter_resolution,
                                        &energy_timestamp);

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
        float counter_resolution = 0.0;
        uint64_t energy_timestamp = 0;

        ret = rsmi_dev_energy_count_get(i, &energy_counter, &counter_resolution,
                                        &energy_timestamp);

        // Only add to JSON if we got valid data
        if (ret == RSMI_STATUS_SUCCESS)
        {
            snprintf(deviceID, 32, "device_%d_energy_uJ", i);
            json_object_set_new(gpu_obj, deviceID, json_integer(energy_counter));

            // Also store the counter resolution
            snprintf(deviceID, 32, "device_%d_counter_resolution_uJ", i);
            json_object_set_new(gpu_obj, deviceID, json_real(counter_resolution));

            // Also store the timestamp from RSMI (in nanoseconds)
            snprintf(deviceID, 32, "device_%d_energy_timestamp_ns", i);
            json_object_set_new(gpu_obj, deviceID, json_integer(energy_timestamp));
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

// Power monitoring
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
        RSMI_POWER_TYPE pwr_type = RSMI_INVALID_POWER;

        // Use rsmi_dev_power_get() - recommended for ROCm 7+
        // Attempts current power first, falls back to average power
        ret = rsmi_dev_power_get(i, &pwr_val, &pwr_type);

        // On newer APUs, device power may only be reported at certain device IDs
        // Handle gracefully if not supported on some devices
        if ((ret != RSMI_STATUS_SUCCESS) && (ret != RSMI_STATUS_NOT_SUPPORTED))
        {
            variorum_error_handler("RSMI power API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        if (ret == RSMI_STATUS_SUCCESS)
        {
            pwr_val_flt = (double)(pwr_val / (1000 * 1000)); // Convert to Watts.

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

void get_json_power_data(json_t *get_power_obj, int total_sockets)
{
    int chipid;
    uint32_t num_devices;
    int gpus_per_socket;
    uint64_t pwr_val = 0;
    double pwr_val_flt = 0.0;
    double total_apu_power = 0.0;
    int d;

    rsmi_status_t ret;

    static size_t devIDlen = 24;
    char devID[devIDlen];
    char socketID[24];

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
        exit(-1);
    }

    gpus_per_socket = num_devices / total_sockets;

    json_object_set_new(get_power_obj, "num_apus_per_socket",
                        json_integer(gpus_per_socket));

    for (chipid = 0; chipid < total_sockets; chipid++)
    {
        snprintf(socketID, devIDlen, "socket_%d", chipid);

        json_t *socket_obj = json_object_get(get_power_obj, socketID);
        if (socket_obj == NULL)
        {
            socket_obj = json_object();
            json_object_set_new(get_power_obj, socketID, socket_obj);
        }

        json_t *apu_obj = json_object();
        json_object_set_new(socket_obj, "power_apu_watts", apu_obj);

        // Iterate over all APU device handles for this socket
        for (d = chipid * gpus_per_socket;
             d < (chipid + 1) * gpus_per_socket; ++d)
        {
            RSMI_POWER_TYPE pwr_type = RSMI_INVALID_POWER;

            // Use rsmi_dev_power_get() - recommended for ROCm 7+
            ret = rsmi_dev_power_get(d, &pwr_val, &pwr_type);

            if (ret == RSMI_STATUS_SUCCESS)
            {
                pwr_val_flt = (double)(pwr_val / (1000 * 1000)); // Convert to Watts
                snprintf(devID, devIDlen, "APU_%d", d);
                json_object_set_new(apu_obj, devID, json_real(pwr_val_flt));
                total_apu_power += pwr_val_flt;
            }
        }
    }

    // If we have an existing CPU object with power_node_watts, update its value
    if (json_object_get(get_power_obj, "power_node_watts") != NULL)
    {
        double power_node;
        power_node = json_real_value(json_object_get(get_power_obj,
                                     "power_node_watts"));
        power_node += total_apu_power;
        json_object_set_new(get_power_obj, "power_node_watts",
                            json_real(power_node));
    }
    else
    {
        json_object_set_new(get_power_obj, "power_node_watts",
                            json_real(total_apu_power));
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

void get_thermals_data(int chipid, int total_sockets, int verbose, FILE *output)
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
                     "_AMD_APU_TEMPERATURE", "Host", "Socket", "DeviceID",
                     "Temperature", "Timestamp_sec");
#else
            fprintf(output,
                    "_AMD_APU_TEMPERATURE Host Socket DeviceID Temperature Timestamp_sec\n");
#endif
        }
    }

    gettimeofday(&now, NULL);

    int i;
    for (i = chipid * gpus_per_socket; i < (chipid + 1) * gpus_per_socket; i++)
    {
        int64_t temp_val = -1;
        double temp_val_flt = -1.0;

        // Get edge temperature (die edge temperature)
        ret = rsmi_dev_temp_metric_get(i, RSMI_TEMP_TYPE_EDGE, RSMI_TEMP_CURRENT,
                                       &temp_val);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI temp API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        temp_val_flt = (double)(temp_val / 1000); // Convert millidegrees to Celsius

        if (verbose == 1)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output,
                     "%s: %s, %s: %d, %s: %d, %s: %0.2lf, %s: %lf sec\n",
                     "_AMD_APU_TEMPERATURE", hostname,
                     "Socket", chipid,
                     "DeviceID", i,
                     "Temperature", temp_val_flt,
                     "Timestamp", (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) /
                     1000000.0);
#else
            fprintf(output,
                    "_AMD_APU_TEMPERATURE Host: %s, Socket: %d, DeviceID: %d,"
                    " Temperature: %0.2lf C, Timestamp: %lf sec\n",
                    hostname, chipid, i, temp_val_flt,
                    (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
        }
        else
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "_AMD_APU_TEMPERATURE %s %d %d %0.2lf %lf\n",
                     hostname, chipid, i, temp_val_flt,
                     (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#else
            fprintf(output, "_AMD_APU_TEMPERATURE %s %d %d %0.2lf %lf\n",
                    hostname, chipid, i, temp_val_flt,
                    (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
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

void get_thermals_json(int chipid, int total_sockets, json_t *output)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    int gpus_per_socket;
    char hostname[1024];

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

    char socketid[12];
    snprintf(socketid, 12, "socket_%d", chipid);

    // Check if socket object is in node object
    json_t *socket_obj = json_object_get(output, socketid);
    if (socket_obj == NULL)
    {
        socket_obj = json_object();
        json_object_set_new(output, socketid, socket_obj);
    }

    // APU object for temperatures
    json_t *apu_obj = json_object();
    json_object_set_new(socket_obj, "APU", apu_obj);

    int i;
    for (i = chipid * gpus_per_socket; i < (chipid + 1) * gpus_per_socket; i++)
    {
        int64_t temp_val = -1;
        double temp_val_flt = -1.0;

        ret = rsmi_dev_temp_metric_get(i, RSMI_TEMP_TYPE_EDGE, RSMI_TEMP_CURRENT,
                                       &temp_val);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI temp API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        temp_val_flt = (double)(temp_val / 1000); // Convert millidegrees to Celsius

        // APU temperature entry
        char apuid[32];
        snprintf(apuid, 32, "temp_celsius_apu_%d", i);
        json_object_set_new(apu_obj, apuid, json_real(temp_val_flt));
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

void get_gpu_utilization_data_json(int chipid, int total_sockets,
                                   json_t *output)
{
    // Placeholder - implementation would mirror AMD_GPU utilization monitoring
}
