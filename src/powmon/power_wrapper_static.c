// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "highlander.h"

#if 0
/********/
/* RAPL */
/********/
static double total_joules = 0.0;
static double limit_joules = 0.0;
static double max_watts = 0.0;
static double min_watts = 1024.0;
#endif

/*************************/
/* HW Counter Structures */
/*************************/
static unsigned long start;
static unsigned long end;
static FILE *logfile = NULL;
static FILE *summaryfile = NULL;
static int watt_cap = 0;

static pthread_mutex_t mlock;
static int *shmseg;
static int shmid;

static int running = 1;

#include "common.c"

int main(int argc, char **argv)
{
    const char *usage = "\n"
                        "NAME\n"
                        "    power_wrapper_static - monitor power and statically enforce power cap\n"
                        "\n"
                        "SYNOPSIS\n"
                        "    power_wrapper_static [--help | -h] [-c] -w pcap -a \"executable [exec-args]\"\n"
                        "\n"
                        "OVERVIEW\n"
                        "    Power_wrapper_static is a utility for setting a package-level power cap, and\n"
                        "    sampling and printing the power usage (for package and DRAM) and power\n"
                        "    limits per socket in a node.\n"
                        "\n"
                        "OPTIONS\n"
                        "    --help | -h\n"
                        "        Display this help information, then exit.\n"
                        "\n"
                        "    -a \"executable [exec-args]\"\n"
                        "        Application and arguments surrounded by quotes\n"
                        "\n"
                        "    -w pcap\n"
                        "        Package-level power cap (integer).\n"
                        "\n"
                        "    -c\n"
                        "        Remove stale shared memory.\n"
                        "\n";
    if (argc == 1 || (argc > 1 && (
                          strncmp(argv[1], "--help", strlen("--help")) == 0 ||
                          strncmp(argv[1], "-h", strlen("-h")) == 0 )))
    {
        printf("%s", usage);
        return 0;
    }

    int opt;
    char *app;
    char **arg = NULL;

    while ((opt = getopt(argc, argv, "cw:a:")) != -1)
    {
        switch(opt)
        {
            case 'c':
                highlander_clean();
                printf("Exiting power_wrapper_static...\n");
                return 0;
            case 'a':
                app = optarg;
                break;
            case 'w':
                watt_cap = atoi(optarg);
                break;
            case '?':
                if (optopt == 'w' || optopt == 'a')
                {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                else if (isprint(optopt))
                {
                    fprintf(stderr, "\nError: unknown parameter \"-%c\"\n", optopt);
                }
                else
                {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
                fprintf(stderr, "%s", usage);
                return 1;
        }
    }

    char *app_split = strtok(app, " ");
    int n_spaces = 0;
    while (app_split)
    {
        arg = realloc(arg, sizeof(char *) * ++n_spaces);

        if (arg == NULL)
        {
            return 1; /* memory allocation failed */
        }
        arg[n_spaces-1] = app_split;
        app_split = strtok(NULL, " ");
    }
    arg = realloc(arg, sizeof(char *) * (n_spaces + 1));
    arg[n_spaces] = 0;

#ifdef VARIORUM_DEBUG
    int i;
    for (i = 0; i < (n_spaces+1); ++i)
    {
        printf("arg[%d] = %s\n", i, arg[i]);
    }
#endif

    char *fname_dat;
    char *fname_summary;
    if (highlander())
    {
        /* Start the log file. */
        int logfd;
        char hostname[64];
        gethostname(hostname, 64);

        asprintf(&fname_dat, "%s.powmon.dat", hostname);

        logfd = open(fname_dat, O_WRONLY|O_CREAT|O_EXCL|O_NOATIME|O_NDELAY, S_IRUSR|S_IWUSR);
        if (logfd < 0)
        {
            fprintf(stderr, "Fatal Error: %s on %s cannot open the appropriate fd for %s -- %s.\n", argv[0], hostname, fname_dat, strerror(errno));
            return 1;
        }
        logfile = fdopen(logfd, "w");
        if (logfile == NULL)
        {
            fprintf(stderr, "Fatal Error: %s on %s fdopen failed for %s -- %s.\n", argv[0], hostname, fname_dat, strerror(errno));
            return 1;
        }

        //read_rapl_init();

        /* Set the cap. */
        //set_rapl_power(watt_cap, watt_cap);
        printf("Setting each package power limit to %dW\n", watt_cap);
        variorum_set_each_socket_power_limit(watt_cap);

        /* Start power measurement thread. */
        pthread_attr_t mattr;
        pthread_t mthread;
        pthread_attr_init(&mattr);
        pthread_attr_setdetachstate(&mattr, PTHREAD_CREATE_DETACHED);
        pthread_mutex_init(&mlock, NULL);
        pthread_create(&mthread, &mattr, power_measurement, NULL);

        /* Fork. */
        pid_t app_pid = fork();
        if (app_pid == 0)
        {
            /* I'm the child. */
            printf("Profiling:");
            int i = 0;
            for (i = 0; i < n_spaces; i++)
            {
                printf(" %s", arg[i]);
            }
            printf("\n");
            execvp(arg[0], &arg[0]);
            printf("Fork failure\n");
            return 1;
        }
        /* Wait. */
        waitpid(app_pid, NULL, 0);
        sleep(1);

        highlander_wait();

        /* Stop power measurement thread. */
        running = 0;
        take_measurement();
        end = now_ms();

        /* Output summary data. */
        asprintf(&fname_summary, "%s.power.summary", hostname);

        logfd = open(fname_summary, O_WRONLY|O_CREAT|O_EXCL|O_NOATIME|O_NDELAY, S_IRUSR|S_IWUSR);
        if (logfd < 0)
        {
            fprintf(stderr, "Fatal Error: %s on %s cannot open the appropriate fd for %s -- %s.\n", argv[0], hostname, fname_summary, strerror(errno));
            return 1;
        }
        summaryfile = fdopen(logfd, "w");
        if (summaryfile == NULL)
        {
            fprintf(stderr, "Fatal Error: %s on %s fdopen failed for %s -- %s.\n", argv[0], hostname, fname_summary, strerror(errno));
            return 1;
        }

        char *msg;
        //asprintf(&msg, "host: %s\npid: %d\ntotal: %lf\nallocated: %lf\nmax_watts: %lf\nmin_watts: %lf\nruntime ms: %lu\n,start: %lu\nend: %lu\n", hostname, app_pid, total_joules, limit_joules, max_watts, min_watts, end-start, start, end);
        asprintf(&msg, "host: %s\npid: %d\nruntime ms: %lu\nstart: %lu\nend: %lu\n", hostname, app_pid, end-start, start, end);

        fprintf(summaryfile, "%s", msg);
        fclose(summaryfile);
        close(logfd);

        shmctl(shmid, IPC_RMID, NULL);
        shmdt(shmseg);

        pthread_attr_destroy(&mattr);
    }
    else
    {
        /* Fork. */
        pid_t app_pid = fork();
        if (app_pid == 0)
        {
            /* I'm the child. */
            execvp(arg[0], &arg[0]);
            printf("Fork failure: %s\n", argv[1]);
            return 1;
        }
        /* Wait. */
        waitpid(app_pid, NULL, 0);

        highlander_wait();
    }

    printf("Output Files\n"
           "  %s\n"
           "  %s\n\n", fname_dat, fname_summary);
    highlander_clean();
    return 0;
}
