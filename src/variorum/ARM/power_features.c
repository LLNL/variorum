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

#include <power_features.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <variorum_timers.h>

unsigned m_num_package;
char m_hostname[1024];

int read_file_ui64(const int file, uint64_t *val)
{
    char buf[32];
    int bytes_read = read(file, buf, 32);
    if (bytes_read == 0)
    {
        return 0;
    }
    sscanf(buf, "%"SCNu64, val);
}

int write_file_ui64(const int file, uint64_t val)
{
    char buf[32];
    sprintf(buf, "%"PRIu64, val);
    int bytes_written = write(file, buf, 32);
    if (bytes_written <= 0)
    {
        return 0;
    }
}

int read_array_ui64(const int fd, uint64_t **array)
{
    int num_freq = 0;
    int iter = 0;
    char buf[4096];
    int rd = read(fd, buf, 4096);

    int nfreq = 0;
    char *fptr = buf;

    for (nfreq = 0; fptr[nfreq] != '\0'; fptr[nfreq] == ' ' ? nfreq++ : * (fptr++));

    uint64_t *val_array = (uint64_t *) malloc(sizeof(uint64_t) * nfreq);
    char *tok = strtok(buf, " ");
    while (tok && iter < nfreq)
    {
        val_array[iter++] = atoll(tok);
        tok = strtok(NULL, " \n");
    }
    *array = val_array;
    return nfreq;
}

void init_arm(void)
{
    /* Collect number of packages and GPUs per package */
    variorum_get_topology(&m_num_package, NULL, NULL);

    /* Save hostname */
    gethostname(m_hostname, 1024);
}

void shutdown_arm(void)
{
    printf("Shutdown ARM\n");
}

int get_power_data(int verbose, FILE *output)
{
    static int init_output = 0;

    uint64_t sys_power_val;
    uint64_t big_power_val;
    uint64_t little_power_val;
    uint64_t gpu_power_val;

    /* The filesystem interfaces used here and in the rest of the ARM port
     * are based on the ARM Juno R2 board technical reference documentation:
     * ARM Versatile Express Juno r2 Development Platform (V2M-Juno r2)
     * Technical Reference Manual.
     * https://developer.arm.com/documentation/100114/0200/.
     * Other ARM implementations should have the same common interfaces
     * available through ARM distributions of Linux. This port does not cover
     * ARM hardware implementation-specific interfaces.
     */

    char *sys_power_fname = "/sys/class/hwmon/hwmon0/power1_input";
    char *big_power_fname = "/sys/class/hwmon/hwmon0/power2_input";
    char *little_power_fname = "/sys/class/hwmon/hwmon0/power3_input";
    char *gpu_power_fname = "/sys/class/hwmon/hwmon0/power4_input";

    int sys_power_fd = open(sys_power_fname, O_RDONLY);
    int big_power_fd = open(big_power_fname, O_RDONLY);
    int little_power_fd = open(little_power_fname, O_RDONLY);
    int gpu_power_fd = open(gpu_power_fname, O_RDONLY);

    if (!sys_power_fd || !big_power_fd || !little_power_fd || !gpu_power_fd)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    /* Power values are reported in micro Watts */

    int sys_bytes = read_file_ui64(sys_power_fd, &sys_power_val);
    int big_bytes = read_file_ui64(big_power_fd, &big_power_val);
    int lil_bytes = read_file_ui64(little_power_fd, &little_power_val);
    int gpu_bytes = read_file_ui64(gpu_power_fd, &gpu_power_val);

    if (!sys_bytes || !big_bytes || !lil_bytes || !gpu_bytes)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    close(sys_power_fd);
    close(big_power_fd);
    close(little_power_fd);
    close(gpu_power_fd);

    /* The power telemetry obtained from the power registers is in
     * microwatts. To improve readability of verbose output, Variorum
     * converts power into milliwatts before reporting.
     */

    if (verbose)
    {
        fprintf(output,
                "_ARM_POWER Host: %s, Sys: %0.2lf mW, Big: %0.2lf mW,"
                " Little: %0.2lf mW, GPU: %0.2lf mW\n",
                m_hostname,
                (double)(sys_power_val) / 1000.0f,
                (double)(big_power_val) / 1000.0f,
                (double)(little_power_val) / 1000.0f,
                (double)(gpu_power_val) / 1000.0f);
    }
    else
    {
        if (!init_output)
        {
            fprintf(output, "_ARM_POWER Host Sys_mW Big_mW Little_mW GPU_mW\n");
            init_output = 1;
        }
        fprintf(output, "_ARM_POWER %s %0.2lf %0.2lf %0.2lf %0.2lf\n",
                m_hostname,
                (double)(sys_power_val) / 1000.0f,
                (double)(big_power_val) / 1000.0f,
                (double)(little_power_val) / 1000.0f,
                (double)(gpu_power_val) / 1000.0f);
    }
    return 0;
}

