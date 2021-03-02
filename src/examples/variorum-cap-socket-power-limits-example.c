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
    int pkg_pow_lim_watts = 100;

    if (argc == 1)
    {
        printf("No power limit specified...using default limit of 100W.\n");
    }
    else if (argc == 2)
    {
        pkg_pow_lim_watts = atoi(argv[1]);
        printf("Capping each socket to %dW.\n", pkg_pow_lim_watts);
    }
    else
    {
        printf("Usage: variorum_cap_socket_power_limit [limit in watts]\n");
        exit(0);
    }

    ret = variorum_cap_each_socket_power_limit(pkg_pow_lim_watts);
    if (ret != 0)
    {
        printf("Cap each socket power limit failed!\n");
        return ret;
    }
    printf("\n");
    ret = variorum_print_verbose_power_limits();
    if (ret != 0)
    {
        printf("Print power limits failed!\n");
    }
    return ret;
}
