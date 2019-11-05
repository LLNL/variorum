// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <clocks_features.h>
#include <counters_features.h>
#include <config_architecture.h>
#include <msr_core.h>
#include <power_features.h>
#include <variorum_cpuid.h>
#include <variorum_error.h>
#include <variorum_timers.h>

int cpuid_num_pmc(void)
{
    /* See Manual Vol 3B, Section 18.2.5 for details. */
    uint64_t rax, rbx, rcx, rdx;
    int leaf = 10; // 0A

    cpuid(leaf, &rax, &rbx, &rcx, &rdx);
    // If CPUID.0AH:rax[15:8] > 3, then up to PMC3 is usable
    // If CPUID.0AH:rax[15:8] > 2, then up to PMC2 is usable
    // If CPUID.0AH:rax[15:8] > 1, then up to PMC1 is usable
    // If CPUID.0AH:rax[15:8] > 0, then only PMC0 is usable
    // If CPUID.0AH:rax[15:8] == 0, then none are usable
    return MASK_VAL(rax, 15, 8);
}

/*****************************************/
/* Fixed Counters Performance Monitoring */
/*****************************************/

void fixed_counter_storage(struct fixed_counter **ctr0, struct fixed_counter **ctr1, struct fixed_counter **ctr2, off_t *msrs_fixed_ctrs)
{
    static struct fixed_counter c0, c1, c2;
    static int init = 0;
    static int nthreads;

    if (!init)
    {
        init = 1;
        variorum_set_topology(NULL, NULL, &nthreads);
        init_fixed_counter(&c0);
        init_fixed_counter(&c1);
        init_fixed_counter(&c2);
        allocate_batch(FIXED_COUNTERS_DATA, 3UL * nthreads);
        load_thread_batch(msrs_fixed_ctrs[0], c0.value, FIXED_COUNTERS_DATA);
        load_thread_batch(msrs_fixed_ctrs[1], c1.value, FIXED_COUNTERS_DATA);
        load_thread_batch(msrs_fixed_ctrs[2], c2.value, FIXED_COUNTERS_DATA);
    }
    if (ctr0 != NULL)
    {
        *ctr0 = &c0;
    }
    if (ctr1 != NULL)
    {
        *ctr1 = &c1;
    }
    if (ctr2 != NULL)
    {
        *ctr2 = &c2;
    }
}

void init_fixed_counter(struct fixed_counter *ctr)
{
    int nthreads = 0;
    variorum_set_topology(NULL, NULL, &nthreads);

    ctr->enable = (uint64_t *) malloc(nthreads * sizeof(uint64_t));
#ifdef VARIORUM_DEBUG
    fprintf(stderr, "DEBUG: note q1, ctr->enable is at %p\n", ctr->enable);
#endif
    ctr->ring_level = (uint64_t *) malloc(nthreads * sizeof(uint64_t));
    ctr->anyThread = (uint64_t *) malloc(nthreads * sizeof(uint64_t));
    ctr->pmi = (uint64_t *) malloc(nthreads * sizeof(uint64_t));
    ctr->overflow = (uint64_t *) malloc(nthreads * sizeof(uint64_t));
    ctr->value = (uint64_t **) malloc(nthreads * sizeof(uint64_t *));
}

void enable_fixed_counters(off_t *msrs_fixed_ctrs, off_t msr1, off_t msr2)
{
    struct fixed_counter *c0, *c1, *c2;
    int i;
    int nthreads = 0;

    variorum_set_topology(NULL, NULL, &nthreads);
    fixed_counter_storage(&c0, &c1, &c2, msrs_fixed_ctrs);

    for (i = 0; i < nthreads; i++)
    {
        c0->enable[i] = c1->enable[i] = c2->enable[i] = 1;
        c0->ring_level[i] = c1->ring_level[i] = c2->ring_level[i] = 3; // usr + os
        c0->anyThread[i] = c1->anyThread[i] = c2->anyThread[i] = 1;
        c0->pmi[i] = c1->pmi[i] = c2->pmi[i] = 0;
    }
    set_fixed_counter_ctrl(c0, c1, c2, msr1, msr2);
}

void disable_fixed_counters(off_t *msrs_fixed_ctrs, off_t msr1, off_t msr2)
{
    struct fixed_counter *c0, *c1, *c2;
    int i;
    int nthreads = 0;

    variorum_set_topology(NULL, NULL, &nthreads);
    fixed_counter_storage(&c0, &c1, &c2, msrs_fixed_ctrs);

    for (i = 0; i < nthreads; i++)
    {
        c0->enable[i] = c1->enable[i] = c2->enable[i] = 0;
        c0->ring_level[i] = c1->ring_level[i] = c2->ring_level[i] = 3; // usr + os
        c0->anyThread[i] = c1->anyThread[i] = c2->anyThread[i] = 1;
        c0->pmi[i] = c1->pmi[i] = c2->pmi[i] = 0;
    }
    set_fixed_counter_ctrl(c0, c1, c2, msr1, msr2);
}

