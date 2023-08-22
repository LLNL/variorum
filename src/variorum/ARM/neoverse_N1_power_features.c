// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <inttypes.h>

#include "neoverse_N1_power_features.h"
#include <variorum_error.h>
#include <variorum_timers.h>

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
        fprintf(output,
                "_ARM_POWER Host: %s, CPU: %0.2lf mW, I/O: %0.2lf mW\n",
                m_hostname,
                (double)(cpu_power_val) / 1000.0f,
                (double)(io_power_val) / 1000.0f);
    }
    else
    {
        if (!init_output)
        {
            fprintf(output, "_ARM_POWER Host CPU_mW I/O_mW\n");
            init_output = 1;
        }
        fprintf(output, "_ARM_POWER %s %0.2lf %0.2lf\n",
                m_hostname,
                (double)(cpu_power_val) / 1000.0f,
                (double)(io_power_val) / 1000.0f);
    }
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
        fprintf(output,
                "_ARM_TEMPERATURE Host: %s, Ethernet Controller 1: %0.2lf C, SoC: %0.2lf C\n",
                m_hostname,
                (double)(loc1_therm_val) / 1000.0f,
                (double)(soc_therm_val) / 1000.0f);
    }
    else
    {
        if (!init_output)
        {
            fprintf(output, "_ARM_TEMPERATURE Host EthCtr1 SoC\n");
            init_output = 1;
        }
        fprintf(output, "_ARM_TEMPERATURE %s %0.2lf %0.2lf\n",
                m_hostname,
                (double)(loc1_therm_val) / 1000.0f,
                (double)(soc_therm_val) / 1000.0f);
    }
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
        sprintf(freq_fname, "%s%d/scaling_cur_freq", freq_path, core_iter);
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
        fprintf(output,
                "_ARM_CLOCKS Host: %s, Socket: %d, Clock: %"PRIu64" MHz\n",
                m_hostname, chipid, freq_val);
    }
    else
    {
        if (!init_output)
        {
            fprintf(output, "_ARM_CLOCKS Host Socket Clock_MHz\n");
            init_output = 1;
        }
        fprintf(output, "_ARM_CLOCKS %s %d %"PRIu64"\n",
                m_hostname, chipid, freq_val);
    }
    return 0;
}

int arm_cpu_neoverse_n1_cap_socket_frequency(int socketid, int new_freq)
{
    static int init_output = 0;
    uint64_t core_iter;
    uint64_t aggregate_freq = 0;



    char freq_fname[4096];
    char *freq_path = "/sys/devices/system/cpu/cpufreq/policy";

    for (core_iter = 0; core_iter < NUM_CORES; core_iter++)
    {
        sprintf(freq_fname, "%s%d/scaling_setspeed", freq_path, core_iter);
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
    char hostname[1024];
    struct timeval tv;
    uint64_t ts;

    uint64_t cpu_power_val;
    uint64_t io_power_val;
    int i;

    gethostname(hostname, 1024);
    gettimeofday(&tv, NULL);
    ts = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;

    json_t *node_obj = json_object_get(get_power_obj, hostname);
	if (node_obj == NULL)
	{
		node_obj = json_object();
		json_object_set_new(get_power_obj, hostname, node_obj);
	}

    json_object_set_new(node_obj, "timestamp", json_integer(ts));

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
        char socketID[16];
        snprintf(socketID, 16, "Socket_%d", i);

        json_t *socket_obj = json_object_get(node_obj, socketID);
		if (socket_obj == NULL)
		{
			socket_obj = json_object();
			json_object_set_new(node_obj, socketID, socket_obj);
		}

		json_t *cpu_obj = json_object();
		json_object_set_new(socket_obj, "CPU", cpu_obj);

        json_object_set_new(cpu_obj, "power_mem_watts", json_real(-1.0));
        if (i != 0)
        {
			json_t *gpu_obj = json_object();
			json_object_set_new(socket_obj, "GPU", gpu_obj);
            json_object_set_new(gpu_obj, "power_gpu_watts", json_real(-1.0));
        }
    }

    /* The power telemetry obtained from the power registers is in
       microwatts. To allow for vendor neutral compatibility with the JSON API,
       Variorum converts power into watts before reporting. Socket 0 is big,
       and Socket 1 is little. */

    json_t *socket_0_obj = json_object_get(node_obj, "Socket_0");
	json_t *socket_0_cpu_obj = json_object_get(socket_0_obj, "CPU");

	json_t *socket_0_gpu_obj = json_object();
	json_object_set_new(socket_0_obj, "GPU", socket_0_gpu_obj);

    json_object_set_new(socket_0_cpu_obj, "power_cpu_watts",
                        json_real((double)(cpu_power_val) / 1000000.0f));
    json_object_set_new(socket_0_gpu_obj, "power_io_watts",
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
    json_object_set_new(measurement_cpu_obj, "units", "Watts");

    json_t *unsupported_features = json_array();
    json_object_set_new(node_obj, "unsupported", unsupported_features);
    json_array_append(unsupported_features, json_string("power_node"));
    json_array_append(unsupported_features, json_string("power_mem"));

    return 0;
}
