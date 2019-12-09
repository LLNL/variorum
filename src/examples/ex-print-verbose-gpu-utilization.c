// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = variorum_print_verbose_gpu_utilization();
    if (ret != 0)
    {
        printf("Print verbose GPU utilization failed!\n");
    }
    return ret;
}