void set_fixed_counter_ctrl(struct fixed_counter *ctr0, struct fixed_counter *ctr1, struct fixed_counter *ctr2, off_t msr1, off_t msr2)
{
    static uint64_t **perf_global_ctrl = NULL;
    static uint64_t **fixed_ctr_ctrl = NULL;
    static int init = 0;
    int i;
    int nthreads = 0;

    if (!init)
    {
        fixed_counter_ctrl_storage(&perf_global_ctrl, &fixed_ctr_ctrl, msr1, msr2);
        init = 1;
    }

    variorum_set_topology(NULL, NULL, &nthreads);

    /* Don't need to read counters data, we are just zeroing things out. */
    read_batch(FIXED_COUNTERS_CTRL_DATA);

    for (i = 0; i < nthreads; i++)
    {
        *ctr0->value[i] = 0;
        *ctr1->value[i] = 0;
        *ctr2->value[i] = 0;
        *perf_global_ctrl[i] = (*perf_global_ctrl[i] & ~(1ULL<<32)) | ctr0->enable[i] << 32;
        *perf_global_ctrl[i] = (*perf_global_ctrl[i] & ~(1ULL<<33)) | ctr1->enable[i] << 33;
        *perf_global_ctrl[i] = (*perf_global_ctrl[i] & ~(1ULL<<34)) | ctr2->enable[i] << 34;

        *fixed_ctr_ctrl[i] = (*fixed_ctr_ctrl[i] & (~(3ULL<<0))) | (ctr0->ring_level[i] << 0);
        *fixed_ctr_ctrl[i] = (*fixed_ctr_ctrl[i] & (~(1ULL<<2))) | (ctr0->anyThread[i] << 2);
        *fixed_ctr_ctrl[i] = (*fixed_ctr_ctrl[i] & (~(1ULL<<3))) | (ctr0->pmi[i] << 3);

        *fixed_ctr_ctrl[i] = (*fixed_ctr_ctrl[i] & (~(3ULL<<4))) | (ctr1->ring_level[i] << 4);
        *fixed_ctr_ctrl[i] = (*fixed_ctr_ctrl[i] & (~(1ULL<<6))) | (ctr1->anyThread[i] << 6);
        *fixed_ctr_ctrl[i] = (*fixed_ctr_ctrl[i] & (~(1ULL<<7))) | (ctr1->pmi[i] << 7);

        *fixed_ctr_ctrl[i] = (*fixed_ctr_ctrl[i] & (~(3ULL<<8)))  | (ctr2->ring_level[i] << 8);
        *fixed_ctr_ctrl[i] = (*fixed_ctr_ctrl[i] & (~(1ULL<<10))) | (ctr2->anyThread[i] << 10);
        *fixed_ctr_ctrl[i] = (*fixed_ctr_ctrl[i] & (~(1ULL<<11))) | (ctr2->pmi[i] << 11);
    }
    write_batch(FIXED_COUNTERS_CTRL_DATA);
    //write_batch(FIXED_COUNTERS_DATA);
}

void fixed_counter_ctrl_storage(uint64_t ***perf_ctrl, uint64_t ***fixed_ctrl, off_t msr_perf_global_ctrl, off_t msr_fixed_counter_ctrl)
{
    static uint64_t **perf_global_ctrl = NULL;
    static uint64_t **fixed_ctr_ctrl = NULL;
    static int nthreads = 0;
    static int init = 0;

    if (!init)
    {
        variorum_set_topology(NULL, NULL, &nthreads);
        perf_global_ctrl = (uint64_t **) malloc(nthreads * sizeof(uint64_t *));
        fixed_ctr_ctrl = (uint64_t **) malloc(nthreads * sizeof(uint64_t *));
        allocate_batch(FIXED_COUNTERS_CTRL_DATA, 2UL * nthreads);
        load_thread_batch(msr_perf_global_ctrl, perf_global_ctrl, FIXED_COUNTERS_CTRL_DATA);
        load_thread_batch(msr_fixed_counter_ctrl, fixed_ctr_ctrl, FIXED_COUNTERS_CTRL_DATA);
        init = 1;
    }
    if (perf_ctrl != NULL)
    {
        *perf_ctrl = perf_global_ctrl;
    }
    if (fixed_ctrl != NULL)
    {
        *fixed_ctrl = fixed_ctr_ctrl;
    }
}

void dump_all_counter_data(FILE *writedest, off_t *msrs_fixed_ctrs, off_t msr_perf_global_ctrl, off_t msr_fixed_counter_ctrl, off_t *msrs_perfevtsel_ctrs, off_t *msrs_perfmon_ctrs, off_t *msrs_pcu_pmon_evtsel, off_t *msrs_pcu_pmon_ctrs)
{
    dump_fixed_counter_data(writedest, msrs_fixed_ctrs, msr_perf_global_ctrl, msr_fixed_counter_ctrl);
    dump_perfmon_counter_data(writedest, msrs_perfevtsel_ctrs, msrs_perfmon_ctrs);
    dump_unc_counter_data(writedest, msrs_pcu_pmon_evtsel, msrs_pcu_pmon_ctrs);
}

