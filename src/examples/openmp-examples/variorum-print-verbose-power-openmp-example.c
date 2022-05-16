// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <omp.h>

#include <variorum.h>

#ifdef SECOND_RUN
static inline double do_work(int input)
{
    int i;
    double result = (double)input;

    for (i = 0; i < 100000; i++)
    {
        result += i * result;
    }

    return result;
}
#endif

int main()
{
    int ret;
    int tid;
#ifdef SECOND_RUN
    int i;
    int size = 1E3;
    double x = 0.0;
#endif

    #pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();

        // higher-level software must check for thread and process safety
        // we assume thread 0 is responsible for monitor and control
        if (tid == 0)
        {
            ret = variorum_print_verbose_power();
            if (ret != 0)
            {
                printf("Print verbose power failed!\n");
            }
#ifdef SECOND_RUN
            for (i = 0; i < size; i++)
            {
                x += do_work(i);
            }
            ret = variorum_print_verbose_power();
            if (ret != 0)
            {
                printf("Print verbose power failed!\n");
            }
#endif
        }
    }

    return ret;
}
