// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <stdio.h>

#include <variorum.h>

#ifdef SECOND_RUN
static inline double do_work(int input)
{
    int i;
    double result = (double)input;

    for (i = 0; i < 100000; i++)
    {
        result += i * result;
    }

    return result;
}
#endif

int main(int argc, char **argv)
{
    int ret;
#ifdef SECOND_RUN
    int i;
    int size = 1E3;
    double x = 0.0;
#endif

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

    ret = variorum_poll_power(stdout);
    if (ret != 0)
    {
        printf("Poll power failed!\n");
    }
#ifdef SECOND_RUN
    for (i = 0; i < size; i++)
    {
        x += do_work(i);
    }
    ret = variorum_poll_power(stdout);
    if (ret != 0)
    {
        printf("Poll power failed!\n");
    }
#endif
    return ret;
}