int get_thermal_data(int verbose, FILE *output)
{
    static int init_output = 0;
    uint64_t sys_therm_val;
    uint64_t big_therm_val;
    uint64_t little_therm_val;
    uint64_t gpu_therm_val;

    char *sys_therm_fname = "/sys/class/hwmon/hwmon0/temp1_input";
    char *big_therm_fname = "/sys/class/hwmon/hwmon0/temp2_input";
    char *little_therm_fname = "/sys/class/hwmon/hwmon0/temp3_input";
    char *gpu_therm_fname = "/sys/class/hwmon/hwmon0/temp4_input";

    int sys_therm_fd = open(sys_therm_fname, O_RDONLY);
    int big_therm_fd = open(big_therm_fname, O_RDONLY);
    int little_therm_fd = open(little_therm_fname, O_RDONLY);
    int gpu_therm_fd = open(gpu_therm_fname, O_RDONLY);

    if (!sys_therm_fd || !big_therm_fd || !little_therm_fd || !gpu_therm_fd)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    int sys_bytes = read_file_ui64(sys_therm_fd, &sys_therm_val);
    int big_bytes = read_file_ui64(big_therm_fd, &big_therm_val);
    int lil_bytes = read_file_ui64(little_therm_fd, &little_therm_val);
    int gpu_bytes = read_file_ui64(gpu_therm_fd, &gpu_therm_val);

    if (!sys_bytes || !big_bytes || !lil_bytes || !gpu_bytes)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    close(sys_therm_fd);
    close(big_therm_fd);
    close(little_therm_fd);
    close(gpu_therm_fd);

    if (verbose)
    {
        fprintf(output,
                "_ARM_TEMPERATURE Host: %s, Sys: %0.2lf C, Big: %0.2lf C,"
                " Little: %0.2lf C, GPU: %0.2lf C\n",
                m_hostname,
                (double)(sys_therm_val) / 1000.0f,
                (double)(big_therm_val) / 1000.0f,
                (double)(little_therm_val) / 1000.0f,
                (double)(gpu_therm_val) / 1000.0f);
    }
    else
    {
        if (!init_output)
        {
            fprintf(output, "_ARM_TEMPERATURE Host Sys_C Big_C Little_C GPU_C\n");
            init_output = 1;
        }
        fprintf(output, "_ARM_TEMPERATURE %s %0.2lf %0.2lf %0.2lf %0.2lf\n",
                m_hostname,
                (double)(sys_therm_val) / 1000.0f,
                (double)(big_therm_val) / 1000.0f,
                (double)(little_therm_val) / 1000.0f,
                (double)(gpu_therm_val) / 1000.0f);
    }
    return 0;
}

int get_clocks_data(int chipid, int verbose, FILE *output)
{
    static int init_output = 0;
    uint64_t freq_val;
    char freq_fname[4096];
    char *freq_path = "/sys/devices/system/cpu/cpufreq/policy";
    sprintf(freq_fname, "%s%d/scaling_cur_freq", freq_path, chipid);
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
    if (verbose)
    {
        fprintf(output,
                "_ARM_CLOCKS Host: %s, CPU: %s, Socket: %d, Clock: %"PRIu64" MHz\n",
                m_hostname, (chipid == 0) ? "Big" : "Little", chipid, freq_val / 1000);
    }
    else
    {
        if (!init_output)
        {
            fprintf(output, "_ARM_CLOCKS Host CPU Socket Clock_MHz\n");
            init_output = 1;
        }
        fprintf(output, "_ARM_CLOCKS %s %s %d %"PRIu64"\n",
                m_hostname, (chipid == 0) ? "Big" : "Little", chipid, freq_val / 1000);
    }
    return 0;
}

