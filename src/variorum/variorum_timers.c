// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <sys/time.h>
#include <time.h>

#include <variorum_timers.h>

unsigned long now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    unsigned long sec = t.tv_sec * 1000;
    unsigned long msec = (t.tv_nsec + 500000) / 1000000;
    return sec + msec;
}

int timer_sleep(struct mstimer *t)
{
    unsigned long now = now_ms();
    if (now >= t->nextms)
    {
        int cadd = 0;
        while (t->nextms <= now)
        {
            cadd++;
            t->step++;
            t->nextms = t->startms + t->step * t->interval;
        }
        /* We slept this many intervals. */
        return cadd;
    }
    sleep_ms(t->nextms - now);
    t->step++;
    t->nextms = t->startms + t->step * t->interval;
    return 0;
}

void init_msTimer(struct mstimer *t, int ms_interval)
{
    t->step = 1;
    t->interval = ms_interval;
    t->startms = now_ms();
    t->nextms = t->startms + t->step * t->interval;
}

void sleep_ms(long ms)
{
    struct timeval i;
    i.tv_sec = ms / 1000;
    i.tv_usec = (ms%1000) * 1000;
    select(0, NULL, NULL, NULL, &i);
}
