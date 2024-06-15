// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <amd_gpu_power_features.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <variorum_timers.h>
#include <sys/time.h>

#ifdef LIBJUSTIFY_FOUND
#include <cprintf.h>
#endif

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
                     "_AMD_GPU_POWER_USAGE", "Host", "Socket", "DeviceID",
                     "Power", "Timestamp_sec");
#else
            fprintf(output,
                    "_AMD_GPU_POWER_USAGE Host Socket DeviceID Power Timestamp_sec\n");
#endif
        }
    }

    gettimeofday(&now, NULL);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
    {
        uint64_t pwr_val = 0;
        double pwr_val_flt = -1.0;

        /* Variorum v0.8 will support the new API from ROCm 6.0.2, which
        * adds the RSMI_POWER_TYPE enum and the rsmi_dev_power_get() API.
        * If using an older version of ROCm, please use the code segment
        * with the rsmi_dev_power_ave_get() API below on line 85 and comment
        * lines 88 and 89. We're not adding backward compatibility checks
        * at the moment due to lack of resources and time.
        *
        *  ret = rsmi_dev_power_ave_get(i, 0, &pwr_val);
        */

        RSMI_POWER_TYPE pwr_type = RSMI_AVERAGE_POWER;
        ret = rsmi_dev_power_get(i, &pwr_val, &pwr_type);

        /* On newer MI200+ GPUs, device power is only reported at the GPU-domain
         * level, and not for all devices. For example, on Tioga, we see power
         * being reported on GPU device IDs 0,2,4,6 and N/As on 1,3,5,7. This is
         * an artifact of the newer GPU power reporting mechanism.
         * As a result, we don't print an error
         * when the RSMI API here fails with status unsupported on devices
         * 1,3,5,7 (in this example), as it will report the data correctly on
         * the other devices. This also allow us to use ROCm 6.0.2+ successfully
         * on older GPUs, such as MI50. */

        if ((ret != RSMI_STATUS_SUCCESS) && (ret != RSMI_STATUS_NOT_SUPPORTED))
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        pwr_val_flt = (double)(pwr_val / (1000 * 1000)); // Convert to Watts.

        if (verbose == 1)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s: %s, %s: %d, %s: %d, %s: %0.2lf, %s: %lf sec\n",
                     "_AMD_GPU_POWER_USAGE", hostname, "Socket", chipid,
                     "DeviceID", i, "Power", pwr_val_flt, "Timestamp",
                     (now.tv_usec - start.tv_usec) / 1000000.0);
#else
            fprintf(output,
                    "_AMD_GPU_POWER_USAGE Host: %s, Socket: %d, DeviceID: %d,"
                    " Power: %0.2lf W, Timestamp: %lf sec\n",
                    hostname, chipid, i, pwr_val_flt,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
        }
        else
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "_AMD_GPU_POWER_USAGE %s %d %d %0.2lf %lf\n",
                     hostname, chipid, i, pwr_val_flt,
                     (now.tv_usec - start.tv_usec) / 1000000.0);
