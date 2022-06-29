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
    int ret = 0;
    int cpu_freq_mhz = 0;

    const char *usage = "Usage: %s [-hvf] -f cpu_freq_mhz\n";
    int opt;
    while ((opt = getopt(argc, argv, "vhf")) != -1)
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
            case 'f':
                cpu_freq_mhz = atoi(optarg);
                break;
            default:
                printf(usage, argv[0]);
                return -1;
        }
    }

    printf("Capping each CPU to %d MHz.\n", cpu_freq_mhz);

    ret = variorum_cap_each_core_frequency_limit(cpu_freq_mhz);
    if (ret != 0)
    {
        printf("Cap each core frequency limit failed!\n");
    }
    printf("\n");
    ret = variorum_print_frequency();
    if (ret != 0)
    {
        printf("Print frequency failed!\n");
    }
    return ret;
}
