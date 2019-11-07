// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    int node_pow_lim_watts;

    if (argc == 1)
    {
        printf("No power limit specified...using default limit of 500W.\n");
        // 500W is based on minimum power on IBM Witherspoon
        node_pow_lim_watts = 500;
    }
    else if (argc == 2)
    {
        node_pow_lim_watts = atoi(argv[1]);
        printf("Setting node to %dW.\n", node_pow_lim_watts);
    }

    ret = set_node_power_limit(node_pow_lim_watts);
    if (ret != 0)
    {
        printf("Set node power limit failed!\n");
        return ret;
    }
    printf("\n");
    ret = print_verbose_power_limits();
    if (ret != 0)
    {
        printf("Print power limits failed!\n");
        return ret;
    }
    return ret;
}
