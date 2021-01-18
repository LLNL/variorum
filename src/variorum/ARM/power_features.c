// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <inttypes.h>

#include <power_features.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <variorum_timers.h>

void read_file_ui64(const int file, uint64_t* val)
{
    char buf[32];
    read(file, buf, 32);
    sscanf(buf, "%"SCNu64, val);
}

void write_file_ui64(const int file, uint64_t val)
{
    char buf[32];
    sprintf(buf, "%lu", val);
    write(file, buf, strlen(buf));
}

int read_array_ui64(const int fd, uint64_t**array)
{
    int num_freq = 0;
    int iter = 0;
    char buf[4096];
    int rd = read(fd, buf, 4096);

    int nfreq = 0;
    char *fptr = buf;

    for(nfreq=0; fptr[nfreq] != '\0'; fptr[nfreq] == ' ' ? nfreq++ : *(fptr++));

    uint64_t *val_array = (uint64_t *) malloc(sizeof(uint64_t) * nfreq);
    char *tok = strtok(buf, " ");
    while(tok && iter < nfreq) {
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
    printf("Host:%s, num_CPUs:%d\n", m_hostname, m_num_package);
}

void shutdown_arm(void)
{
    printf("Shutdown ARM\n");
}

void dump_power_data(int verbose, FILE *output)
{
    static int init_output = 0;

    uint64_t pmic_power_val;
    uint64_t sys_power_val;
    uint64_t big_power_val;
    uint64_t little_power_val;

    char* pmic_power_fname = "/sys/class/hwmon/hwmon0/power1_input";
    char* sys_power_fname = "/sys/class/hwmon/hwmon0/power2_input";
    char* big_power_fname = "/sys/class/hwmon/hwmon0/power3_input";
    char* little_power_fname = "/sys/class/hwmon/hwmon0/power4_input";

    int pmic_power_fd = open(pmic_power_fname, O_RDONLY);
    read_file_ui64(pmic_power_fd, &pmic_power_val);
    close(pmic_power_fd );
    int sys_power_fd = open(sys_power_fname, O_RDONLY);
    read_file_ui64(sys_power_fd, &sys_power_val);
    close(sys_power_fd );
    int big_power_fd = open(big_power_fname, O_RDONLY);
    read_file_ui64(big_power_fd, &big_power_val);
    close(big_power_fd );
    int little_power_fd = open(little_power_fname, O_RDONLY);
    read_file_ui64(little_power_fd, &little_power_val);
    close(little_power_fd);

    fprintf(output, "%0.2lf mW\n", (double)(pmic_power_val)/1000.0f);
    fprintf(output, "%0.2lf mW\n", (double)(sys_power_val)/1000.0f);
    fprintf(output, "%0.2lf mW\n", (double)(big_power_val)/1000.0f);
    fprintf(output, "%0.2lf mW\n", (double)(little_power_val)/1000.0f);
}

void dump_thermal_data(int verbose, FILE *output)
{
    static int init_output = 0;
    uint64_t pmic_therm_val;
    uint64_t sys_therm_val;
    uint64_t big_therm_val;
    uint64_t little_therm_val;

    char* pmic_therm_fname = "/sys/class/hwmon/hwmon0/temp1_input";
    char* sys_therm_fname = "/sys/class/hwmon/hwmon0/temp2_input";
    char* big_therm_fname = "/sys/class/hwmon/hwmon0/temp3_input";
    char* little_therm_fname = "/sys/class/hwmon/hwmon0/temp4_input";

    int pmic_therm_fd = open(pmic_therm_fname, O_RDONLY);
    read_file_ui64(pmic_therm_fd, &pmic_therm_val);
    close(pmic_therm_fd );
    int sys_therm_fd = open(sys_therm_fname, O_RDONLY);
    read_file_ui64(sys_therm_fd, &sys_therm_val);
    close(sys_therm_fd );
    int big_therm_fd = open(big_therm_fname, O_RDONLY);
    read_file_ui64(big_therm_fd, &big_therm_val);
    close(big_therm_fd );
    int little_therm_fd = open(little_therm_fname, O_RDONLY);
    read_file_ui64(little_therm_fd, &little_therm_val);
    close(little_therm_fd);

    fprintf(output, "%0.2lf\n", (double)(pmic_therm_val)/1000.0f);
    fprintf(output, "%0.2lf\n", (double)(sys_therm_val)/1000.0f);
    fprintf(output, "%0.2lf\n", (double)(big_therm_val)/1000.0f);
    fprintf(output, "%0.2lf\n", (double)(little_therm_val)/1000.0f);
}

void dump_clocks_data(int chipid, int verbose, FILE *output)
{
    static int init_output = 0;
    uint64_t freq_val;
    char freq_fname[4096];
    char* freq_path = "/sys/devices/system/cpu/cpufreq/policy";
    sprintf(freq_fname, "%s%d/scaling_cur_freq", freq_path, chipid);
    int freq_fd = open(freq_fname,O_RDONLY);
    read_file_ui64(freq_fd, &freq_val);
    fprintf(output, "%"PRIu64"\n", freq_val/1000);
    close(freq_fd);
}

void dump_frequencies(int chipid, FILE *output)
{
    static int init_output = 0;
    char freq_fname[4096];
    char* freq_path = "/sys/devices/system/cpu/cpufreq/policy";
    sprintf(freq_fname, "%s%d/scaling_available_frequencies", freq_path, chipid);
    int freq_fd = open(freq_fname, O_RDONLY);
    uint64_t* freq_array;
    int arr_size = read_array_ui64(freq_fd, &freq_array);
    for (int i = 0; i < arr_size; i++){
        fprintf(output, "%"PRIu64"\n", freq_array[i]/1000);
    }
    close(freq_fd);
    free(freq_array);
}

void set_socket_frequency(int socketid, int new_freq)
{
    static int init_output = 0;
    uint64_t freq_val;
    char freq_fname[4096];
    char* freq_path = "/sys/devices/system/cpu/cpufreq/policy";
    sprintf(freq_fname, "%s%d/scaling_setspeed", freq_path, socketid);
    int freq_fd = open(freq_fname, O_WRONLY);
    write_file_ui64(freq_fd, new_freq);
    close(freq_fd);
}



