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
    // 500W is based on minimum power on IBM Witherspoon
    int node_pow_lim_watts = 500;

    if (argc == 1)
    {
        printf("Please specify an input value in Watts for correctness.\n");
        printf("Cannot set defaults due to architecture dependence.\n");
        return 0;
    }
    else if (argc == 2)
    {
        node_pow_lim_watts = atoi(argv[1]);
        printf("Capping node to %dW.\n", node_pow_lim_watts);
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