void print_all_counter_data(FILE *writedest, off_t *msrs_fixed_ctrs, off_t msr_perf_global_ctrl, off_t msr_fixed_counter_ctrl, off_t *msrs_perfevtsel_ctrs, off_t *msrs_perfmon_ctrs, off_t *msrs_pcu_pmon_evtsel, off_t *msrs_pcu_pmon_ctrs)
{
    print_fixed_counter_data(writedest, msrs_fixed_ctrs, msr_perf_global_ctrl, msr_fixed_counter_ctrl);
    print_perfmon_counter_data(writedest, msrs_perfevtsel_ctrs, msrs_perfmon_ctrs);
    print_unc_counter_data(writedest, msrs_pcu_pmon_evtsel, msrs_pcu_pmon_ctrs);
}

void dump_fixed_counter_data(FILE *writedest, off_t *msrs_fixed_ctrs, off_t msr_perf_global_ctrl, off_t msr_fixed_counter_ctrl)
{
    static int init = 0;
    struct fixed_counter *c0, *c1, *c2;
    int i;
    char hostname[1024];
    int nthreads = 0;

    if (!init)
    {
        fprintf(writedest, "_FIXED_COUNTERS | Host | Thread | InstRet | UnhaltClkCycles | UnhaltRefCycles\n");
        init = 1;
    }
    variorum_set_topology(NULL, NULL, &nthreads);
    gethostname(hostname, 1024);
    fixed_counter_storage(&c0, &c1, &c2, msrs_fixed_ctrs);

    read_batch(FIXED_COUNTERS_DATA);
    for (i = 0; i < nthreads; i++)
    {
        fprintf(writedest, "_FIXED_COUNTERS | %s | %d | %lu | %lu | %lu\n", hostname, i, *c0->value[i], *c1->value[i], *c2->value[i]);
    }
}

void dump_perfmon_counter_data(FILE *writedest, off_t *msrs_perfevtsel_ctrs, off_t *msrs_perfmon_ctrs)
{
    static struct pmc *p = NULL;
    static int init = 0;
    int i;
    char hostname[1024];
    int nthreads;
    int avail = 0;

    gethostname(hostname, 1024);
    avail = cpuid_num_pmc();
    variorum_set_topology(NULL, NULL, &nthreads);

    if (p == NULL && !init)
    {
        switch(avail)
        {
            case 8:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | Host | Thread | PMC0 | PMC1 | PMC2 | PMC3 | PMC4 | PMC5 | PMC6 | PMC7\n");
                break;
            case 7:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | Host | Thread | PMC0 | PMC1 | PMC2 | PMC3 | PMC4 | PMC5 | PMC6\n");
                break;
            case 6:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | Host | Thread | PMC0 | PMC1 | PMC2 | PMC3 | PMC4 | PMC5\n");
                break;
            case 5:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | Host | Thread | PMC0 | PMC1 | PMC2 | PMC3 | PMC4\n");
                break;
            case 4:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | Host | Thread | PMC0 | PMC1 | PMC2 | PMC3\n");
                break;
            case 3:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | Host | Thread | PMC0 | PMC1 | PMC2\n");
                break;
            case 2:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | Host | Thread | PMC0 | PMC1\n");
                break;
            case 1:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | Host | Thread | PMC0\n");
                break;
        }

        set_all_pmc_ctrl(0x0, 0x67, 0x00, 0xC4, 1, msrs_perfevtsel_ctrs);
        set_all_pmc_ctrl(0x0, 0x67, 0x00, 0xC4, 2, msrs_perfevtsel_ctrs);
        enable_pmc(msrs_perfevtsel_ctrs, msrs_perfmon_ctrs);
        pmc_storage(&p, msrs_perfmon_ctrs);
        init = 1;
    }

    read_batch(COUNTERS_DATA);
    for (i = 0; i < nthreads; i++)
    {
        switch (avail)
        {
            case 8:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | %s | %d | %lu | %lu | %lu | %lu | %lu | %lu | %lu | %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i], *p->pmc4[i], *p->pmc5[i], *p->pmc6[i], *p->pmc7[i]);
                break;
            case 7:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | %s | %d | %lu | %lu | %lu | %lu | %lu | %lu | %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i], *p->pmc4[i], *p->pmc5[i], *p->pmc6[i]);
                break;
            case 6:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | %s | %d | %lu | %lu | %lu | %lu | %lu | %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i], *p->pmc4[i], *p->pmc5[i]);
                break;
            case 5:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | %s | %d | %lu | %lu | %lu | %lu | %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i], *p->pmc4[i]);
                break;
            case 4:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | %s | %d | %lu | %lu | %lu | %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i]);
                break;
            case 3:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | %s | %d | %lu | %lu | %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i]);
                break;
            case 2:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | %s | %d | %lu | %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i]);
                break;
            case 1:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS | %s | %d | %lu\n",
                        hostname, i, *p->pmc0[i]);
                break;
        }
    }
}

