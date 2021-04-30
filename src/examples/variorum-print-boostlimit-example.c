// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = variorum_print_boostlimit();
    if (ret != 0)
    {
        printf("Print core boostlimit failed!\n");
    }
    return ret;
}
