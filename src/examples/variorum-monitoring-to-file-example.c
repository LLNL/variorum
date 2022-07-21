// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <getopt.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
    char hostname[1024];
    char *fname;
#ifdef SECOND_RUN
    int i;
    int size = 1E3;
    double x = 0.0;
#endif

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
    if (argc > 1)
    {
        printf("Fatal Error: No argument needed.\n");
        return 1;
    }

    gethostname(hostname, 1024);
    ret = asprintf(&fname, "%s.powmon.dat", hostname);
    if (ret < 0)
    {
        printf("Fatal Error: Cannot allocate memory for fname.\n");
        return 1;
    }

    int logfd = open(fname, O_WRONLY | O_CREAT | O_EXCL | O_NDELAY,
                     S_IRUSR | S_IWUSR);
    if (logfd < 0)
    {
        printf("Fatal Error: %s on %s cannot open the appropriate fd.\n", argv[0],
               hostname);
        return 1;
    }
    FILE *logfile = fdopen(logfd, "w");

    ret = variorum_monitoring(logfile);
    if (ret != 0)
    {
        printf("Monitoring failed!\n");
    }
#ifdef SECOND_RUN
    for (i = 0; i < size; i++)
    {
        x += do_work(i);
    }
    ret = variorum_poll_power(logfile);
    if (ret != 0)
    {
        printf("Monitoring failed!\n");
    }
#endif
    fclose(logfile);
    return ret;
}
