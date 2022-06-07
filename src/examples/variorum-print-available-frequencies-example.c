// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum.h>

int main(void)
{
    int ret;

    /* 02/25/19 SB
     * How do we distinguish errors if this function pointer does not exist
     * because it has not yet been implemented or if it cannot be implemented?
     */
    ret = variorum_print_available_frequencies();
    if (ret != 0)
    {
        printf("Print available frequencies failed!\n");
    }
    return ret;
}
