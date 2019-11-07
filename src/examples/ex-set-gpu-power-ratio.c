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
    int gpu_power_ratio_pct;

    if (argc == 1)
    {
        printf("No GPU power ratio specified...using default ratio of 100 percent.\n");
        gpu_power_ratio_pct = 100; /*100 % is based on IBM Witherspoon default */
    }
    else if (argc == 2)
    {
        gpu_power_ratio_pct = atoi(argv[1]);
        printf("Setting GPU power ratio to %d percent.\n", gpu_power_ratio_pct);
    }

    ret = set_gpu_power_ratio(gpu_power_ratio_pct);
    if (ret != 0)
    {
        printf("Set GPU power ratio failed!\n");
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
