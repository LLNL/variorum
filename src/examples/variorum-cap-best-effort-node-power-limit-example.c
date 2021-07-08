// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    int *node_pow_lim_watts;
    int num_power_limits = 0;
    int i;

    if (argc == 1)
    {
        printf("Please specify an input value in Watts for correctness.\n");
        printf("Cannot set defaults due to architecture dependence.\n");
        return 0;
    }
    else if (argc >= 2)
    {
        num_power_limits = argc - 2;
        node_pow_lim_watts = (int *) malloc(num_power_limits * sizeof(int));
        for (i = 1; i < argc; i++)
        {
            node_pow_lim_watts[i - 1] = atoi(argv[i]);
            printf("Setting node %d to %dW.\n", i - 1, node_pow_lim_watts[i - 1]);
        }
        printf("Setting node to specified limits.\n");
    }

    ret = variorum_cap_best_effort_node_power_limit(node_pow_lim_watts);
    if (ret != 0)
    {
        printf("Cap node power limit failed!\n");
        return ret;
    }
    printf("\n");
    ret = variorum_print_verbose_power_limits();
    if (ret != 0)
    {
        printf("Print power limits failed!\n");
        return ret;
    }
    return ret;
}