void print_fixed_counter_data(FILE *writedest, off_t *msrs_fixed_ctrs, off_t msr_perf_global_ctrl, off_t msr_fixed_counter_ctrl)
{
    static int init = 0;
    struct fixed_counter *c0, *c1, *c2;
    int i;
    char hostname[1024];
    int nthreads = 0;

    if (!init)
    {
        init = 1;
    }
    variorum_set_topology(NULL, NULL, &nthreads);
    gethostname(hostname, 1024);
    fixed_counter_storage(&c0, &c1, &c2, msrs_fixed_ctrs);

    read_batch(FIXED_COUNTERS_DATA);
    for (i = 0; i < nthreads; i++)
    {
        fprintf(writedest, "_FIXED_COUNTERS Host: %s Thread: %d InstRet: %lu UnhaltClkCycles: %lu UnhaltRefCycles: %lu\n",
                hostname, i, *c0->value[i], *c1->value[i], *c2->value[i]);
    }
}

void print_perfmon_counter_data(FILE *writedest, off_t *msrs_perfevtsel_ctrs, off_t *msrs_perfmon_ctrs)
{
    static struct pmc *p = NULL;
    static int init = 0;
    int i;
    char hostname[1024];
    int nthreads;
    int avail = 0;

    gethostname(hostname, 1024);
    avail = cpuid_num_pmc();
    variorum_set_topology(NULL, NULL, &nthreads);

    if (p == NULL && !init)
    {
        set_all_pmc_ctrl(0x0, 0x67, 0x00, 0xC4, 1, msrs_perfevtsel_ctrs);
        set_all_pmc_ctrl(0x0, 0x67, 0x00, 0xC4, 2, msrs_perfevtsel_ctrs);
        enable_pmc(msrs_perfevtsel_ctrs, msrs_perfmon_ctrs);
        pmc_storage(&p, msrs_perfmon_ctrs);
        init = 1;
    }

    read_batch(COUNTERS_DATA);
    for (i = 0; i < nthreads; i++)
    {
        switch (avail)
        {
            case 8:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS Host: %s Thread: %d PMC0: %lu PMC1: %lu PMC2: %lu PMC3: %lu PMC4: %lu PMC5: %lu PMC6: %lu PMC7: %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i], *p->pmc4[i], *p->pmc5[i], *p->pmc6[i], *p->pmc7[i]);
                break;
            case 7:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS Host: %s Thread: %d PMC0: %lu PMC1: %lu PMC2: %lu PMC3: %lu PMC4: %lu PMC5: %lu PMC6: %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i], *p->pmc4[i], *p->pmc5[i], *p->pmc6[i]);
                break;
            case 6:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS Host: %s Thread: %d PMC0: %lu PMC1: %lu PMC2: %lu PMC3: %lu PMC4: %lu PMC5: %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i], *p->pmc4[i], *p->pmc5[i]);
                break;
            case 5:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS Host: %s Thread: %d PMC0: %lu PMC1: %lu PMC2: %lu PMC3: %lu PMC4: %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i], *p->pmc4[i]);
                break;
            case 4:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS Host: %s Thread: %d PMC0: %lu PMC1: %lu PMC2: %lu PMC3: %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i], *p->pmc3[i]);
                break;
            case 3:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS Host: %s Thread: %d PMC0: %lu PMC1: %lu PMC2: %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i], *p->pmc2[i]);
                break;
            case 2:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS Host: %s Thread: %d PMC0: %lu PMC1: %lu\n",
                        hostname, i, *p->pmc0[i], *p->pmc1[i]);
                break;
            case 1:
                fprintf(writedest, "_PERFORMANCE_MONITORING_COUNTERS Host: %s Thread: %d PMC0: %lu\n",
                        hostname, i, *p->pmc0[i]);
                break;
        }
    }
}

/**************************************/
/* Programmable Performance Counters  */
/**************************************/

