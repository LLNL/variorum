// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    // 500W is based on minimum power on IBM Witherspoon
    int node_pow_lim_watts = 500;

    const char *usage = "Usage: %s [-hv] -l Watts\n";
    int opt;
    while ((opt = getopt(argc, argv, "hvl:")) != -1)
    {
        switch (opt)
        {
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
                break;
            case 'h':
                printf(usage, argv[0]);
                return 0;
                break;
            case 'l':
                node_pow_lim_watts = atoi(optarg);
                break;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }

    printf("Capping node to %dW.\n", node_pow_lim_watts);

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
