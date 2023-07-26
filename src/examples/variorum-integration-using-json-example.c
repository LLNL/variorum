// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <variorum.h>
#include <variorum_topology.h>

#ifdef SECOND_RUN
static inline double do_work(int input)
{
    int i;
    double result = (double)input;

    for (i = 0; i < 100000; i++)
    {
        result += i * result;
    }

    return result;
}
#endif

void old_parse_json_obj(char *s, int num_sockets)
{
    int i, j;

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

    /* Deallocate metric array */
    for (i = 0; i < num_sockets * 3; i++)
    {
        free(json_metric_names[i]);
    }
    free(json_metric_names);

    /*Deallocate JSON object*/
    json_decref(power_obj);
}

void parse_json_obj(char *s, int num_sockets, char *hostname)
{
	int i;
	char socketID[12];
	double power_node, power_cpu, power_gpu, power_mem;

	json_t *power_obj = json_loads(s, JSON_DECODE_ANY, NULL);
	json_t *node_obj = json_object_get(power_obj, hostname);

	if(node_obj == NULL)
	{
		printf("host object not found");
	}
	
	power_node = json_real_value(json_object_get(node_obj, "power_node_watts"));

	printf("\nExtracted power values at node and socket level are:");
	printf("\n\nNode Power: %lf Watts\n\n", power_node);

	for(i = 0; i < num_sockets; ++i)
	{
		snprintf(socketID, 12, "Socket_%d", i);
		json_t *socket_obj = json_object_get(node_obj, socketID);
		if(socket_obj == NULL)
		{
			printf("Socket object not found!\n");
		}
		power_cpu = json_real_value(json_object_get(socket_obj, "power_cpu_watts"));
		power_gpu = json_real_value(json_object_get(socket_obj, "power_gpu_watts"));
		power_mem = json_real_value(json_object_get(socket_obj, "power_mem_watts"));

		printf("Socket %d, CPU Power: %lf Watts\n", i, power_cpu);
		printf("Socket %d, GPU Power: %lf Watts\n", i, power_gpu);
		printf("Socket %d, Mem Power: %lf Watts\n\n", i, power_mem);
	}

	json_decref(power_obj);
}


int main(void)
{
	char hostname[1024];
    int ret;
    int num_sockets = 0;
    char *s = NULL;
#ifdef SECOND_RUN
    int i;
    int size = 1E4;
    volatile double x = 0.0;
#endif

    /* Determine number of sockets */
    num_sockets = variorum_get_num_sockets();

	/* get the host name */
	gethostname(hostname, 1024);

    if (num_sockets <= 0)
    {
        printf("HWLOC returned an invalid number of sockets. Exiting.\n");
        exit(-1);
    }

    ret = variorum_get_node_power_json(&s);
    if (ret != 0)
    {
        printf("First run: JSON get node power failed!\n");
        free(s);
        exit(-1);
    }

    /* Print the entire JSON object and then the parsed JSON object */
    printf("\n*****JSON object received from first run is: \n");
    puts(s);
    parse_json_obj(s, num_sockets, hostname);

#ifdef SECOND_RUN
    for (i = 0; i < size; i++)
    {
        x += do_work(i);
    }
    printf("Final result: %f\n", x);

    ret = variorum_get_node_power_json(&s);
    if (ret != 0)
    {
        printf("Second run: JSON get node power failed!\n");
        free(s);
        exit(-1);
    }

    printf("\n*****JSON object received from second run is: \n");
    puts(s);
    parse_json_obj(s, num_sockets, hostname);
#endif

    /* Deallocate the string */
    free(s);

    return ret;
}
