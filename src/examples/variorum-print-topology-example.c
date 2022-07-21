// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    const char *usage = "Usage: %s [-hv]\n";
    int opt;
    while ((opt = getopt(argc, argv, "hv")) != -1)
    {
        switch (opt)
        {
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
            case 'h':
                printf(usage, argv[0]);
                return 0;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }
    variorum_print_topology();
    return 0;
}
