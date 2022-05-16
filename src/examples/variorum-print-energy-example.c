// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum.h>

int main()
{
    int ret;

    ret = variorum_print_energy();
    if (ret != 0)
    {
        printf("Print core and Socket energy failed!\n");
    }
    return ret;
}
