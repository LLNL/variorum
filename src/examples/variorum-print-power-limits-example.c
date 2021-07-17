// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#ifdef PARALLEL
#include <mpi.h>
#endif

#include <variorum.h>

int main(void)
{
    int ret;

#ifdef PARALLEL
    int rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    ret = variorum_print_power_limits(rank);
    if (ret != 0)
    {
        printf("Print power limits failed!\n");
    }

#ifdef PARALLEL
    MPI_Finalize();
#endif
    return ret;
}
