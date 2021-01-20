// Copyright 2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    int pkg_pow_lim_watts;
    int numprocs, rank;

    const char *usage = "%s [--help | -h] -l power_lim_watts\n";

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
    while ((opt = getopt(argc, argv, "l:")) != -1)
    {
        switch (opt)
        {
            case 'l':
                pkg_pow_lim_watts = atoi(optarg);
                break;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // higher-level software must check for thread and process safety
    // we assume rank 0 is responsible for monitor and control
    if (rank == 0)
    {
        printf("Capping each socket to %dW.\n", pkg_pow_lim_watts);

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
    }

    MPI_Finalize();
    return ret;
}