/// @brief Initialize storage for general-purpose performance counter data.
///
/// @param [out] p Data for general-purpose performance counters.
/// @param [in] msrs_perfmon_ctrs Array of unique addresses for PERFMON_CTRS.
///
/// @return 0 if successful, else -1 if number of general-purpose performance
/// counters is less than 1.
static int init_pmc(struct pmc *p, off_t *msrs_perfmon_ctrs)
{
    int nthreads = 0;
    int avail = cpuid_num_pmc();

    variorum_set_topology(NULL, NULL, &nthreads);

    if (avail < 1)
    {
        return -1;
    }
    switch (avail)
    {
        case 8:
            p->pmc7 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 7:
            p->pmc6 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 6:
            p->pmc5 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 5:
            p->pmc4 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 4:
            p->pmc3 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 3:
            p->pmc2 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 2:
            p->pmc1 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 1:
            p->pmc0 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
    }
    allocate_batch(COUNTERS_DATA, avail * nthreads);
    switch (avail)
    {
        case 8:
            load_thread_batch(msrs_perfmon_ctrs[7], p->pmc7, COUNTERS_DATA);
        case 7:
            load_thread_batch(msrs_perfmon_ctrs[6], p->pmc6, COUNTERS_DATA);
        case 6:
            load_thread_batch(msrs_perfmon_ctrs[5], p->pmc5, COUNTERS_DATA);
        case 5:
            load_thread_batch(msrs_perfmon_ctrs[4], p->pmc4, COUNTERS_DATA);
        case 4:
            load_thread_batch(msrs_perfmon_ctrs[3], p->pmc3, COUNTERS_DATA);
        case 3:
            load_thread_batch(msrs_perfmon_ctrs[2], p->pmc2, COUNTERS_DATA);
        case 2:
            load_thread_batch(msrs_perfmon_ctrs[1], p->pmc1, COUNTERS_DATA);
        case 1:
            load_thread_batch(msrs_perfmon_ctrs[0], p->pmc0, COUNTERS_DATA);
    }
    return 0;
}

/// @brief Initialize storage for performance event select counter data.
///
/// @param [out] evt Data for performance event select counters.
/// @param [in] msrs_perfevtsel_ctrs Array of unique addresses for
///        PERFEVTSEL_CTRS.
///
/// @return 0 if successful, else -1 if number of general-purpose performance
/// counters is less than 1.
static int init_perfevtsel(struct perfevtsel *evt, off_t *msrs_perfevtsel_ctrs)
{
    int nthreads;
    int avail = cpuid_num_pmc();

    variorum_set_topology(NULL, NULL, &nthreads);

    if (avail < 1)
    {
        return -1;
    }
    switch (avail)
    {
        case 8:
            evt->perf_evtsel7 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 7:
            evt->perf_evtsel6 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 6:
            evt->perf_evtsel5 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 5:
            evt->perf_evtsel4 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 4:
            evt->perf_evtsel3 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 3:
            evt->perf_evtsel2 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 2:
            evt->perf_evtsel1 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
        case 1:
            evt->perf_evtsel0 = (uint64_t **) calloc(nthreads, sizeof(uint64_t *));
    }
    allocate_batch(COUNTERS_CTRL, avail * nthreads);
    switch (avail)
    {
        case 8:
            load_thread_batch(msrs_perfevtsel_ctrs[7], evt->perf_evtsel7, COUNTERS_CTRL);
        case 7:
            load_thread_batch(msrs_perfevtsel_ctrs[6], evt->perf_evtsel6, COUNTERS_CTRL);
        case 6:
            load_thread_batch(msrs_perfevtsel_ctrs[5], evt->perf_evtsel5, COUNTERS_CTRL);
        case 5:
            load_thread_batch(msrs_perfevtsel_ctrs[4], evt->perf_evtsel4, COUNTERS_CTRL);
        case 4:
            load_thread_batch(msrs_perfevtsel_ctrs[3], evt->perf_evtsel3, COUNTERS_CTRL);
        case 3:
            load_thread_batch(msrs_perfevtsel_ctrs[2], evt->perf_evtsel2, COUNTERS_CTRL);
        case 2:
            load_thread_batch(msrs_perfevtsel_ctrs[1], evt->perf_evtsel1, COUNTERS_CTRL);
        case 1:
            load_thread_batch(msrs_perfevtsel_ctrs[0], evt->perf_evtsel0, COUNTERS_CTRL);
    }
    return 0;
}

void set_all_pmc_ctrl(uint64_t cmask, uint64_t flags, uint64_t umask, uint64_t eventsel, int pmcnum, off_t *msrs_perfevtsel_ctrs)
{
    int nthreads;
    int i;

    variorum_set_topology(NULL, NULL, &nthreads);
    for (i = 0; i < nthreads; i++)
    {
        set_pmc_ctrl_flags(cmask, flags, umask, eventsel, pmcnum, i, msrs_perfevtsel_ctrs);
    }
}

int enable_pmc(off_t *msrs_perfevtsel_ctrs, off_t *msrs_perfmon_ctrs)
{
    static struct perfevtsel *evt = NULL;
    static int avail = 0;

    if (evt == NULL)
    {
        avail = cpuid_num_pmc();
        if (avail == 0)
        {
            return -1;
        }
        perfevtsel_storage(&evt, msrs_perfevtsel_ctrs);
    }
    write_batch(COUNTERS_CTRL);
    clear_all_pmc(msrs_perfmon_ctrs);
    return 0;
}

