// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "neoverse_N1_power_features.h"
#include <variorum_error.h>
#include <variorum_timers.h>

#ifdef LIBJUSTIFY_FOUND
#include <cprintf.h>
#endif

int arm_cpu_neoverse_n1_get_power_data(int verbose, FILE *output)
{
    static int init_output = 0;

    uint64_t cpu_power_val;
    uint64_t io_power_val;

    /* The filesystem interfaces used for the Neoverse N1 platform are
     * based on the technical reference documentation for the platform:
     * ARM Versatile Express Juno r2 Development Platform (V2M-Juno r2)
     * Technical Reference Manual.
     * https://developer.arm.com/documentation/100616/latest.
     */

    char *cpu_power_fname = "/sys/class/hwmon/hwmon1/power1_input";
    char *io_power_fname = "/sys/class/hwmon/hwmon1/power2_input";

    int cpu_power_fd = open(cpu_power_fname, O_RDONLY);
    int io_power_fd = open(io_power_fname, O_RDONLY);

    if (!cpu_power_fd || !io_power_fd)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    /* Power values are reported in micro Watts */

    int cpu_bytes = read_file_ui64(cpu_power_fd, &cpu_power_val);
    int io_bytes = read_file_ui64(io_power_fd, &io_power_val);

    if (!cpu_bytes || !io_bytes)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    close(cpu_power_fd);
    close(io_power_fd);

    /* The power telemetry obtained from the power registers is in
     * microwatts. To improve readability of verbose output, Variorum
     * converts power into milliwatts before reporting.
     */

    if (verbose)
    {
#ifdef LIBJUSTIFY_FOUND
        cfprintf(output, "%s: %s, %s: %0.2lf mW, %s: %0.2lf mW\n",
                 "_ARM_POWER Host:", m_hostname,
                 "CPU", (double)(cpu_power_val) / 1000.0f,
                 "I/O", (double)(io_power_val) / 1000.0f);
#else
        fprintf(output,
                "_ARM_POWER Host: %s, CPU: %0.2lf mW, I/O: %0.2lf mW\n",
                m_hostname,
                (double)(cpu_power_val) / 1000.0f,
                (double)(io_power_val) / 1000.0f);
#endif
    }
    else
    {
        if (!init_output)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %s %s\n",
                     "_ARM_POWER", "Host", "CPU_mW", "I/O_mW");
#else
            fprintf(output, "_ARM_POWER Host CPU_mW I/O_mW\n");
#endif
            init_output = 1;
        }
#ifdef LIBJUSTIFY_FOUND
        cfprintf(output, "%s %s %0.2lf %0.2lf\n",
                 "_ARM_POWER", m_hostname,
                 (double)(cpu_power_val) / 1000.0f,
                 (double)(io_power_val) / 1000.0f);
#else
        fprintf(output, "_ARM_POWER %s %0.2lf %0.2lf\n",
                m_hostname,
                (double)(cpu_power_val) / 1000.0f,
                (double)(io_power_val) / 1000.0f);
#endif
    }
#ifdef LIBJUSTIFY_FOUND
    cflush();
#endif
    return 0;
}

int arm_cpu_neoverse_n1_get_thermal_data(int verbose, FILE *output)
{
    static int init_output = 0;
    uint64_t loc1_therm_val;
    uint64_t soc_therm_val;

    char *loc1_therm_fname = "/sys/class/hwmon/hwmon0/temp1_input";
    char *soc_therm_fname = "/sys/class/hwmon/hwmon1/temp1_input";

    int loc1_therm_fd = open(loc1_therm_fname, O_RDONLY);
    int soc_therm_fd = open(soc_therm_fname, O_RDONLY);

    if (!loc1_therm_fd || !soc_therm_fd)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    int loc1_bytes = read_file_ui64(loc1_therm_fd, &loc1_therm_val);
    int soc_bytes = read_file_ui64(soc_therm_fd, &soc_therm_val);

    if (!loc1_bytes || !soc_bytes)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    close(loc1_therm_fd);
    close(soc_therm_fd);

    if (verbose)
    {
#ifdef LIBJUSTIFY_FOUND
        cfprintf(output,
                 "%s: %s, %s: %0.2lf C, %s: %0.2lf C\n",
                 "_ARM_TEMPERATURE Host", m_hostname,
                 "Ethernet Controller 1", (double)(loc1_therm_val) / 1000.0f,
                 "SoC", (double)(soc_therm_val) / 1000.0f);
#else
        fprintf(output,
                "_ARM_TEMPERATURE Host: %s, Ethernet Controller 1: %0.2lf C, SoC: %0.2lf C\n",
                m_hostname,
                (double)(loc1_therm_val) / 1000.0f,
                (double)(soc_therm_val) / 1000.0f);
#endif
    }
    else
    {
        if (!init_output)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %s %s\n",
                     "_ARM_TEMPERATURE", "Host", "EthCtr1", "SoC");
#else
            fprintf(output, "_ARM_TEMPERATURE Host EthCtr1 SoC\n");
#endif
            init_output = 1;
        }
