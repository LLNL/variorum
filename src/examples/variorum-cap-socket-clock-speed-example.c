// Copyright 2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    int cpu_id;
    int cpu_freq_mhz;

    if (argc != 3)
    {
        printf("%s <socket_id> <cpu_freq_mhz>\n", argv[0]);
        return 1;
    }

    cpu_id = atoi(argv[1]);
    cpu_freq_mhz = atoi(argv[2]);
    printf("Capping CPU %d to %d MHz.\n", cpu_id, cpu_freq_mhz);

    ret = variorum_cap_socket_frequency(cpu_id, cpu_freq_mhz);
    if (ret != 0)
    {
        printf("Cap socket clock speed failed!\n");
    }
    printf("\n");
    ret = variorum_print_clock_speed();
    if (ret != 0)
    {
        printf("Print clock speed failed!\n");
    }
    return ret;
}