#else
            fprintf(output, "_AMD_GPU_POWER_USAGE %s %d %d %0.2lf %lf\n",
                    hostname, chipid, i, pwr_val_flt,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
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

void get_power_limit_data(int chipid, int total_sockets, int verbose,
                          FILE *output)
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
            cfprintf(output, "%s %s %s %s %s %s %s\n",
                     "_AMD_GPU_POWER_CAP", "Host", "Socket", "DeviceID",
                     "PowerCap_Current", "PowerCap_Min", "PowerCap_Max",
                     "Timestamp_sec");
#else
            fprintf(output,
                    "_AMD_GPU_POWER_CAP Host Socket DeviceID PowerCap_Current "
                    "PowerCap_Min PowerCap_Max Timestamp_sec\n");
#endif
        }
    }

    gettimeofday(&now, NULL);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
    {
        uint64_t pwr_val = 0, pwr_min = 0, pwr_max = 0;
        double pwr_val_flt = 0.0;

        ret = rsmi_dev_power_cap_get(i, 0, &pwr_val);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        ret = rsmi_dev_power_cap_range_get(i, 0, &pwr_max, &pwr_min);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        pwr_val_flt = (double)(pwr_val / (1000 * 1000)); // Convert to Watts.
        pwr_max = (pwr_max / (1000 * 1000)); // Convert to Watts.
        pwr_min = (pwr_min / (1000 * 1000)); // Convert to Watts.

        if (verbose == 1)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output,
                     "%s: %s, %s: %d, %s: %d, %s: %0.2lf, %s: %ld, %s: %ld, %s: %lf sec\n",
                     "_AMD_GPU_POWER_CAP", hostname,
                     "Socket", chipid,
                     "DeviceID", i,
                     "PowerCap_Current", pwr_val_flt,
                     "PowerCap_Min", pwr_min,
                     "PowerCap_Max", pwr_max,
                     "Timestamp", (now.tv_usec - start.tv_usec) / 1000000.0);
#else
            fprintf(output,
                    "_AMD_GPU_POWER_CAP Host: %s, Socket: %d, DeviceID: %d,"
                    " PowerCap_Current: %0.2lf W, PowerCap_Min: %ld W, PowerCap_Max: %ld W,"
                    "Timestamp: %lf sec\n",
                    hostname, chipid, i, pwr_val_flt, pwr_min, pwr_max,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
        }
        else
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %d %d %0.2lf %ld %ld %lf\n",
                     "_AMD_GPU_POWER_CAP", hostname, chipid, i, pwr_val_flt,
                     pwr_min, pwr_max,
                     (now.tv_usec - start.tv_usec) / 1000000.0);
#else
            fprintf(output, "_AMD_GPU_POWER_CAP %s %d %d %0.2lf %ld %ld %lf\n",
                    hostname, chipid, i, pwr_val_flt, pwr_min, pwr_max,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
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

void get_thermals_data(int chipid, int total_sockets, int verbose, FILE *output)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    int gpus_per_socket;
    char hostname[1024];
    static int init = 0;
    static struct timeval start;
    struct timeval now;
    int i;

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
                     "_AMD_GPU_TEMPERATURE", "Host", "Socket", "DeviceID",
                     "Temperature", "Timestamp_sec");
#else
            fprintf(output,
                    "_AMD_GPU_TEMPERATURE Host Socket DeviceID Temperature Timestamp_sec\n");
#endif
        }
    }

    gettimeofday(&now, NULL);

    for (i = chipid * gpus_per_socket; i < (chipid + 1) * gpus_per_socket; i++)
    {
        int64_t temp_val = -1;
        double temp_val_flt = -1.0;

        ret = rsmi_dev_temp_metric_get(i, RSMI_TEMP_TYPE_EDGE, RSMI_TEMP_CURRENT,
                                       &temp_val);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        temp_val_flt = (double)(temp_val / (1000)); // Convert to Celcius.

        if (verbose == 1)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output,
                     "%s: %s, %s: %d, %s: %d, %s: %0.2lf, %s: %lf sec\n",
                     "_AMD_GPU_TEMPERATURE Host", hostname,
                     "Socket", chipid,
                     "DeviceID", i,
                     "Temperature", temp_val_flt,
                     "Timestamp", (now.tv_usec - start.tv_usec) / 1000000.0);
#else
            fprintf(output,
                    "_AMD_GPU_TEMPERATURE Host: %s, Socket: %d, DeviceID: %d,"
                    " Temperature: %0.2lf C, Timestamp: %lf sec\n",
                    hostname, chipid, i, temp_val_flt,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
        }
        else
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %d %d %0.2lf %lf\n",
                     "_AMD_GPU_TEMPERATURE", hostname, chipid, i, temp_val_flt,
                     (now.tv_usec - start.tv_usec) / 1000000.0);
