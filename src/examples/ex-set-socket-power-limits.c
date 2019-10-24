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
    int pkg_pow_lim;

    if (argc == 1)
    {
        printf("No power limit specified...using default limit of 100W.\n");
        pkg_pow_lim = 100;
    }
    else if (argc == 2)
    {
        pkg_pow_lim = atoi(argv[1]);
        printf("Setting each socket to %dW.\n", pkg_pow_lim);
    }

    ret = set_each_socket_power_limit(pkg_pow_lim);
    if (ret != 0)
    {
        printf("Set each socket power limit failed!\n");
    }
    printf("\n");
    ret = print_power_limits();
    if (ret != 0)
    {
        printf("Print power limits failed!\n");
    }
    return ret;
}
