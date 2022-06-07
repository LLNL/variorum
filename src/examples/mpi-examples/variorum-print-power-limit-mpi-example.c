// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <mpi.h>
#include <rankstr_mpi.h>
#include <unistd.h>

#include <variorum.h>

int main()
{
    int ret = 0;
    int numprocs = 0, rank = 0;
    char host[1024];

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
        ret = variorum_print_power_limit();
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
