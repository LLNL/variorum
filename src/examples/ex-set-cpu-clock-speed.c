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
    int cpu_freq_mhz;

    if (argc == 1)
    {
        printf("%s <cpu_freq_mhz>\n", argv[0]);
        return 1;
    }

    cpu_freq_mhz = atoi(argv[1]);
    printf("Setting each CPU to %d MHz.\n", cpu_freq_mhz);

    ret = variorum_set_each_core_frequency(cpu_freq_mhz);
    if (ret != 0)
    {
        printf("Set each core frequency failed!\n");
    }
    printf("\n");
    ret = variorum_print_clock_speed();
    if (ret != 0)
    {
        printf("Print clock speed failed!\n");
    }
    return ret;
}
