// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdbool.h>

#include <variorum.h>
#include <variorum_topology.h>
#include <variorum_timers.h>
#include <jansson.h>

struct thread_args
{
    bool measure_all;
    unsigned long sample_interval;
};

int init_data(void)
{
    return 0;
}

void parse_json_obj(char *s, int num_sockets)
{
    int i, j;
    static bool write_header = true;

    /* Create a Jansson based JSON structure */
    json_t *power_obj = NULL;

    double power_node, power_cpu, power_gpu, power_mem;
    static char **json_metric_names = NULL;

    /* List of socket-level JSON object metrics */
    static const char *metrics[] = {"power_cpu_watts_socket_",
                                    "power_gpu_watts_socket_",
                                    "power_mem_watts_socket_"
                                   };

    /* Allocate space for metric names */
    json_metric_names = malloc(3 * num_sockets * sizeof(char *));
    for (i = 0; i < (3 * num_sockets); i++)
    {
        json_metric_names[i] = malloc(40);
    }

    for (i = 0; i < num_sockets; i++)
    {
        /* Create a metric name list for querying json object later on */
        for (j = 0; j < 3; j++)
        {
            char current_metric[40];
            char current_socket[16];
            strcpy(current_metric, metrics[j]);
            sprintf(current_socket, "%d", i);
            strcat(current_metric, current_socket);
            strcpy(json_metric_names[(num_sockets * j) + i], current_metric);
        }
    }

    /* Load the string as a JSON object using Jansson */
    power_obj = json_loads(s, JSON_DECODE_ANY, NULL);

    /* Extract and print values from JSON object */
    power_node = json_real_value(json_object_get(power_obj, "power_node_watts"));

    if (write_header == true)
    {
        fprintf(logfile, "%s, %s, ", "Timestamp (ms)", "Node Power (W)");
        for (i = 0; i < num_sockets; i++)
        {
            char str[40];
            sprintf(str, "Socket %i CPU Power (W)", i);
            fprintf(logfile, "%s,", str);
            sprintf(str, "Socket %i GPU Power (W)", i);
            fprintf(logfile, "%s,", str);
            if ((i + 1) == num_sockets)
            {
                // Don't write out a comma after the last column name
                sprintf(str, "Socket %i Mem Power (W)", i);
                fprintf(logfile, "%s\n", str);
            }
            else
            {
                sprintf(str, "Socket %i Mem Power (W)", i);
                fprintf(logfile, "%s,", str);
            }

            if ((i + 1) == num_sockets)
                write_header = false;
        }

    }
    fprintf(logfile, "%ld, %lf, ", now_ms(), power_node);

    for (i = 0; i < num_sockets; i++)
    {
        power_cpu = json_real_value(json_object_get(power_obj, json_metric_names[i]));
        power_gpu = json_real_value(json_object_get(power_obj,
                                    json_metric_names[num_sockets + i]));
        power_mem = json_real_value(json_object_get(power_obj,
                                    json_metric_names[(num_sockets * 2) + i]));

        if ((i + 1) == num_sockets)
        {
            // Don't write out a comma after the last socket's entry.
            // Write a new line instead.
            fprintf(logfile, "%lf, %lf, %lf", power_cpu, power_gpu, power_mem);
            fprintf(logfile, "\n");
        }
        else
        {
            fprintf(logfile, "%lf, %lf, %lf,", power_cpu, power_gpu, power_mem);
        }
    }

    /* Deallocate metric array */
    for (i = 0; i < num_sockets * 3; i++)
    {
        free(json_metric_names[i]);
    }
    free(json_metric_names);

    /*Deallocate JSON object*/
    json_decref(power_obj);
}

void take_measurement(bool measure_all)
{
#if 0
    uint64_t instr0 = 0;
    uint64_t instr1 = 0;
    uint64_t core0 = 0;
    uint64_t core1 = 0;
    double rapl_data[10];
#endif
    pthread_mutex_lock(&mlock);

    // Default is to just dump out instantaneous power samples
    if (measure_all == false)
    {
        // Extract power information from Variorum JSON API
        int ret;
        int num_sockets = 0;
        char *s = NULL;

        num_sockets = variorum_get_num_sockets();

        if (num_sockets <= 0)
        {
            printf("HWLOC returned an invalid number of sockets. Exiting.\n");
            exit(-1);
        }

        ret = variorum_get_node_power_json(&s);
        if (ret != 0)
        {
            printf("JSON get node power failed. Exiting.\n");
            free(s);
            exit(-1);
        }

        // Write out to logfile
        parse_json_obj(s, num_sockets);
    }

    // Verbose output with all sensors/registers
    if (measure_all == true)
        variorum_monitoring(logfile);

#if 0
    total_joules += rapl_data[0] + rapl_data[1];
    limit_joules += rapl_data[2] + rapl_data[3];
    if (max_watts < rapl_data[4])
    {
        max_watts = rapl_data[4];
    }
    if (max_watts < rapl_data[5])
    {
        max_watts = rapl_data[5];
    }
    if (min_watts > rapl_data[4])
    {
        min_watts = rapl_data[4];
    }
    if (min_watts > rapl_data[5])
    {
        min_watts = rapl_data[5];
    }
    fprintf(logfile, "%ld %lf %lf %lf %lf %lf %lf %lu %lu %lu %lu\n", now_ms(),
            rapl_data[0], rapl_data[1], rapl_data[6], rapl_data[7], rapl_data[8],
            rapl_data[9], instr0, instr1, core0, core1);
#endif
    pthread_mutex_unlock(&mlock);
}

void *power_measurement(void *arg)
{
    struct mstimer timer;
    struct thread_args th_args;
    th_args.sample_interval = (*(struct thread_args *)arg).sample_interval;
    th_args.measure_all = (*(struct thread_args *)arg).measure_all;
    // According to the Intel docs, the counter wraps at most once per second.
    // 50 ms should be short enough to always get good information (this is
    // default).
    printf("Using sampling interval of: %ld ms\n", th_args.sample_interval);
    printf("Using verbosity of: %d\n", th_args.measure_all);
    init_msTimer(&timer, th_args.sample_interval);
    start = now_ms();

    timer_sleep(&timer);
    while (running)
    {
        take_measurement(th_args.measure_all);
        timer_sleep(&timer);
    }
    return arg;
}
