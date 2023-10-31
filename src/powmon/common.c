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
    bool power_with_util; 
};

int init_data(void)
{
    return 0;
}

void parse_power_and_util_json_obj(char *pow_str, char *util_str, int num_sockets)
{
    int i, j;
    char hostname[1024];
    char socket_num[11];
    size_t size;
    int ndevices, ndevices_socket ;
    gethostname(hostname, 1024);

    /* Create a Jansson based JSON structure */
    json_t *util_obj = NULL;

    double cpu_util, mem_util, sys_util, user_util;
    int gpu_util;
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
    power_obj = json_loads(pow_str, JSON_DECODE_ANY, NULL);

    /* Extract and print values from JSON object */
    power_node = json_real_value(json_object_get(power_obj, "power_node_watts"));

    //static char **json_metric_names = NULL;

    /* List of socket-level JSON object metrics */
    /*static const char *metrics[] = {"power_cpu_watts_socket_",
                                    "power_gpu_watts_socket_",
                                    "power_mem_watts_socket_"
                                   };*/

    /* Allocate space for metric names */
    /*json_metric_names = malloc(3 * num_sockets * sizeof(char *));
    for (i = 0; i < (3 * num_sockets); i++)
    {
        json_metric_names[i] = malloc(40);
    }

    for (i = 0; i < num_sockets; i++)
    {
        // Create a metric name list for querying json object later on /
        for (j = 0; j < 3; j++)
        {
            char current_metric[40];
            char current_socket[16];
            strcpy(current_metric, metrics[j]);
            sprintf(current_socket, "%d", i);
            strcat(current_metric, current_socket);
            strcpy(json_metric_names[(num_sockets * j) + i], current_metric);
        }
    }*/

    /* Load the string as a JSON object using Jansson */
    util_obj = json_loads(util_str, JSON_DECODE_ANY, NULL);
  

    
    /* Extract and print values from JSON object */
    json_t *host_obj = json_object_get(util_obj, hostname);
    mem_util = json_real_value(json_object_get(host_obj, "memory_util%"));
    json_t *cpu_util_obj = json_object_get(host_obj, "CPU");
    cpu_util = json_real_value(json_object_get(cpu_util_obj, "total_util%"));
    sys_util = json_real_value(json_object_get(cpu_util_obj, "system_util%"));
    user_util = json_real_value(json_object_get(cpu_util_obj, "user_util%"));
    json_t *gpu_obj = json_object_get(host_obj, "GPU");


    if (write_header == true)
    { 
        fprintf(logfile, "%s,%s,", "Timestamp (ms)", "Node Power (W)");
        for (i = 0; i < num_sockets; i++)
        {
            char str[40];
            sprintf(str, "Socket %i CPU Power (W)", i);
            fprintf(logfile, "%s,", str);
            sprintf(str, "Socket %i GPU Power (W)", i);
            fprintf(logfile, "%s,", str);
            sprintf(str, "Socket %i Mem Power (W)", i);
            fprintf(logfile, " %s,", str);

        }

        fprintf(logfile, "%s,%s,%s,%s,", "Memory Utiilizaion%", "Total CPU Utilization%", "User Utilization%", "System Utilization%");
        for (i = 0; i < num_sockets; ++i)
        {
            sprintf(socket_num,"Socket_%d",i);
            json_t *socket_obj = json_object_get(gpu_obj, socket_num);
            size = json_object_size(socket_obj);
            ndevices  = size; 
            for ( j=0; j < ndevices; j++)
            {
                char device_id[12];
                snprintf(device_id, 12, "GPU%d%d_util%", i, j);
                // Don't write out a comma after the last column name
                if ((i + 1) == num_sockets && (j +1) == ndevices)
                {
                    fprintf(logfile, "%s\n", device_id);
                    write_header = false;
                }
                else
                {
                    fprintf(logfile, "%s,", device_id);
                }
            }
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
        fprintf(logfile, "%lf, %lf, %lf,", power_cpu, power_gpu, power_mem);
    }
        

    fprintf(logfile, "%lf, %lf, %lf, %lf, ", mem_util, cpu_util, user_util, sys_util);
    for (i = 0; i < num_sockets; ++i)
    {
        sprintf(socket_num,"Socket_%d",i);
        json_t *socket_obj = json_object_get(gpu_obj, socket_num);
        size = json_object_size(socket_obj);
        ndevices  = size;
        for ( j=0; j < ndevices; j++)
        {   
            char device_id[12]; 
            snprintf(device_id, 12, "GPU%d%d_util%", i, j);
            gpu_util = json_integer_value(json_object_get(socket_obj, device_id));
            // Don't write out a comma after the last column name
            if ((i + 1) == num_sockets && (j + 1) == ndevices)
            {
                fprintf(logfile, "%d", gpu_util);
                fprintf(logfile, "\n");
            }
            else
            {
                fprintf(logfile, "%d,", gpu_util);
            }
        }
    }

 



    /* Deallocate metric array */
    for (i = 0; i < num_sockets * 3; i++)
    {
        free(json_metric_names[i]);
    }
    free(json_metric_names);

    /*Deallocate JSON object*/
    json_decref(util_obj);
    json_decref(power_obj);
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
        for (i = 0; i < num_sockets; i++)
        {
            char str[40];
            sprintf(str, "Socket %i CPU Power (W)", i);
            fprintf(logfile, "%s,", str);
            sprintf(str, "Socket %i GPU Power (W)", i);
            fprintf(logfile, "%s,", str);
            if ((i + 1) == num_sockets)
            {
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


void take_measurement(bool measure_all, bool power_with_util)
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
        int ret_util, ret_pow;
        int num_sockets = 0;
        char *power_str = NULL;
        char *util_str = NULL;

        num_sockets = variorum_get_num_sockets();

        if (num_sockets <= 0)
        {
            printf("HWLOC returned an invalid number of sockets. Exiting.\n");
            exit(-1);
        }

        //ret = variorum_get_node_power_json(&s);
        if (power_with_util == true)
        { 
            ret_util = variorum_get_node_utilization_json(&util_str); 
            if (ret_util != 0)
            {
                printf("JSON get node utilization failed. Exiting.\n");
                free(util_str);
                exit(-1);
            }


            ret_pow = variorum_get_node_power_json(&power_str);
            if (ret_pow != 0)
            {
                printf("JSON get node power failed. Exiting.\n");
                free(power_str);
                exit(-1);
            }
        
            parse_power_and_util_json_obj(power_str, util_str, num_sockets); 
        }
        else
        {
            ret_pow = variorum_get_node_power_json(&power_str);
            if (ret_pow != 0)
            {
                printf("JSON get node power failed. Exiting.\n");
                free(power_str);
                exit(-1);
            }
            // Write out to logfile
            parse_json_obj(power_str, num_sockets);
        }    
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
    th_args.power_with_util = (*(struct thread_args *)arg).power_with_util; 
    // According to the Intel docs, the counter wraps at most once per second.
    // 50 ms should be short enough to always get good information (this is
    // default).
    printf("Using sampling interval of: %ld ms\n", th_args.sample_interval);
    printf("Using verbosity of: %d\n", th_args.measure_all);
    printf("Using verbosity of: %d\n", th_args.power_with_util);
    init_msTimer(&timer, th_args.sample_interval);
    start = now_ms();

    timer_sleep(&timer);
    while (running)
    {
        take_measurement(th_args.measure_all, th_args.power_with_util);
        timer_sleep(&timer);
    }
    return arg;
}
