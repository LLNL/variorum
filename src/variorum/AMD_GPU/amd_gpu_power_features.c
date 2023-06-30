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
        uint64_t pwr_val = -1;
        double pwr_val_flt = -1.0;

        ret = rsmi_dev_power_ave_get(i, 0, &pwr_val);
        if (ret != RSMI_STATUS_SUCCESS)
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
        uint64_t pwr_val = -1, pwr_min = -1, pwr_max = -1;
        double pwr_val_flt = -1.0;

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

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
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
    static int json_init = 0;
    static struct timeval json_start;
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

    if (!json_init)
    {
        json_init = 1;
        gettimeofday(&json_start, NULL);
    }

    gettimeofday(&now, NULL);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
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

        char key[1024];
        char socket_gpu[128];
        snprintf(socket_gpu, 128, "socket%d_gpu%d_timestamp:%lf", chipid, i,
                 (now.tv_usec - json_start.tv_usec) / 1000000.0);
        snprintf(key, 1024, "%s_%s", hostname, socket_gpu);
        json_object_set_new(output, key, json_real(temp_val_flt));
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

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
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
