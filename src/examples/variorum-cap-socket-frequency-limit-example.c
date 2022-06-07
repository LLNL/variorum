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
    int cpu_id = 0;
    int cpu_freq_mhz = 0;

    const char *usage = "%s [--help | -h] -i socket_id -f cpu_freq_mhz\n";

    if (argc == 1 || (argc > 1 && (
                          strncmp(argv[1], "--help", strlen("--help")) == 0 ||
                          strncmp(argv[1], "-h", strlen("-h")) == 0)))
    {
        printf(usage, argv[0]);
        return 0;
    }
    if (argc <= 2)
    {
        printf(usage, argv[0]);
        return 1;
    }

    int opt;
    while ((opt = getopt(argc, argv, "i:f:")) != -1)
    {
        switch (opt)
        {
            case 'i':
                cpu_id = atoi(optarg);
                break;
            case 'f':
                cpu_freq_mhz = atoi(optarg);
                break;
            default:
                printf(usage, argv[0]);
                return -1;
        }
    }

    printf("Capping CPU %d to %d MHz.\n", cpu_id, cpu_freq_mhz);

    ret = variorum_cap_socket_frequency_limit(cpu_id, cpu_freq_mhz);
    if (ret != 0)
    {
        printf("Cap socket frequency limit failed!\n");
    }
    printf("\n");
    ret = variorum_print_frequency();
    if (ret != 0)
    {
        printf("Print frequency failed!\n");
    }
    return ret;
}
