// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char **argv)
{
    int ret;
    char *s = NULL;
    int num_sockets = 0;
#ifdef SECOND_RUN
    int i;
    int size = 1E4;
    volatile double x = 0.0;
#endif

    const char *usage = "Usage: %s [-h] [-v]\n";
    int opt;
    while ((opt = getopt(argc, argv, "hv")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf(usage, argv[0]);
                return 0;
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }

    /* Determine number of sockets */
    num_sockets = variorum_get_num_sockets();

    if (num_sockets <= 0)
    {
        printf("HWLOC returned an invalid number of sockets. Exiting.\n");
        exit(-1);
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
        exit(-1);
    }

    /* Print the entire JSON object */
    puts(s);

#ifdef SECOND_RUN
    for (i = 0; i < size; i++)
    {
        x += do_work(i);
    }
    ret = variorum_get_node_power_json(&s);
    if (ret != 0)
    {
        printf("Second run: JSON get node power failed!\n");
        free(s);
        exit(-1);
    }
    /* Print the entire JSON object */
    puts(s);
#endif

    /* Deallocate the string */
    free(s);

    return ret;
}
