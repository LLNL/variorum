// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <variorum.h>

int main(int argc, char **argv)
{
    const char *usage = "Usage: %s [-vh] \n";
    int opt;
    while ((opt = getopt(argc, argv, "hv")) != -1)
    {
        switch (opt)
        {
            case 'v':
                printf("%s\n", variorum_get_current_version());
                exit(0);
                break;
            case 'h':
                printf(usage, argv[0]);
                exit(0);
                break;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }
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
