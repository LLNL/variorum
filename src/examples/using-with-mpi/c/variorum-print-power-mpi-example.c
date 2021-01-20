// Copyright 2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <mpi.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    int numprocs, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // higher-level software must check for thread and process safety
    // we assume rank 0 is responsible for monitor and control
    if (rank == 0)
    {
        ret = variorum_print_power();
        if (ret != 0)
        {
            printf("Print power failed!\n");
        }
    }

    MPI_Finalize();
    return ret;
}
