// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <getopt.h>
#include <variorum.h>
#include <variorum_config.h>

int main(int argc, char **argv)
{
    const char *usage = "Usage: %s [-h] \n";
    int opt;
    while ((opt = getopt(argc, argv, "hv")) != -1)
    {
        switch (opt)
        {
            case 'v':
                printf(QuoteMacro(VARIORUM_VERSION)"\n");
                return 0;
                break;
            case 'h':
                printf(usage, argv[0]);
                return 0;
                break;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }

    int ret;

    ret = variorum_print_features();
    if (ret != 0)
    {
        printf("Print features failed!\n");
    }
    return ret;
}
