/* power_wrapper_dynamic.c
 *
 * Copyright (c) 2011-2016, Lawrence Livermore National Security, LLC.
 * LLNL-CODE-645430
 *
 * Produced at Lawrence Livermore National Laboratory
 * Written by  Barry Rountree,   rountree@llnl.gov
 *             Daniel Ellsworth, ellsworth8@llnl.gov
 *             Scott Walker,     walker91@llnl.gov
 *             Kathleen Shoga,   shoga1@llnl.gov
 *
 * All rights reserved.
 *
 * This file is part of libmsr.
 *
 * libmsr is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * libmsr is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libmsr. If not, see <http://www.gnu.org/licenses/>.
 *
 * This material is based upon work supported by the U.S. Department of
 * Energy's Lawrence Livermore National Laboratory. Office of Science, under
 * Award number DE-AC52-07NA27344.
 *
 */

#define _GNU_SOURCE

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
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

static unsigned long start;
static unsigned long end;
static FILE *summaryfile = NULL;
static double watt_cap = 0.0;
static volatile int poll_num = 0;
static volatile int poll_dir = 5;

static pthread_mutex_t mlock;
static int *shmseg;
static int shmid;

static int running = 1;

#include "common.c"

void *power_set_measurement(void *arg)
{
    unsigned long poll_num = 0;
    struct mstimer timer;
    //set_rapl_power(watt_cap, watt_cap);
    set_each_socket_power_limit(watt_cap);
    double watts = watt_cap;
    // According to the Intel docs, the counter wraps a most once per second.
    // 100 ms should be short enough to always get good information.
    init_msTimer(&timer, 1500);
    init_data();
    start = now_ms();

    poll_num++;
    timer_sleep(&timer);
    while (running)
    {
        take_measurement();
        if (poll_num%5 == 0)
        {
            if (watts >= watt_cap)
            {
                poll_dir = -5;
            }
            if (watts <= 30)
            {
                poll_dir = 5;
            }
            watts += poll_dir;
            //set_rapl_power(watts, watts);
            set_each_socket_power_limit(watt_cap);
        }
        poll_num++;
        timer_sleep(&timer);
    }
}

int main(int argc, char**argv)
{
    if (argc < 3)
    {
        printf("Usage: %s <watt_cap> <executable> <args>...\n", argv[0]);
        return 1;
    }

    if (highlander())
    {
        /* Start the log file. */
        int logfd;
        char hostname[64];
        gethostname(hostname, 64);

        char *fname;
#if 0
        asprintf(&fname, "%s.power.dat", hostname);

        logfd = open(fname, O_WRONLY|O_CREAT|O_EXCL|O_NOATIME|O_NDELAY, S_IRUSR|S_IWUSR);
        if (logfd < 0)
        {
            printf( "Fatal Error: %s on %s cannot open the appropriate fd.\n", argv[0], hostname);
            return 1;
        }
        logfile = fdopen(logfd, "w");
#endif

        //read_rapl_init();

        /* Set the cap. */
        watt_cap = strtod(argv[1], NULL);
        //set_rapl_power(watt_cap, watt_cap);
        set_each_socket_power_limit(watt_cap);

        /* Start power measurement thread. */
        pthread_t mthread;
        pthread_create(&mthread, NULL, power_set_measurement, NULL);

        /* Fork. */
        pid_t app_pid = fork();
        if (app_pid == 0)
        {
            /* I'm the child. */
            execvp(argv[2], &argv[2]);
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

        asprintf(&fname, "%s.power.summary", hostname);

        logfd = open(fname, O_WRONLY|O_CREAT|O_EXCL|O_NOATIME|O_NDELAY, S_IRUSR|S_IWUSR);
        if (logfd < 0)
        {
            printf("Fatal Error: %s on %s cannot open the appropriate fd.\n", argv[0], hostname);
            return 1;
        }
        summaryfile = fdopen(logfd, "w");
        /* Output summary data. */
        char *msg;
        //asprintf(&msg, "host: %s\npid: %d\ntotal: %lf\nallocated: %lf\nmax_watts: %lf\nmin_watts: %lf\nruntime ms: %lu\n,start: %lu\nend: %lu\n", hostname, app_pid, total_joules, limit_joules, max_watts, min_watts, end-start, start, end);
        asprintf(&msg, "host: %s\npid: %d\nruntime ms: %lu\nstart: %lu\nend: %lu\n", hostname, app_pid, end-start, start, end);

        fprintf(summaryfile, "%s", msg);
        fclose(summaryfile);
        close(logfd);
        shmctl(shmid, IPC_RMID, NULL);
        shmdt(shmseg);
    }
    else
    {
        /* Fork. */
        pid_t app_pid = fork();
        if (app_pid == 0)
        {
            /* I'm the child. */
            execvp(argv[2], &argv[2]);
            return 1;
        }
        /* Wait. */
        waitpid(app_pid, NULL, 0);

        highlander_wait();
    }

    return 0;
}
