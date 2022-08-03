// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    int *node_pow_lim_watts;
    int num_power_limits = 0;
    int i;

    num_power_limits = argc - 2;
    node_pow_lim_watts = (int *) malloc(num_power_limits * sizeof(int));

    const char *usage = "Usage: %s [-h] [-v] -l watts -m watts\n";
    int opt;
    while ((opt = getopt(argc, argv, "hvl:m:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf(usage, argv[0]);
                return 0;
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
            case 'l':
                node_pow_lim_watts[0] = atoi(optarg);
                break;
            case 'm':
                node_pow_lim_watts[1] = atoi(optarg);
                break;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }

    // TODO: need to detect how many platforms are requesting a cap from number
    // of l/m flags
    for (i = 1; i < 3; i++)
    {
        printf("Setting node %d to %dW.\n", i - 1, node_pow_lim_watts[i - 1]);
    }
    printf("Setting node to specified limits.\n");

    ret = variorum_cap_best_effort_node_power_limit(node_pow_lim_watts);
    if (ret != 0)
    {
        printf("Cap node power limit failed!\n");
        return ret;
    }
    printf("\n");
    ret = variorum_print_verbose_power_limit();
    if (ret != 0)
    {
        printf("Print power limits failed!\n");
        return ret;
    }
    return ret;
}