#else
            fprintf(output, "_AMD_GPU_TEMPERATURE %s %d %d %0.2lf %lf\n",
                    hostname, chipid, i, temp_val_flt,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
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
        variorum_error_handler("Could not get number of GPU devices",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
    }

    gpus_per_socket = num_devices / total_sockets;

    char socketid[12];
    snprintf(socketid, 12, "socket_%d", chipid);

    // check if socket object is in node object
    json_t *socket_obj = json_object_get(output, socketid);
    if (socket_obj == NULL)
    {
        socket_obj = json_object();
        json_object_set_new(output, socketid, socket_obj);
    }

    // general gpu object
    json_t *gpu_obj = json_object();
    json_object_set_new(socket_obj, "GPU", gpu_obj);

    int i;
    for (i = chipid * gpus_per_socket; i < (chipid + 1) * gpus_per_socket; i++)
    {
        int64_t temp_val = -1;
        double temp_val_flt = -1.0;

        ret = rsmi_dev_temp_metric_get(i, RSMI_TEMP_TYPE_EDGE, RSMI_TEMP_CURRENT,
                                       &temp_val);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        temp_val_flt = (double)(temp_val / (1000)); // Convert to Celcius.

        // gpu entry
        char gpuid[32];
        snprintf(gpuid, 32, "temp_celsius_gpu_%d", i);
        json_object_set_new(gpu_obj, gpuid, json_real(temp_val_flt));
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
                     "_AMD_GPU_CLOCKS", "Host", "Socket", "DeviceID",
                     "SystemClock_MHz", "MemoryClock_MHz", "Timestamp_sec");
#else
            fprintf(output,
                    "_AMD_GPU_CLOCKS Host Socket DeviceID SystemClock_MHz "
                    "MemoryClock_MHz Timestamp_sec\n");
#endif
        }
    }

    gettimeofday(&now, NULL);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
    {
        rsmi_frequencies_t f_sys, f_mem;
        uint32_t f_sys_val, f_mem_val;

        ret = rsmi_dev_gpu_clk_freq_get(i, RSMI_CLK_TYPE_SYS, &f_sys);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        ret = rsmi_dev_gpu_clk_freq_get(i, RSMI_CLK_TYPE_MEM, &f_mem);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        f_sys_val = f_sys.frequency[f_sys.current] / (1000 * 1000); // Convert to MHz
        f_mem_val = f_mem.frequency[f_mem.current] / (1000 * 1000); // Convert to MHz

        if (verbose == 1)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output,
                     "%s: %s, %s: %d, %s: %d, %s: %d MHz, %s: %d MHz, %s: %lf sec\n",
                     "_AMD_GPU_CLOCKS", hostname,
                     "Socket", chipid,
                     "DeviceID", i,
                     "SystemClock", f_sys_val,
                     "MemoryClock", f_mem_val,
                     "Timestamp", (now.tv_usec - start.tv_usec) / 1000000.0);