/* IA32_PEREVTSELx MSRs
 * cmask [31:24]
 * flags [23:16]
 * umask [15:8]
 * eventsel [7:0]
 */
void set_pmc_ctrl_flags(uint64_t cmask, uint64_t flags, uint64_t umask, uint64_t eventsel, int pmcnum, unsigned thread, off_t *msrs_perfevtsel_ctrs)
{
    static struct perfevtsel *evt = NULL;
    if (evt == NULL)
    {
        perfevtsel_storage(&evt, msrs_perfevtsel_ctrs);
    }
    switch(pmcnum)
    {
        case 8:
            *evt->perf_evtsel7[thread] = 0UL | (cmask << 24) | (flags << 16) | (umask << 8) | eventsel;
            break;
        case 7:
            *evt->perf_evtsel6[thread] = 0UL | (cmask << 24) | (flags << 16) | (umask << 8) | eventsel;
            break;
        case 6:
            *evt->perf_evtsel5[thread] = 0UL | (cmask << 24) | (flags << 16) | (umask << 8) | eventsel;
            break;
        case 5:
            *evt->perf_evtsel4[thread] = 0UL | (cmask << 24) | (flags << 16) | (umask << 8) | eventsel;
            break;
        case 4:
            *evt->perf_evtsel3[thread] = 0UL | (cmask << 24) | (flags << 16) | (umask << 8) | eventsel;
            break;
        case 3:
            *evt->perf_evtsel2[thread] = 0UL | (cmask << 24) | (flags << 16) | (umask << 8) | eventsel;
            break;
        case 2:
            *evt->perf_evtsel1[thread] = 0UL | (cmask << 24) | (flags << 16) | (umask << 8) | eventsel;
            break;
        case 1:
            *evt->perf_evtsel0[thread] = 0UL | (cmask << 24) | (flags << 16) | (umask << 8) | eventsel;
            break;
    }
}

void perfevtsel_storage(struct perfevtsel **e, off_t *msrs_perfevtsel_ctrs)
{
    static struct perfevtsel evt;
    static int init = 0;

    if (!init)
    {
        init_perfevtsel(&evt, msrs_perfevtsel_ctrs);
        init = 1;
    }
    if (e != NULL)
    {
        *e = &evt;
    }
}

void pmc_storage(struct pmc **p, off_t *msrs_perfmon_ctrs)
{
    static struct pmc counters;
    static int init = 0;

    if (!init)
    {
        init_pmc(&counters, msrs_perfmon_ctrs);
        init = 1;
    }
    if (p != NULL)
    {
        *p = &counters;
    }
}

void clear_all_pmc(off_t *msrs_perfmon_ctrs)
{
    static struct pmc *p = NULL;
    static int nthreads = 0;
    static int avail = 0;
    int i;

    if (p == NULL)
    {
        avail = cpuid_num_pmc();
        variorum_set_topology(NULL, NULL, &nthreads);
        pmc_storage(&p, msrs_perfmon_ctrs);
    }
    for (i = 0; i < nthreads; i++)
    {
        switch (avail)
        {
            case 8:
                *p->pmc7[i] = 0;
            case 7:
                *p->pmc6[i] = 0;
            case 6:
                *p->pmc5[i] = 0;
            case 5:
                *p->pmc4[i] = 0;
            case 4:
                *p->pmc3[i] = 0;
            case 3:
                *p->pmc2[i] = 0;
            case 2:
                *p->pmc1[i] = 0;
            case 1:
                *p->pmc0[i] = 0;
        }
    }
    write_batch(COUNTERS_DATA);
}

///*************************************/
///* Uncore PCU Performance Monitoring */
///*************************************/

/// @brief Initialize storage for uncore performance event select counter data.
///
/// @param [out] uevt Data for uncore performance event select counters.
/// @param [in] msrs_pcu_pmon_evtsel Array of unique addresses for
///        MSR_PCU_PMON_EVTSEL.
static void init_unc_perfevtsel(struct unc_perfevtsel *uevt, off_t *msrs_pcu_pmon_evtsel)
{
    static int init = 0;
    int nsockets;

    variorum_set_topology(&nsockets, NULL, NULL);

    if (!init)
    {
        uevt->c0 = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
        uevt->c1 = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
        uevt->c2 = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
        uevt->c3 = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
        allocate_batch(UNCORE_EVTSEL, 4 * nsockets);
        load_socket_batch(msrs_pcu_pmon_evtsel[0], uevt->c0, UNCORE_EVTSEL);
        load_socket_batch(msrs_pcu_pmon_evtsel[1], uevt->c1, UNCORE_EVTSEL);
        load_socket_batch(msrs_pcu_pmon_evtsel[2], uevt->c2, UNCORE_EVTSEL);
        load_socket_batch(msrs_pcu_pmon_evtsel[3], uevt->c3, UNCORE_EVTSEL);
        init = 1;
    }
}

