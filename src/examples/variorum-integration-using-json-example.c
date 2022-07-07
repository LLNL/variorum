// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <variorum.h>
#include <variorum_topology.h>
#include <jansson.h>

int main(void)
{
    int ret;
    char *s = NULL;
    int num_sockets = 0;
    int i, j;
    json_t *power_obj = NULL;
    double power_node, power_cpu, power_gpu, power_mem;
    static char **json_metric_names = NULL;
    static const char *metrics[] = {"power_cpu_watts_socket_",
                                    "power_gpu_watts_socket_", "power_mem_watts_socket_"
                                   };

    /* Determine number of sockets */
    num_sockets = variorum_get_num_sockets();

    if (num_sockets <= 0)
    {
        printf("HWLOC returned an invalid number of sockets. Exiting.\n");
        exit(-1);
    }

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

    /* Allocate string based on number of sockets on the platform */
    /* String allocation below assumes the following:
     * Upper bound of 180 characters for hostname, timestamp and node power.
     * Upper bound of 150 characters for per-socket information */
    s = (char *) malloc((num_sockets * 150 + 180) * sizeof(char));

    ret = variorum_get_node_power_json(&s);
    if (ret != 0)
    {
        printf("First run: JSON get node power failed!\n");
        free(s);
        for (int i = 0; i < num_sockets * 3; i++)
        {
            free(json_metric_names[i]);
        }
        free(json_metric_names);
        exit(-1);
    }

    /* Print the entire JSON object */
    printf("\nJSON object received from Variorum is: \n");
    puts(s);

    /* Load the string as a JSON object using Jansson */
    power_obj = json_loads(s, JSON_DECODE_ANY, NULL);

    /* Extract and print values from JSON object */
    power_node = json_real_value(json_object_get(power_obj, "power_node_watts"));

    printf("\nExtracted power values at node and socket level are:");
    printf("\n\nNode Power: %lf Watts\n\n", power_node);

    for (i = 0; i < num_sockets; i++)
    {
        power_cpu = json_real_value(json_object_get(power_obj, json_metric_names[i]));
        power_gpu = json_real_value(json_object_get(power_obj,
                                    json_metric_names[num_sockets + i]));
        power_mem = json_real_value(json_object_get(power_obj,
                                    json_metric_names[(num_sockets * 2) + i]));

        printf("Socket %d, CPU Power: %lf Watts\n", i, power_cpu);
        printf("Socket %d, GPU Power: %lf Watts\n", i, power_gpu);
        printf("Socket %d, Memory Power: %lf Watts\n\n", i, power_mem);
    }

    /* Deallocate the string */
    free(s);

    /* Deallocate metric array */
    for (i = 0; i < num_sockets * 3; i++)
    {
        free(json_metric_names[i]);
    }
    free(json_metric_names);

    /*Deallocate JSON object*/
    json_decref(power_obj);

    return ret;
}