#ifdef LIBJUSTIFY_FOUND
        cfprintf(output, "%s %s %0.2lf %0.2lf\n",
                 "_ARM_TEMPERATURE", m_hostname,
                 (double)(loc1_therm_val) / 1000.0f,
                 (double)(soc_therm_val) / 1000.0f);
#else
        fprintf(output, "_ARM_TEMPERATURE %s %0.2lf %0.2lf\n",
                m_hostname,
                (double)(loc1_therm_val) / 1000.0f,
                (double)(soc_therm_val) / 1000.0f);
#endif
    }
#ifdef LIBJUSTIFY_FOUND
    cflush();
#endif
    return 0;
}

int arm_cpu_neoverse_n1_get_clocks_data(int chipid, int verbose, FILE *output)
{
    static int init_output = 0;
    uint64_t freq_val = 0;
    uint64_t core_iter;
    uint64_t aggregate_freq = 0;
    char freq_fname[4096];
    char *freq_path = "/sys/devices/system/cpu/cpufreq/policy";

    for (core_iter = 0; core_iter < NUM_CORES; core_iter++)
    {
        sprintf(freq_fname, "%s%lu/scaling_cur_freq", freq_path, core_iter);
        int freq_fd = open(freq_fname, O_RDONLY);
        if (!freq_fd)
        {
            variorum_error_handler("Error encountered in accessing sysfs interface",
                                   VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                                   __FILE__, __FUNCTION__, __LINE__);
            return -1;
        }
        int bytes_read = read_file_ui64(freq_fd, &freq_val);
        if (!bytes_read)
        {
            variorum_error_handler("Error encountered in accessing sysfs interface",
                                   VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                                   __FILE__, __FUNCTION__, __LINE__);
            return -1;
        }
        close(freq_fd);

        /* The clocks telemetry obtained from the sysfs interface is in
         * KHz. Variorum converts and reports this telemetry in MHz to
         * keep it consistent with the clocks reported for other
         * supported architectures.
         */
        aggregate_freq += freq_val / 1000;
    }
    freq_val = aggregate_freq / NUM_CORES;
    if (verbose)
    {
#ifdef LIBJUSTIFY_FOUND
        cfprintf(output, "%s: %s, %s: %d, %s: %llu MHz\n",
                 "_ARM_CLOCKS Host", m_hostname,
                 "Socket", chipid,
                 "Clock", freq_val);
#else
        fprintf(output,
                "_ARM_CLOCKS Host: %s, Socket: %d, Clock: %"PRIu64" MHz\n",
                m_hostname, chipid, freq_val);
#endif
    }
    else
    {
        if (!init_output)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %s %s\n",
                     "_ARM_CLOCKS", "Host", "Socket", "Clock_MHz");
#else
            fprintf(output, "_ARM_CLOCKS Host Socket Clock_MHz\n");
#endif
            init_output = 1;
        }
#ifdef LIBJUSTIFY_FOUND
        cfprintf(output, "%s %s %d %llu\n",
                 "_ARM_CLOCKS", m_hostname, chipid, freq_val);
#else
        fprintf(output, "_ARM_CLOCKS %s %d %"PRIu64"\n",
                m_hostname, chipid, freq_val);
#endif
    }
#ifdef LIBJUSTIFY_FOUND
    cflush();
#endif
    return 0;
}