#else
            fprintf(output,
                    "_AMD_GPU_CLOCKS Host: %s, Socket: %d, DeviceID: %d,"
                    " SystemClock: %d MHz, MemoryClock: %d MHz, Timestamp: %lf sec\n",
                    hostname, chipid, i, f_sys_val, f_mem_val,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
        }
        else
        {
            fprintf(output, "_AMD_GPU_CLOCKS %s %d %d %d %d %lf\n",
                    hostname, chipid, i, f_sys_val, f_mem_val,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
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

void get_clocks_json(int chipid, int total_sockets, json_t *output)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    int gpus_per_socket;
    char socketID[16];

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
    json_object_set_new(socket_obj, "GPU", gpu_obj);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
    {
        rsmi_frequencies_t f_sys, f_mem;
        uint32_t f_sys_val, f_mem_val;

        ret = rsmi_dev_gpu_clk_freq_get(i, RSMI_CLK_TYPE_SYS, &f_sys);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        ret = rsmi_dev_gpu_clk_freq_get(i, RSMI_CLK_TYPE_MEM, &f_mem);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        f_sys_val = f_sys.frequency[f_sys.current] / (1000 * 1000); // Convert to MHz
        f_mem_val = f_mem.frequency[f_mem.current] / (1000 * 1000); // Convert to MHz

        char gpu_clock_string[32];
        snprintf(gpu_clock_string, 32, "gpu_%d_freq_mhz", i);

        char gpu_mem_clock_string[32];
        snprintf(gpu_mem_clock_string, 32, "gpu_%d_mem_freq_mhz", i);

        json_object_set_new(gpu_obj, gpu_clock_string, json_integer(f_sys_val));
        json_object_set_new(gpu_obj, gpu_mem_clock_string, json_integer(f_mem_val));
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

void get_gpu_utilization_data(int chipid, int total_sockets, int verbose,
                              FILE *output)
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
            cfprintf(output, "%s %s %s %s %s\n",
                     "_AMD_GPU_UTILIZATION", "Host", "Socket", "DeviceID",
                     "Util");
#else
            fprintf(output,
                    "_AMD_GPU_UTILIZATION Host Socket DeviceID Util\n");
#endif
        }
    }

    gettimeofday(&now, NULL);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
    {
        uint32_t utilpercent; // Percentage of time the GPU was busy

        ret = rsmi_dev_busy_percent_get(i, &utilpercent);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }

        if (verbose == 1)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output,
                     "%s: %s, %s: %d, %s: %d, %s: %d%%\n", //TODO NOT SURE IF THIS WILL WORK
                     "_AMD_GPU_UTILIZATION Host", hostname,
                     "Socket", chipid,
                     "DeviceID", i,
                     "Util", utilpercent);
#else
            fprintf(output,
                    "_AMD_GPU_UTILIZATION Host: %s, Socket: %d, DeviceID: %d,"
                    "Util: %d%%\n", hostname, chipid, i, utilpercent);
#endif
        }
        else
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %d %d %d\n",
                     "_AMD_GPU_UTILIZATION", hostname, chipid, i, utilpercent);
#else
            fprintf(output, "_AMD_GPU_UTILIZATION %s %d %d %d\n",
                    hostname, chipid, i, utilpercent);
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

