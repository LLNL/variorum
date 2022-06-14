// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <variorum_config.h>
#include <getopt.h>
#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = variorum_print_verbose_frequency();
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
    if (ret != 0)
    {
        printf("Print verbose frequency failed!\n");
    }
#ifdef SECOND_RUN
    ret = variorum_print_verbose_frequency();
#endif
    return ret;
}