/// @brief Initialize storage for uncore general-purpose performance event
/// counter data.
///
/// @param [out] uc Data for uncore general-purpose performance counters.
/// @param [in] msrs_pcu_pmon_ctrs Array of unique addresses for
///        MSR_PCU_PMON_CTRS.
static void init_unc_counters(struct unc_counters *uc, off_t *msrs_pcu_pmon_ctrs)
{
    static int init = 0;
    int nsockets;

    variorum_set_topology(&nsockets, NULL, NULL);
    if (!init)
    {
        uc->c0 = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
        uc->c1 = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
        uc->c2 = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
        uc->c3 = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
        allocate_batch(UNCORE_COUNT, 4 * nsockets);
        load_socket_batch(msrs_pcu_pmon_ctrs[0], uc->c0, UNCORE_COUNT);
        load_socket_batch(msrs_pcu_pmon_ctrs[1], uc->c1, UNCORE_COUNT);
        load_socket_batch(msrs_pcu_pmon_ctrs[2], uc->c2, UNCORE_COUNT);
        load_socket_batch(msrs_pcu_pmon_ctrs[3], uc->c3, UNCORE_COUNT);
        init = 1;
    }
}

void unc_perfevtsel_storage(struct unc_perfevtsel **uevt, off_t *msrs_pcu_pmon_evtsel)
{
    static struct unc_perfevtsel uevt_data;
    static int init = 0;
    if (!init)
    {
        init = 1;
        init_unc_perfevtsel(&uevt_data, msrs_pcu_pmon_evtsel);
    }
    if (uevt != NULL)
    {
        *uevt = &uevt_data;
    }
}

void unc_counters_storage(struct unc_counters **uc, off_t *msrs_pcu_pmon_ctrs)
{
    static struct unc_counters uc_data;
    static int init = 0;
    if (!init)
    {
        init = 1;
        init_unc_counters(&uc_data, msrs_pcu_pmon_ctrs);
    }
    if (uc != NULL)
    {
        *uc = &uc_data;
    }
}

void enable_pcu(off_t *msrs_pcu_pmon_evtsel, off_t *msrs_pcu_pmon_ctrs)
{
    static struct unc_perfevtsel *uevt = NULL;
    if (uevt == NULL)
    {
        unc_perfevtsel_storage(&uevt, msrs_pcu_pmon_evtsel);
    }
    write_batch(UNCORE_EVTSEL);
    clear_all_pcu(msrs_pcu_pmon_ctrs);
}

void clear_all_pcu(off_t *msrs_pcu_pmon_ctrs)
{
    static struct unc_counters *uc = NULL;
    int nsockets = 0;
    int i;

    variorum_set_topology(&nsockets, NULL, NULL);
    if (uc == NULL)
    {
        unc_counters_storage(&uc, msrs_pcu_pmon_ctrs);
    }
    for (i = 0; i < nsockets; i++)
    {
        *uc->c0[i] = 0;
        *uc->c1[i] = 0;
        *uc->c2[i] = 0;
        *uc->c3[i] = 0;
    }
    write_batch(UNCORE_COUNT);
}

void dump_unc_counter_data(FILE *writedest, off_t *msrs_pcu_pmon_evtsel, off_t *msrs_pcu_pmon_ctrs)
{
    static int init = 0;
    struct unc_counters *uc;
    int i;
    int nsockets;
    char hostname[1024];

    variorum_set_topology(&nsockets, NULL, NULL);
    gethostname(hostname, 1024);

    unc_counters_storage(&uc, msrs_pcu_pmon_ctrs);
    if (!init)
    {
        fprintf(writedest, "_UNCORE_COUNTERS | Host | Socket | c0 | c1 | c2 | c3\n");
        enable_pcu(msrs_pcu_pmon_evtsel, msrs_pcu_pmon_ctrs);
        init = 1;
    }
    for (i = 0; i < nsockets; i++)
    {
        fprintf(writedest, "_UNCORE_COUNTERS | %s | %d | %lx | %lx | %lx | %lx\n", hostname, i, *uc->c0[i], *uc->c1[i], *uc->c2[i], *uc->c3[i]);
    }
}

void print_unc_counter_data(FILE *writedest, off_t *msrs_pcu_pmon_evtsel, off_t *msrs_pcu_pmon_ctrs)
{
    struct unc_counters *uc;
    int i;
    int nsockets;
    char hostname[1024];

    variorum_set_topology(&nsockets, NULL, NULL);
    gethostname(hostname, 1024);

    unc_counters_storage(&uc, msrs_pcu_pmon_ctrs);
    enable_pcu(msrs_pcu_pmon_evtsel, msrs_pcu_pmon_ctrs);
    for (i = 0; i < nsockets; i++)
    {
        fprintf(writedest, "_UNCORE_COUNTERS Host: %s Socket: %d c0: %lx c1: %lx c2: %lx c3: %lx\n",
                hostname, i, *uc->c0[i], *uc->c1[i], *uc->c2[i], *uc->c3[i]);
    }
}

