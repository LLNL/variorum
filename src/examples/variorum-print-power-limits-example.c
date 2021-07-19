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
    int mpi_comm;
    MPI_Init(NULL, NULL);
    mpi_comm = MPI_Comm_c2f(MPI_COMM_WORLD);
    ret = variorum_print_power_limits(mpi_comm);
#else
    ret = variorum_print_power_limits();
#endif
    if (ret != 0)
    {
        printf("Print power limits failed!\n");
    }

#ifdef PARALLEL
    MPI_Finalize();
#endif
    return ret;
}