int arm_cpu_neoverse_n1_cap_socket_frequency(int new_freq)
{
    uint64_t core_iter;
    char freq_fname[4096];
    char *freq_path = "/sys/devices/system/cpu/cpufreq/policy";

    for (core_iter = 0; core_iter < NUM_CORES; core_iter++)
    {
        sprintf(freq_fname, "%s%lu/scaling_setspeed", freq_path, core_iter);
        int freq_fd = open(freq_fname, O_WRONLY);
        if (!freq_fd)
        {
            variorum_error_handler("Error encountered in opening the sysfs interface",
                                   VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                                   __FILE__, __FUNCTION__, __LINE__);
            return -1;
        }
        else
        {
            int bytes_written = write_file_ui64(freq_fd, new_freq * 1000);
            if (!bytes_written)
            {
                variorum_error_handler("Error encountered in writing to the sysfs interface",
                                       VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                                       __FILE__, __FUNCTION__, __LINE__);
                return -1;
            }
            close(freq_fd);
        }
    }
    return 0;
}

int arm_cpu_neoverse_n1_json_get_power_data(json_t *get_power_obj)
{
    uint64_t cpu_power_val;
    uint64_t io_power_val;
    int i;

    /* Read power data from hwmon interfaces, similar to the get_power_data()
       function, defined previously. */

    char *cpu_power_fname = "/sys/class/hwmon/hwmon1/power1_input";
    char *io_power_fname = "/sys/class/hwmon/hwmon1/power2_input";

    int cpu_power_fd = open(cpu_power_fname, O_RDONLY);
    int io_power_fd = open(io_power_fname, O_RDONLY);

    if (!cpu_power_fd || !io_power_fd)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    /* Power values are reported in micro Watts */

    int cpu_bytes = read_file_ui64(cpu_power_fd, &cpu_power_val);
    int io_bytes = read_file_ui64(io_power_fd, &io_power_val);

    if (!cpu_bytes || !io_bytes)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    close(cpu_power_fd);
    close(io_power_fd);

    /* Initialize GPU and memory to -1 first, as there is no memory power,
       and GPU power exists only on socket 0.
       The value for m_num_package has been obtained in the init_arm() call. */

    for (i = 0; i < (int)m_num_package; i++)
    {
        char socketID[12];
        snprintf(socketID, 12, "Socket_%d", i);

        json_t *socket_obj = json_object();
        json_object_set_new(get_power_obj, socketID, socket_obj);

        json_object_set_new(socket_obj, "power_mem_watts", json_real(-1.0));
    }

    /* The power telemetry obtained from the power registers is in
       microwatts. To allow for vendor neutral compatibility with the JSON API,
       Variorum converts power into watts before reporting. Socket 0 is big,
       and Socket 1 is little. */

    json_t *socket_0_obj = json_object_get(get_power_obj, "socket_0");

    json_object_set_new(socket_0_obj, "power_cpu_watts",
                        json_real((double)(cpu_power_val) / 1000000.0f));
    json_object_set_new(socket_0_obj, "power_io_watts",
                        json_real((double)(io_power_val) / 1000000.0f));
    return 0;
}

int arm_cpu_neoverse_n1_json_get_power_domain_info(json_t *get_domain_obj)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    char hostname[1024];
    struct timeval tv;
    uint64_t ts;

    gethostname(hostname, 1024);
    gettimeofday(&tv, NULL);

    ts = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;

    json_t *node_obj = json_object();
    json_object_set_new(get_domain_obj, hostname, node_obj);
    json_object_set_new(node_obj, "timestamp", json_integer(ts));

    json_t *control_obj = json_object();
    json_object_set_new(node_obj, "control", control_obj);

    json_t *measurement_obj = json_object();
    json_object_set_new(node_obj, "measurement", measurement_obj);

    json_t *measurement_cpu_obj = json_object();
    json_object_set_new(measurement_obj, "power_cpu", measurement_cpu_obj);
    json_object_set_new(measurement_cpu_obj, "units", json_string("Watts"));

    json_t *unsupported_features = json_array();
    json_object_set_new(node_obj, "unsupported", unsupported_features);
    json_array_append(unsupported_features, json_string("power_node"));
    json_array_append(unsupported_features, json_string("power_mem"));

    return 0;
}