void get_all_power_data_fixed(FILE *writedest, off_t msr_pkg_power_limit, off_t msr_dram_power_limit, off_t msr_rapl_unit, off_t msr_package_energy_status, off_t msr_dram_energy_status, off_t *msrs_fixed_ctrs, off_t msr_perf_global_ctrl, off_t msr_fixed_counter_ctrl, off_t msr_aperf, off_t msr_mperf, off_t msr_tsc)
{
    // The length of the rlim array assumes dual socket system.
    static struct rapl_limit *rlim;
    //static struct rapl_limit rlim[6];
    static struct rapl_data *rapl = NULL;
    static struct fixed_counter *c0, *c1, *c2;
    static struct clocks_data *cd;
    static int init_get_power_data = 0;
    static int nsockets, nthreads;
    char hostname[1024];
    int i;
    int rlim_idx = 0;

    variorum_set_topology(&nsockets, NULL, &nthreads);
    gethostname(hostname, 1024);

    get_power(msr_rapl_unit, msr_package_energy_status, msr_dram_energy_status);

    if (!init_get_power_data)
    {
        init_get_power_data = 1;

        rlim = (struct rapl_limit *) malloc(sizeof(struct rapl_limit) * nsockets * 3);
        if (rlim == NULL)
        {
            printf("malloc of size %d failed!\n", nsockets * 3);
            exit(1);
        }

        rapl_storage(&rapl);
        fixed_counter_storage(&c0, &c1, &c2, msrs_fixed_ctrs);
        enable_fixed_counters(msrs_fixed_ctrs, msr_perf_global_ctrl, msr_fixed_counter_ctrl);
        clocks_storage(&cd, msr_aperf, msr_mperf, msr_tsc);

        fprintf(writedest, "_POWMON | time");
        for (i = 0; i < nsockets; i++)
        {
            fprintf(writedest, " | pkg%d_joules | pkg%d_lim1watts | pkg%d_lim2watts | dram%d_joules | dram%d_limwatts", i, i, i, i, i);
            get_package_rapl_limit(i, &(rlim[rlim_idx]), &(rlim[rlim_idx+1]), msr_pkg_power_limit, msr_rapl_unit);
            get_dram_rapl_limit(i, &(rlim[rlim_idx+2]), msr_dram_power_limit, msr_rapl_unit);
            rlim_idx += 3;
            // rlim[0] = first socket, power limit 1
            // rlim[1] = first socket, power limit 2
            // rlim[2] = first socket, dram power limit
            // rlim[3] = second socket, power limit 1
            // rlim[4] = second socket, power limit 2
            // rlim[5] = second socket, dram power limit

            // This code assumed dual socket system.
            //get_package_rapl_limit(0, &(rlim[0]), &(rlim[1]), msr_pkg_power_limit, msr_rapl_unit);
            //get_package_rapl_limit(1, &(rlim[2]), &(rlim[3]), msr_pkg_power_limit, msr_rapl_unit);
            //get_dram_rapl_limit(0, &(rlim[4]), msr_dram_power_limit, msr_rapl_unit);
            //get_dram_rapl_limit(1, &(rlim[5]), msr_dram_power_limit, msr_rapl_unit);
        }
        for (i = 0; i < nthreads; i++)
        {
            fprintf(writedest, " | InstRet%d | UnhaltClkCycles%d | UnhaltRefCycles%d | APERF%d | MPERF%d | TSC%d", i, i, i, i, i, i);
        }
        fprintf(writedest, "\n");
    }

    read_batch(FIXED_COUNTERS_DATA);
    read_batch(CLOCKS_DATA);
    rlim_idx = 0;
    for (i = 0; i < nsockets; i++)
    {
        get_package_rapl_limit(i, &(rlim[rlim_idx]), &(rlim[rlim_idx+1]), msr_pkg_power_limit, msr_rapl_unit);
        get_dram_rapl_limit(i, &(rlim[rlim_idx+2]), msr_dram_power_limit, msr_rapl_unit);
        rlim_idx += 3;
    }

    rlim_idx = 0;
    fprintf(writedest, "_POWMON | %ld", now_ms());
    for (i = 0; i < nsockets; i++)
    {
        fprintf(writedest, " | %lf | %lf | %lf | %lf | %lf", rapl->pkg_delta_joules[i], rlim[rlim_idx].watts, rlim[rlim_idx+1].watts, rapl->dram_delta_joules[i], rlim[rlim_idx+2].watts);
        rlim_idx += 3;
    }

    for (i = 0; i < nthreads; i++)
    {
        fprintf(writedest, " | %lu | %lu | %lu | %lu | %lu | %lu", *c0->value[i], *c1->value[i], *c2->value[i], *cd->aperf[i], *cd->mperf[i], *cd->tsc[i]);
    }
    fprintf(writedest, "\n");
}
