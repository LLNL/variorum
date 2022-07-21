// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    /* 02/25/19 SB
     * How do we distinguish errors if this function pointer does not exist
     * because it has not yet been implemented or if it cannot be implemented?
     */
    const char *usage = "Usage: %s [-h] [-v]\n";
    int opt;
    while ((opt = getopt(argc, argv, "hv")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf(usage, argv[0]);
                return 0;
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }

    ret = variorum_print_available_frequencies();
    if (ret != 0)
    {
        printf("Print available frequencies failed!\n");
    }
    return ret;
}
