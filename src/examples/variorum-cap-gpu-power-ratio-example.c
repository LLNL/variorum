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
    int gpu_power_ratio_pct = 0;

    const char *usage = "Usage: %s [-h] [-v] -r percent\n";
    int opt;
    while ((opt = getopt(argc, argv, "hvr:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf(usage, argv[0]);
                return 0;
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
            case 'r':
                gpu_power_ratio_pct = atoi(optarg);
                break;
            default:
                printf(usage, argv[0]);
                return -1;
        }
    }

    printf("Capping GPU power ratio to %d percent.\n", gpu_power_ratio_pct);

    ret = variorum_cap_gpu_power_ratio(gpu_power_ratio_pct);
    if (ret != 0)
    {
        printf("Cap GPU power ratio failed!\n");
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
