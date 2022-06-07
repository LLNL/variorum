// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <rankstr_mpi.h>
#include <unistd.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret = 0;
    int pkg_pow_lim_watts = 0;
    int numprocs = 0, rank = 0;
    char host[1024];

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
    gethostname(host, 1023);

    MPI_Comm newcomm;
    int new_rank, new_size;

    rankstr_mpi_comm_split(MPI_COMM_WORLD, host, rank, 1, 2, &newcomm);
    MPI_Comm_size(newcomm, &new_size);
    MPI_Comm_rank(newcomm, &new_rank);

    // higher-level software must check for thread and process safety
    // we assume rank 0 on each node is responsible for monitor and control
    if (new_rank == 0)
    {
        printf("Capping each socket to %dW.\n", pkg_pow_lim_watts);

        ret = variorum_cap_each_socket_power_limit(pkg_pow_lim_watts);
        if (ret != 0)
        {
            printf("Cap each socket power limit failed!\n");
            return ret;
        }
        printf("\n");
        ret = variorum_print_verbose_power_limit();
        if (ret != 0)
        {
            printf("Print power limit failed!\n");
        }
    }

    MPI_Bcast(&ret, 1, MPI_INT, new_rank, MPI_COMM_WORLD);
    MPI_Comm_free(&newcomm);
    MPI_Finalize();
    return ret;
}
