// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <omp.h>
#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    int tid;

    const char *usage = "Usage: %s [-h] [-v]\n";
    int opt;
    while ((opt = getopt(argc, argv, "hv")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf(usage, argv[0]);
                return 0;
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }

    #pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();

        // higher-level software must check for thread and process safety
        // we assume thread 0 is responsible for monitor and control
        if (tid == 0)
        {
            ret = variorum_print_power();
            if (ret != 0)
            {
                printf("Print power failed!\n");
            }
        }
    }

    return ret;
}
