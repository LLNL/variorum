// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <clocks_features.h>
#include <config_architecture.h>
#include <misc_features.h>
#include <msr_core.h>
#include <variorum_cpuid.h>
#include <variorum_error.h>

void clocks_storage(struct clocks_data **cd, off_t msr_aperf, off_t msr_mperf,
                    off_t msr_tsc)
{
    static int init = 0;
    static struct clocks_data d;
    static unsigned nthreads = 0;

    if (!init)
    {
        variorum_get_topology(NULL, NULL, &nthreads);
        d.aperf = (uint64_t **) malloc(nthreads * sizeof(uint64_t *));
        d.mperf = (uint64_t **) malloc(nthreads * sizeof(uint64_t *));
        d.tsc = (uint64_t **) malloc(nthreads * sizeof(uint64_t *));
        allocate_batch(CLOCKS_DATA, 3UL * nthreads);
        load_thread_batch(msr_aperf, d.aperf, CLOCKS_DATA);
        load_thread_batch(msr_mperf, d.mperf, CLOCKS_DATA);
        load_thread_batch(msr_tsc, d.tsc, CLOCKS_DATA);
        init = 1;
    }
    if (cd != NULL)
    {
        *cd = &d;
    }
}

void perf_storage_temp(struct perf_data **pd, off_t msr_perf_ctl,
                       enum ctl_domains_e control_domains)
{
    static int init = 0;
    static struct perf_data d;
    unsigned nsockets, ncores, nthreads;

    variorum_get_topology(&nsockets, &ncores, &nthreads);

    if (!init)
    {
        init = 1;
        switch (control_domains)
        {
            case SOCKET:
                d.perf_ctl = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
                allocate_batch(PERF_CTRL, 2UL * nsockets);
                load_socket_batch(msr_perf_ctl, d.perf_ctl, PERF_CTRL);
                break;
            case CORE:
                d.perf_ctl = (uint64_t **) malloc(nthreads * sizeof(uint64_t *));
                allocate_batch(PERF_CTRL, 2UL * nthreads);
                load_thread_batch(msr_perf_ctl, d.perf_ctl, PERF_CTRL);
                break;
            default:
                break;
        }
    }
    if (pd != NULL)
    {
        *pd = &d;
    }
}

void perf_storage(struct perf_data **pd, off_t msr_perf_status)
{
    static struct perf_data d;
    static unsigned nsockets = 0;

    if (!nsockets)
    {
        variorum_get_topology(&nsockets, NULL, NULL);
        d.perf_status = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(PERF_DATA, 2UL * nsockets);
        load_socket_batch(msr_perf_status, d.perf_status, PERF_DATA);
        //d.perf_ctl = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        //allocate_batch(PERF_CTL, 2UL * nsockets());
        //load_socket_batch(IA32_PERF_CTL, d.perf_ctl, PERF_CTL);
    }
    if (pd != NULL)
    {
        *pd = &d;
    }
}

int dump_clocks_data(FILE *writedest, off_t msr_aperf, off_t msr_mperf,
                     off_t msr_tsc, off_t msr_perf_status, off_t msr_platform_info,
                     enum ctl_domains_e control_domains)
{
    static struct clocks_data *cd;
    static struct perf_data *pd;
    static int init = 0;
    unsigned i, j, k;
    unsigned nsockets, ncores, nthreads;
    int idx;
    char hostname[1024];
    int max_non_turbo_ratio;
    int err;

