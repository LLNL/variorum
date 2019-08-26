/* common.c
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
 * This file is part of libvariorum.
 *
 * libvariorum is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * libvariorum is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libvariorum. If not, see <http://www.gnu.org/licenses/>.
 *
 * This material is based upon work supported by the U.S. Department of
 * Energy's Lawrence Livermore National Laboratory. Office of Science, under
 * Award number DE-AC52-07NA27344.
 *
 */

#include <stdint.h>

#include <variorum.h>
#include <variorum_timers.h>

int init_data(void)
{
    return 0;
}

void take_measurement(void)
{
#if 0
    uint64_t instr0 = 0;
    uint64_t instr1 = 0;
    uint64_t core0 = 0;
    uint64_t core1 = 0;
    double rapl_data[10];
#endif
    pthread_mutex_lock(&mlock);

    /* RAPL reads. */
    //poll_power(logfile); // PKG/DRAM energy, PKG/DRAM power limit
    monitoring(logfile); // PKG/DRAM energy, PKG/DRAM power limit, fixed counters, TSC, APERF, MPERF

#if 0
    total_joules += rapl_data[0] + rapl_data[1];
    limit_joules += rapl_data[2] + rapl_data[3];
    if (max_watts < rapl_data[4])
    {
        max_watts = rapl_data[4];
    }
    if (max_watts < rapl_data[5])
    {
        max_watts = rapl_data[5];
    }
    if (min_watts > rapl_data[4])
    {
        min_watts = rapl_data[4];
    }
    if (min_watts > rapl_data[5])
    {
        min_watts = rapl_data[5];
    }
    fprintf(logfile, "%ld %lf %lf %lf %lf %lf %lf %lu %lu %lu %lu\n", now_ms(), rapl_data[0], rapl_data[1], rapl_data[6], rapl_data[7], rapl_data[8], rapl_data[9], instr0, instr1, core0, core1);
#endif
    pthread_mutex_unlock(&mlock);
}

void *power_measurement(void *arg)
{
    struct mstimer timer;
    unsigned long sample_interval = *(unsigned long *)arg;
    // According to the Intel docs, the counter wraps at most once per second.
    // 50 ms should be short enough to always get good information (this is
    // default).
    printf("Using sampling interval of: %ld ms\n", sample_interval);
    init_msTimer(&timer, sample_interval);
    start = now_ms();

    timer_sleep(&timer);
    while (running)
    {
        take_measurement();
        timer_sleep(&timer);
    }
    return arg;
}
