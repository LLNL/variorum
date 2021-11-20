// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum.h>

int main(void)
{
    int ret;

    ret = variorum_print_verbose_frequency();
    if (ret != 0)
    {
        printf("Print verbose frequency failed!\n");
    }
#ifdef SECOND_RUN
    ret = variorum_print_verbose_frequency();
#endif
    return ret;
}
