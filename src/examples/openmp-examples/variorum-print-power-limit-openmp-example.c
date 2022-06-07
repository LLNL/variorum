// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <omp.h>

#include <variorum.h>

int main()
{
    int ret;
    int tid;

    #pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();

        // higher-level software must check for thread and process safety
        // we assume thread 0 is responsible for monitor and control
        if (tid == 0)
        {
            ret = variorum_print_power_limit();
            if (ret != 0)
            {
                printf("Print power limit failed!\n");
            }
        }
    }

    return ret;
}