void get_gpu_utilization_data_json(int chipid, int total_sockets,
                                   json_t *get_gpu_util_obj)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    int gpus_per_socket;
    char socket_id[12];
    char hostname[1024];
    char device_id[12];
    static int init = 0;
    static struct timeval start;
    struct timeval now;
    struct timeval tv;
    uint64_t ts;

    gethostname(hostname, 1024);
    gettimeofday(&tv, NULL);
    ts = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;

    json_t *get_host_util_obj = json_object_get(get_gpu_util_obj, hostname);
    if (get_host_util_obj == NULL)
    {
        get_host_util_obj = json_object();
        json_object_set_new(get_gpu_util_obj, hostname, get_host_util_obj);
    }

    json_t *timestamp_obj = json_object_get(get_host_util_obj, "timestamp");
    if (timestamp_obj == NULL)
    {
        json_object_set_new(get_host_util_obj, "timestamp", json_integer(ts));
    }

    json_t *gpu_obj = json_object_get(get_host_util_obj, "GPU");
    if (gpu_obj == NULL)
    {
        gpu_obj = json_object();
        json_object_set_new(get_host_util_obj, "GPU", gpu_obj);
    }
    snprintf(socket_id, 12, "Socket_%d", chipid);

    json_t *socket_obj = json_object_get(gpu_obj, socket_id);
    if (socket_obj == NULL)
    {
        socket_obj = json_object();
        json_object_set_new(gpu_obj, socket_id, socket_obj);
    }

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

    if (!init)
    {
        init = 1;
        gettimeofday(&start, NULL);
    }

    gettimeofday(&now, NULL);
    int i;
    for (i = chipid * gpus_per_socket; i < (chipid + 1) * gpus_per_socket; i++)
    {
        uint32_t utilpercent = 0; // Percentage of time the GPU was busy
        if (ret != RSMI_STATUS_SUCCESS)
        {
            variorum_error_handler("RSMI API was not successful",
                                   VARIORUM_ERROR_PLATFORM_ENV,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
        }
        snprintf(device_id, 12, "GPU%d_util%%", i);
        json_object_set_new(socket_obj, device_id, json_integer(utilpercent));
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

void cap_each_gpu_power_limit(int chipid, int total_sockets,
                              unsigned int powerlimit)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    int gpus_per_socket;
    char hostname[1024];
    static int init = 0;
    static struct timeval start;
    struct timeval now;
    unsigned int powerlimit_uwatts = powerlimit * 1000000;

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
    }

    gettimeofday(&now, NULL);

    int i;
    for (i = chipid * gpus_per_socket; i < (chipid + 1) * gpus_per_socket; i++)
    {
        ret = rsmi_dev_power_cap_set(i, 0, powerlimit_uwatts);
        if (ret != RSMI_STATUS_SUCCESS)
        {
            if (ret == RSMI_STATUS_PERMISSION)
            {
                variorum_error_handler(
                    "Insufficient permissions to set the GPU power limit",
                    VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"),
                    __FILE__, __FUNCTION__, __LINE__);
            }
            else
            {
                variorum_error_handler(
                    "Could not set the specified GPU power limit",
                    VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"),
                    __FILE__, __FUNCTION__, __LINE__);
            }
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

void get_json_power_data(json_t *get_power_obj, int total_sockets)
{
    int chipid;
    uint32_t num_devices;
    int gpus_per_socket;
    uint64_t pwr_val = 0;
    double pwr_val_flt = 0.0;
    double total_gpu_power = 0.0;
    int d;

    rsmi_status_t ret;

    static size_t devIDlen = 24; // Long enough to avoid format truncation.
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
        variorum_error_handler("Could not get number of GPU devices",
                               VARIORUM_ERROR_PLATFORM_ENV,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
        exit(-1);
    }

    gpus_per_socket = num_devices / total_sockets;

    json_object_set_new(get_power_obj, "num_gpus_per_socket",
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

        json_t *gpu_obj = json_object();
        json_object_set_new(socket_obj, "power_gpu_watts", gpu_obj);

        //Iterate over all GPU device handles for this socket and update object
        for (d = chipid * gpus_per_socket;
             d < (chipid + 1) * gpus_per_socket; ++d)
        {

            /* Variorum v0.8 will support the new API from ROCm 6.0.2, which
             * adds the RSMI_POWER_TYPE enum and the rsmi_dev_power_get() API.
             * If using an older version of ROCm, please use the code segment
             * with the rsmi_dev_power_ave_get() API below on line 85 and comment
             * lines 88 and 89. We're not adding backward compatibility checks
             * at the moment due to lack of resources and time.
             *
             * ret = rsmi_dev_power_ave_get(d, 0, &pwr_val);
             *
             */

            RSMI_POWER_TYPE pwr_type = RSMI_AVERAGE_POWER;
            ret = rsmi_dev_power_get(d, &pwr_val, &pwr_type);
            pwr_val_flt = (double)(pwr_val / (1000 * 1000)); // Convert to Watts
            snprintf(devID, devIDlen, "GPU_%d", d);
            json_object_set_new(gpu_obj, devID, json_real(pwr_val_flt));
            total_gpu_power += pwr_val_flt;
        }
    }

    // If we have an existing CPU object with power_node_watts, update its value.
    if (json_object_get(get_power_obj, "power_node_watts") != NULL)
    {
        double power_node;
        power_node = json_real_value(json_object_get(get_power_obj,
                                     "power_node_watts"));
        json_object_set(get_power_obj, "power_node_watts",
                        json_real(power_node + total_gpu_power));
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
