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
    int ret = 0;
    int pkg_pow_lim_watts = 0;

    const char *usage = "Usage: %s [-hv] -l Watts\n";
    int opt;
    while ((opt = getopt(argc, argv, "hvl:")) != -1)
    {
        switch (opt)
        {
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
            case 'h':
                printf(usage, argv[0]);
                return 0;
            case 'l':
                pkg_pow_lim_watts = atoi(optarg);
                break;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }

    printf("Capping each socket to %dW.\n", pkg_pow_lim_watts);

    ret = variorum_cap_each_socket_power_limit(pkg_pow_lim_watts);
    if (ret != 0)
    {
        printf("Cap each socket power limit failed!\n");
        return ret;
    }
    printf("\n");
    ret = variorum_print_verbose_power_limit();
    if (ret != 0)
    {
        printf("Print power limit failed!\n");
    }
    return ret;
}
