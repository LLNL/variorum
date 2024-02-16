// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
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

void parse_json_power_obj(char *s, int num_sockets)
{
    const char *hostname = NULL;
    char header_str[500]; //A big allocation at the moment.
    char value_str[1000]; //A big allocation at the moment.
    char temp_value_str[100];
    json_t *node_obj = NULL;
    json_t *power_obj = json_loads(s, JSON_DECODE_ANY, NULL);
    void *iter = json_object_iter(power_obj);

    static bool write_header = true;

    /* This is tailored to the nested structure that we have created (see docs). */
    /* Just for the first level, we use the iterator to obtain the hostname,
     * as this is encoded as a key to reduce verbosity. */

    while (iter)
    {
        hostname = json_object_iter_key(iter);
        node_obj = json_object_iter_value(iter);
        if (node_obj == NULL)
        {
            printf("JSON object not found");
            exit(0);
        }
        /* The following should return NULL after the first call per our object. */
        iter = json_object_iter_next(power_obj, iter);
    }

    uint64_t timestamp;
    int num_gpus_per_socket = -1;

    //  Extract node-levels value from node object
    timestamp = json_integer_value(json_object_get(node_obj, "timestamp"));

    if (write_header == true)
    {
        strcpy(header_str, "Hostname,Timestamp,");
    }
    sprintf(temp_value_str, "%s,%ld,", hostname, timestamp);
    strcat(value_str, temp_value_str);
    // printf("Hostname: %s\n", hostname);
    // printf("Timestamp: %lu\n", timestamp);

    // If we're on a GPU-only build, we don't have power_node_watts.
    if (json_object_get(node_obj, "power_node_watts") != NULL)
    {
        double power_node;
        power_node = json_real_value(json_object_get(node_obj, "power_node_watts"));
        if (write_header == true)
        {
            strcat(header_str, "Node Power (W),");
        }
        sprintf(temp_value_str, "%0.2lf,", power_node);
        strcat(value_str, temp_value_str);
        //printf("Node Power: %0.2lf Watts\n", power_node);
    }

    // If we're on a CPU-only build, we don't have num_gpus_per_socket.
    // Powmon doesn't need to print this, but needs to know this value.
    if (json_object_get(node_obj, "num_gpus_per_socket") != NULL)
    {
        num_gpus_per_socket = json_integer_value(json_object_get(node_obj,
                              "num_gpus_per_socket"));
        //printf("Number of GPUs per socket: %d\n", num_gpus_per_socket);
    }

    //  Extract socket and GPU-level data from the node object

    int i;
    char socketID[20];

    for (i = 0; i < num_sockets; ++i)
    {
        snprintf(socketID, 20, "socket_%d", i);
        json_t *socket_obj = json_object_get(node_obj, socketID);
        if (socket_obj == NULL)
        {
            printf("Socket object not found!\n");
            exit(0);
        }

        //If we're on a GPU-only build, we don't have power_cpu_watts
        if (json_object_get(socket_obj, "power_cpu_watts") != NULL)
        {
            double power_cpu;
            power_cpu = json_real_value(json_object_get(socket_obj, "power_cpu_watts"));
            if (write_header == true)
            {
                sprintf(temp_value_str, "Socket %d CPU Power (W),", i);
                strcat(header_str, temp_value_str);
            }
            sprintf(temp_value_str, "%0.2lf,", power_cpu);
            strcat(value_str, temp_value_str);

            //printf("Socket %d, CPU Power: %0.2lf Watts\n", i, power_cpu);
        }

        //If we're on a GPU-only build, we don't have power_mem_watts
        if (json_object_get(socket_obj, "power_mem_watts") != NULL)
        {
            double power_mem;
            power_mem = json_real_value(json_object_get(socket_obj, "power_mem_watts"));
            if (write_header == true)
            {
                sprintf(temp_value_str, "Socket %d Mem Power (W),", i);
                strcat(header_str, temp_value_str);
            }
            sprintf(temp_value_str, "%0.2lf,", power_mem);
            strcat(value_str, temp_value_str);
            //printf("Socket %d, Mem Power: %0.2lf Watts\n", i, power_mem);
        }

        if (num_gpus_per_socket > 0)
        {
            json_t *gpu_obj = json_object_get(socket_obj, "power_gpu_watts");

            if (gpu_obj == NULL)
            {
                printf("GPU object not found! \n");
                exit(0);
            }
            const char *key;
            json_t *gpu_value;
            char last_gpu_str[32];

            json_object_foreach(gpu_obj, key, gpu_value)
            {
                //Determine last GPU, and check to add newline instead of comma.
                sprintf(last_gpu_str, "GPU_%d", ((num_gpus_per_socket * num_sockets) - 1));
                if (write_header == true)
                {
                    if (strcmp(last_gpu_str, key) == 0)
                    {
                        sprintf(temp_value_str, "%s Power\n", key);
                        strcat(header_str, temp_value_str);
                    }
                    else
                    {
                        sprintf(temp_value_str, "%s Power,", key);
                        strcat(header_str, temp_value_str);
                    }

                }
                if (strcmp(last_gpu_str, key) == 0)
                {
                    sprintf(temp_value_str, "%0.2lf\n", json_real_value(gpu_value));
                    strcat(value_str, temp_value_str);
                }
                else
                {
                    sprintf(temp_value_str, "%0.2lf,", json_real_value(gpu_value));
                    strcat(value_str, temp_value_str);
                }
                //printf("Socket %d, %s Power: %0.2lf Watts\n", i, key, json_real_value(value));
            }
        }

        if ((i + 1) == num_sockets)
        {
            // Write the header and set flag to false
            fprintf(logfile, "%s,", header_str);
            write_header = false;
        }

    }

    //Write the values now
    fprintf(logfile, "%s,", value_str);

    // Clean up memory.
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

        ret = variorum_get_power_json(&s);
        if (ret != 0)
        {
            printf("JSON get node power failed. Exiting.\n");
            free(s);
            exit(-1);
        }

        // Write out to logfile
        parse_json_power_obj(s, num_sockets);
    }

    // Verbose output with all sensors/registers
    if (measure_all == true)
    {
        variorum_monitoring(logfile);
    }

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