    err = get_max_non_turbo_ratio(msr_platform_info, &max_non_turbo_ratio);
    if (err)
    {
        variorum_error_handler("Error retrieving max non-turbo ratio",
                               VARIORUM_ERROR_FUNCTION, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    variorum_get_topology(&nsockets, &ncores, &nthreads);
    gethostname(hostname, 1024);
    if (!init)
    {
        clocks_storage(&cd, msr_aperf, msr_mperf, msr_tsc);
        perf_storage(&pd, msr_perf_status);
        if (control_domains == SOCKET)
        {
            fprintf(writedest,
                    "_CLOCKS_DATA hostname socket aperf mperf tsc curr_freq_mhz avg_freq_mhz\n");
        }
        else if (control_domains == CORE)
        {
            fprintf(writedest,
                    "_CLOCKS_DATA hostname socket core thread_phy thread_log aperf mperf tsc curr_freq_mhz avg_freq_mhz\n");
        }
        init = 1;
    }
    read_batch(CLOCKS_DATA);
    read_batch(PERF_DATA);

    switch (control_domains)
    {
        case SOCKET:
            for (i = 0; i < nsockets; i++)
            {
                for (j = 0; j < ncores / nsockets; j += ncores / nsockets)
                {
                    for (k = 0; k < nthreads / ncores; k += nthreads / ncores)
                    {
                        idx = (k * nsockets * (ncores / nsockets)) + (i * (ncores / nsockets)) + j;
                        fprintf(writedest, "_CLOCKS_DATA %s %d %lu %lu %lu %lu %f\n",
                                hostname, i, *cd->aperf[idx], *cd->mperf[idx], *cd->tsc[idx],
                                MASK_VAL(*pd->perf_status[i], 15, 8) * 100,
                                max_non_turbo_ratio * ((*cd->aperf[idx]) / (double)(*cd->mperf[idx])));
                    }
                }
            }
            break;
        case CORE:
            for (i = 0; i < nsockets; i++)
            {
                for (j = 0; j < ncores / nsockets; j++)
                {
                    for (k = 0; k < nthreads / ncores; k++)
                    {
                        idx = (k * nsockets * (ncores / nsockets)) + (i * (ncores / nsockets)) + j;
                        fprintf(writedest, "_CLOCKS_DATA %s %d %d %d %d %lu %lu %lu %lu %f\n",
                                hostname, i, j, k, idx, *cd->aperf[idx], *cd->mperf[idx], *cd->tsc[idx],
                                MASK_VAL(*pd->perf_status[i], 15, 8) * 100,
                                max_non_turbo_ratio * ((*cd->aperf[idx]) / (double)(*cd->mperf[idx])));
                    }
                }
            }
            break;
        default:
            fprintf(stderr, "Not a valid control domain.\n");
            break;
    }
    return 0;
}

int print_clocks_data(FILE *writedest, off_t msr_aperf, off_t msr_mperf,
                      off_t msr_tsc, off_t msr_perf_status, off_t msr_platform_info,
                      enum ctl_domains_e control_domains)
{
    static struct clocks_data *cd;
    static struct perf_data *pd;
    unsigned i, j, k;
    int idx;
    unsigned nsockets, ncores, nthreads;
    char hostname[1024];
    int max_non_turbo_ratio;
    int err;

    err = get_max_non_turbo_ratio(msr_platform_info, &max_non_turbo_ratio);
    if (err)
    {
        variorum_error_handler("Error retrieving max non-turbo ratio",
                               VARIORUM_ERROR_FUNCTION, getenv("HOSTNAME"),
                               __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    variorum_get_topology(&nsockets, &ncores, &nthreads);
    gethostname(hostname, 1024);

    clocks_storage(&cd, msr_aperf, msr_mperf, msr_tsc);
    perf_storage(&pd, msr_perf_status);
    read_batch(CLOCKS_DATA);
    read_batch(PERF_DATA);

    switch (control_domains)
    {
        case SOCKET:
            for (i = 0; i < nsockets; i++)
            {
                for (j = 0; j < ncores / nsockets; j += ncores / nsockets)
                {
                    for (k = 0; k < nthreads / ncores; k += nthreads / ncores)
                    {
                        idx = (k * nsockets * (ncores / nsockets)) + (i * (ncores / nsockets)) + j;
                        fprintf(writedest,
                                "_CLOCKS_DATA Host: %s, Socket: %d, APERF: %lu, MPERF: %lu, TSC: %lu, Curr_Freq: %lu MHz, Avg_Freq: %f MHz\n",
                                hostname, i, *cd->aperf[idx], *cd->mperf[idx], *cd->tsc[idx],
                                MASK_VAL(*pd->perf_status[i], 15, 8) * 100,
                                max_non_turbo_ratio * ((*cd->aperf[idx]) / (double)(*cd->mperf[idx])));
                    }
                }
            }
            break;
        case CORE:
            for (i = 0; i < nsockets; i++)
            {
                for (j = 0; j < ncores / nsockets; j++)
                {
                    for (k = 0; k < nthreads / ncores; k++)
                    {
                        idx = (k * nsockets * (ncores / nsockets)) + (i * (ncores / nsockets)) + j;
                        fprintf(writedest,
                                "_CLOCKS_DATA Host: %s, Socket: %d, Core: %d, Thread_Phy: %d, Thread_Log: %d, APERF: %lu, MPERF: %lu, TSC: %lu, Curr_Freq: %lu MHz, Avg_Freq: %f MHz\n",
                                hostname, i, j, k, idx, *cd->aperf[idx], *cd->mperf[idx], *cd->tsc[idx],
                                MASK_VAL(*pd->perf_status[i], 15, 8) * 100,
                                max_non_turbo_ratio * ((*cd->aperf[idx]) / (double)(*cd->mperf[idx])));
                    }
                }
            }
            break;
        default:
            fprintf(stderr, "Not a valid control domain.\n");
            break;
    }
    return 0;
}

//void print_clocks_data_socket(FILE *writedest, off_t msr_aperf, off_t msr_mperf, off_t msr_tsc, off_t msr_perf_status, off_t msr_platform_info)
//{
//    static struct clocks_data *cd;
//    static struct perf_data *pd;
//    int i, j, k;
//    int idx;
//    int nsockets, ncores, nthreads;
//    char hostname[1024];
//    int max_non_turbo_ratio = get_max_non_turbo_ratio(msr_platform_info);
//
//    variorum_get_topology(&nsockets, &ncores, &nthreads);
//    gethostname(hostname, 1024);
//
//    clocks_storage(&cd, msr_aperf, msr_mperf, msr_tsc);
//    perf_storage(&pd, msr_perf_status);
//    read_batch(CLOCKS_DATA);
//    read_batch(PERF_DATA);
//    for (i = 0; i < nsockets; i++)
//    {
//        for (j = 0; j < ncores/nsockets; j+=ncores/nsockets)
//        {
//            for (k = 0; k < nthreads/ncores; k+=nthreads/ncores)
//            {
//                idx = (k * nsockets * (ncores/nsockets)) + (i * (ncores/nsockets)) + j;
//                fprintf(writedest, "_CLOCKS_DATA Host: %s Socket: %d APERF: %lu MPERF: %lu TSC: %lu Curr_Freq_MHz: %lu Avg_Freq_MHz: %f\n",
//                        hostname, i, *cd->aperf[idx], *cd->mperf[idx], *cd->tsc[idx], MASK_VAL(*pd->perf_status[i], 15, 8) * 100, max_non_turbo_ratio*((*cd->aperf[idx])/(double)(*cd->mperf[idx])));
//            }
//        }
//    }
//}
//
//void print_clocks_data_core(FILE *writedest, off_t msr_aperf, off_t msr_mperf, off_t msr_tsc, off_t msr_perf_status, off_t msr_platform_info)
//{
//    static struct clocks_data *cd;
//    static struct perf_data *pd;
//    int i, j, k;
//    int idx;
//    int nsockets, ncores, nthreads;
//    char hostname[1024];
//    int max_non_turbo_ratio = get_max_non_turbo_ratio(msr_platform_info);
//
//    variorum_get_topology(&nsockets, &ncores, &nthreads);
//    gethostname(hostname, 1024);
//
//    clocks_storage(&cd, msr_aperf, msr_mperf, msr_tsc);
//    perf_storage(&pd, msr_perf_status);
//    read_batch(CLOCKS_DATA);
//    read_batch(PERF_DATA);
//    for (i = 0; i < nsockets; i++)
//    {
//        for (j = 0; j < ncores/nsockets; j++)
//        {
//            for (k = 0; k < nthreads/ncores; k++)
//            {
//                idx = (k * nsockets * (ncores/nsockets)) + (i * (ncores/nsockets)) + j;
//                fprintf(writedest, "_CLOCKS_DATA Host: %s Socket: %d Core: %d Thread_Phy: %d Thread_Log: %d APERF: %lu MPERF: %lu TSC: %lu Curr_Freq_Mhz: %lu Avg_Freq_Mhz: %f\n",
//                        hostname, i, j, k, idx, *cd->aperf[idx], *cd->mperf[idx], *cd->tsc[idx], MASK_VAL(*pd->perf_status[i], 15, 8) * 100, max_non_turbo_ratio*((*cd->aperf[idx])/(double)(*cd->mperf[idx])));
//            }
//        }
//    }
//}

void set_p_state(int cpu_freq_mhz, enum ctl_domains_e domain,
                 off_t msr_perf_status)
{
    unsigned nsockets, ncores, nthreads;
    unsigned i;
    static struct perf_data *pd;
    static int init = 0;

    variorum_get_topology(&nsockets, &ncores, &nthreads);
    if (!init)
    {
        init = 1;
        perf_storage_temp(&pd, msr_perf_status, domain);
    }

    switch (domain)
    {
        case SOCKET:
            printf("Set frequencies per socket\n");
            for (i = 0; i < nsockets; i++)
            {
                *pd->perf_ctl[i] = cpu_freq_mhz / 100 * 256;
            }
            write_batch(PERF_CTRL);
            break;
        case CORE:
            printf("Set frequencies per core\n");
            for (i = 0; i < nthreads; i++)
            {
                *pd->perf_ctl[i] = cpu_freq_mhz / 100 * 256;
            }
#if VARIORUM_DEBUG
            printf("PERF_CTL raw decimal %" PRIu64 "\n", *pd->perf_ctl[9]);
#endif
            write_batch(PERF_CTRL);
            read_batch(PERF_CTRL);
#if VARIORUM_DEBUG
            printf("---reading PERF_CTL raw decimal %" PRIu64 "\n", *pd->perf_ctl[9]);
#endif
            break;
        default:
            fprintf(stderr, "Not a valid control domain.\n");
            break;
    }
}

//void set_p_state(unsigned socket, uint64_t pstate)
//{
//    static uint64_t procs = 0;
//    static struct perf_data *cd;
//
//    if (!procs)
//    {
//        procs = num_sockets();
//        perf_storage(&cd);
//    }
//    *cd->perf_ctl[socket] = pstate;
//#ifdef LIBMSR_DEBUG
//    printf("PERF_CTL raw decimal %" PRIu64 "\n", *cd->perf_ctl[socket]);
//#endif
//    write_batch(PERF_CTL);
//
//#ifdef LIBMSR_DEBUG
//    read_batch(PERF_CTL);
//    printf("---reading PERF_CTL raw decimal %" PRIu64 "\n", *cd->perf_ctl[socket]);
//#endif
//}

//void dump_clock_mod(struct clock_mod *s, FILE *writedest)
//{
//    double percent = 0.0;
//
//    if (s->duty_cycle == 0)
//    {
//        percent = 6.25;
//    }
//    else if (s->duty_cycle == 1)
//    {
//        percent = 12.5;
//    }
//    else if (s->duty_cycle == 2)
//    {
//        percent = 25.0;
//    }
//    else if (s->duty_cycle == 3)
//    {
//        percent = 37.5;
//    }
//    else if (s->duty_cycle == 4)
//    {
//        percent = 50.0;
//    }
//    else if (s->duty_cycle == 5)
//    {
//        percent = 63.5;
//    }
//    else if (s->duty_cycle == 6)
//    {
//        percent = 75.0;
//    }
//    else if (s->duty_cycle == 7)
//    {
//        percent = 87.5;
//    }
//    fprintf(writedest, "duty_cycle        = %d\n", s->duty_cycle);
//    fprintf(writedest, "percentage        = %.2f\n", percent);
//    fprintf(writedest, "duty_cycle_enable = %d\n", s->duty_cycle_enable);
//}
//
//void get_clock_mod(int socket, int core, struct clock_mod *s)
//{
//    read_msr_by_coord(socket, core, 0, IA32_CLOCK_MODULATION, &(s->raw));
//    /* Specific encoded values for target duty cycle. */
//    s->duty_cycle = MASK_VAL(s->raw, 3, 1);
//
//    /* On-Demand Clock Modulation Enable */
//    /* 1 = enabled, 0 disabled */
//    s->duty_cycle_enable = MASK_VAL(s->raw, 4, 4);
//}
//
//int set_clock_mod(int socket, int core, struct clock_mod *s)
//{
//    uint64_t msrVal;
//
//    read_msr_by_coord(socket, core, 0, IA32_CLOCK_MODULATION, &msrVal);
//    if (!(s->duty_cycle > 0 && s->duty_cycle < 8))
//    {
//        return -1;
//    }
//    if (!(s->duty_cycle_enable == 0 || s->duty_cycle_enable == 1))
//    {
//        return -1;
//    }
//
//    msrVal = (msrVal & (~(3<<1))) | (s->duty_cycle << 1);
//    msrVal = (msrVal & (~(1<<4))) | (s->duty_cycle_enable << 4);
//
//    write_msr_by_coord(socket, core, 0, IA32_CLOCK_MODULATION, msrVal);
//    return 0;
//}

void get_available_frequencies_skx(FILE *writedest, off_t *msr_platform_info,
                                   off_t *msr_turbo_ratio_limit, off_t *msr_turbo_ratio_limit_cores,
                                   off_t *msr_config_tdp_l1, off_t *msr_config_tdp_l2)
{
    /* Turbo Range
     * Default ratio for 1C Max Turbo == P01
     * All core turbo == P0n
     * MSR_TURBO_RATIO_LIMIT_CORES for Skylake (1AEh)
     */
    fprintf(writedest, "=== Turbo Schedule ===\n");
    if (get_turbo_ratio_limits_skx(*msr_turbo_ratio_limit,
                                   *msr_turbo_ratio_limit_cores) != 0)
    {
        variorum_error_handler("Values do not match across sockets",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
    }

    fprintf(writedest, "\n");

    /* AVX2, AVX512 (i.e., AVX3) */
    fprintf(writedest, "=== AVX Schedule ===\n");
    get_avx_limits(msr_platform_info, msr_config_tdp_l1, msr_config_tdp_l2);

    fprintf(writedest, "\n");

    /* P-State Table -- P1, Pn, and Pm
     * Read IA32_PLATFORM_INFO 0xCE
     * Field "Maximum Efficiency Ratio: Bits 47:40 == Pn
     * Field "Maximum Non-Turbo Ratio: Bits 15:8 == P1
     * Field "Minimum Operating Ratio: Bits 55:48 == Pm
     */
    fprintf(writedest, "=== P-State Table ===\n");
    int max_non_turbo_ratio;
    int max_eff_ratio;
    int min_oper_ratio;

    int err = get_max_non_turbo_ratio(*msr_platform_info, &max_non_turbo_ratio);
    if (!err)
    {
        fprintf(writedest, "Max Efficiency Ratio = %d MHz\n",
                max_non_turbo_ratio);
    }
    err = get_max_efficiency_ratio(*msr_platform_info, &max_eff_ratio);
    if (!err)
    {
        fprintf(writedest, "Max Non-Turbo Ratio  = %d MHz\n",
                max_eff_ratio);
    }
    err = get_min_operating_ratio(*msr_platform_info, &min_oper_ratio);
    if (!err)
    {
        fprintf(writedest, "Min Operating Ratio  = %d MHz\n",
                min_oper_ratio);
    }
}

void get_available_frequencies(FILE *writedest, off_t *msr_platform_info,
                               off_t *msr_turbo_ratio_limit, off_t *msr_turbo_ratio_limit1,
                               off_t *msr_config_tdp_l1, off_t *msr_config_tdp_l2)
{
    /* Turbo Range
     * Default ratio for 1C Max Turbo == P01
     * All core turbo == P0n
     * MSR_TURBO_RATIO_LIMIT_CORES for Skylake (1AEh)
     */
    fprintf(writedest, "=== Turbo Schedule ===\n");
    if (get_turbo_ratio_limits(*msr_turbo_ratio_limit,
                               *msr_turbo_ratio_limit1) != 0)
    {
        variorum_error_handler("Values do not match across sockets",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
    }

    fprintf(writedest, "\n");

    /* AVX2, AVX512 (i.e., AVX3) */
    fprintf(writedest, "=== AVX Schedule ===\n");
    get_avx_limits(msr_platform_info, msr_config_tdp_l1, msr_config_tdp_l2);

    fprintf(writedest, "\n");

    /* P-State Table -- P1, Pn, and Pm
     * Read IA32_PLATFORM_INFO 0xCE
     * Field "Maximum Efficiency Ratio: Bits 47:40 == Pn
     * Field "Maximum Non-Turbo Ratio: Bits 15:8 == P1
     * Field "Minimum Operating Ratio: Bits 55:48 == Pm
     */
    fprintf(writedest, "=== P-State Table ===\n");
    int max_non_turbo_ratio;
    int max_eff_ratio;
    int min_oper_ratio;

    int err = get_max_non_turbo_ratio(*msr_platform_info, &max_non_turbo_ratio);
    if (!err)
    {
        fprintf(writedest, "Max Efficiency Ratio = %d MHz\n",
                max_non_turbo_ratio);
    }
    err = get_max_efficiency_ratio(*msr_platform_info, &max_eff_ratio);
    if (!err)
    {
        fprintf(writedest, "Max Non-Turbo Ratio  = %d MHz\n",
                max_eff_ratio);
    }
    err = get_min_operating_ratio(*msr_platform_info, &min_oper_ratio);
    if (!err)
    {
        fprintf(writedest, "Min Operating Ratio  = %d MHz\n",
                min_oper_ratio);
    }
}