int get_frequencies(int chipid, FILE *output)
{
    static int init_output = 0;
    char freq_fname[4096];
    char *freq_path = "/sys/devices/system/cpu/cpufreq/policy";
    int freq_fd;
    uint64_t *freq_array;
    int arr_size;

    sprintf(freq_fname, "%s%d/scaling_available_frequencies", freq_path, chipid);
    freq_fd = open(freq_fname, O_RDONLY);

    if (!freq_fd)
    {
        variorum_error_handler("Error encountered in accessing sysfs interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    arr_size = read_array_ui64(freq_fd, &freq_array);
    if (!arr_size)
    {
        variorum_error_handler("Error encountered in accessing sysfs interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    close(freq_fd);

    fprintf(output, "=== Available frequencies for %s CPU (ID: %d) in MHz ===\n",
            (chipid == 0) ? "Big" : "Little", chipid);
    for (int i = 0; i < arr_size; i++)
    {
        fprintf(output, "%"PRIu64" ", freq_array[i] / 1000);
    }
    fprintf(output, "\n");
    free(freq_array);
    return 0;
}

int cap_socket_frequency(int socketid, int new_freq)
{
    static int init_output = 0;
    uint64_t freq_val;
    char freq_fname[4096];
    char *freq_path = "/sys/devices/system/cpu/cpufreq/policy";
    sprintf(freq_fname, "%s%d/scaling_setspeed", freq_path, socketid);
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
    return 0;
}


int json_get_power_data(json_t *get_power_obj)
{
    char hostname[1024];
    struct timeval tv;
    uint64_t ts;

    uint64_t sys_power_val;
    uint64_t big_power_val;
    uint64_t little_power_val;
    uint64_t gpu_power_val;
    int i;

    char sockID[4];

    gethostname(hostname, 1024);
    gettimeofday(&tv, NULL);
    ts = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
    json_object_set_new(get_power_obj, "host", json_string(hostname));
    json_object_set_new(get_power_obj, "timestamp", json_integer(ts));

    /* Read power data from hwmon interfaces, similar to the get_power_data()
       function, defined previously. */

    char *sys_power_fname = "/sys/class/hwmon/hwmon0/power1_input";
    char *big_power_fname = "/sys/class/hwmon/hwmon0/power2_input";
    char *little_power_fname = "/sys/class/hwmon/hwmon0/power3_input";
    char *gpu_power_fname = "/sys/class/hwmon/hwmon0/power4_input";

    int sys_power_fd = open(sys_power_fname, O_RDONLY);
    int big_power_fd = open(big_power_fname, O_RDONLY);
    int little_power_fd = open(little_power_fname, O_RDONLY);
    int gpu_power_fd = open(gpu_power_fname, O_RDONLY);

    if (!sys_power_fd || !big_power_fd || !little_power_fd || !gpu_power_fd)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    /* Power values are reported in micro Watts */

    int sys_bytes = read_file_ui64(sys_power_fd, &sys_power_val);
    int big_bytes = read_file_ui64(big_power_fd, &big_power_val);
    int lil_bytes = read_file_ui64(little_power_fd, &little_power_val);
    int gpu_bytes = read_file_ui64(gpu_power_fd, &gpu_power_val);

    if (!sys_bytes || !big_bytes || !lil_bytes || !gpu_bytes)
    {
        variorum_error_handler("Error encountered in accessing hwmon interface",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    close(sys_power_fd);
    close(big_power_fd);
    close(little_power_fd);
    close(gpu_power_fd);

    /* Initialize GPU and memory to -1 first, as there is no memory power,
       and GPU power exists only on socket 0.
       The value for m_num_package has been obtained in the init_arm() call. */

    for (i = 0; i < m_num_package; i++)
    {
        char mem_str[36] = "power_mem_watts_socket_";
        char gpu_str[36] = "power_gpu_watts_socket_";
        sprintf(sockID, "%d", i);
        strcat(mem_str, sockID);
        strcat(gpu_str, sockID);
        json_object_set_new(get_power_obj, mem_str, json_real(-1.0));
        json_object_set_new(get_power_obj, gpu_str, json_real(-1.0));
    }

    /* The power telemetry obtained from the power registers is in
       microwatts. To allow for vendor neutral compatibility with the JSON API,
       Variorum converts power into watts before reporting. Socket 0 is big,
       and Socket 1 is little. */

    json_object_set_new(get_power_obj, "power_node_watts",
                        json_real((double)(sys_power_val) / 1000000.0f));
    json_object_set_new(get_power_obj, "power_cpu_watts_socket_0",
                        json_real((double)(big_power_val) / 1000000.0f));
    json_object_set_new(get_power_obj, "power_cpu_watts_socket_1",
                        json_real((double)(little_power_val) / 1000000.0f));
    json_object_set_new(get_power_obj, "power_gpu_watts_socket_0",
                        json_real((double)(gpu_power_val) / 1000000.0f));
    return 0;
}


int json_get_power_domain_info(json_t *get_domain_obj)
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

    json_object_set_new(get_domain_obj, "host", json_string(hostname));
    json_object_set_new(get_domain_obj, "timestamp", json_integer(ts));

    json_object_set_new(get_domain_obj, "measurement",
                        json_string("[power_cpu, power_gpu]"));
    json_object_set_new(get_domain_obj, "control",
                        json_string("[]"));
    json_object_set_new(get_domain_obj, "unsupported",
                        json_string("[power_node, power_mem]"));
    json_object_set_new(get_domain_obj, "measurement_units",
                        json_string("[Watts, Watts]"));
    json_object_set_new(get_domain_obj, "control_units",
                        json_string("[]"));
    json_object_set_new(get_domain_obj, "control_range",
                        json_string("[]"));

    return 0;
}
