// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <power_features.h>
#include <config_architecture.h>
#include <msr_core.h>
#include <variorum_error.h>
#include <variorum_timers.h>

static int translate(const unsigned socket, uint64_t *bits, double *units, int type, off_t msr)
{
    static int init_translate = 0;
    double logremainder = 0.0;
    static struct rapl_units *ru = NULL;
    uint64_t timeval_z = 0;
    uint64_t timeval_y = 0;
    static int nsockets;

#ifdef VARIORUM_DEBUG
    fprintf(stderr, "DEBUG: (translate) bits are at %p\n", bits);
#endif
    if (!init_translate)
    {
        init_translate = 1;
        variorum_set_topology(&nsockets, NULL, NULL);
        ru = (struct rapl_units *) malloc(nsockets * sizeof(struct rapl_units));
        get_rapl_power_unit(ru, msr);
    }

    switch(type)
    {
        case BITS_TO_WATTS:
            *units = (double)(*bits) * ru[socket].watts;
            break;
        case BITS_TO_JOULES_DRAM:
            /* If Haswell (06_3F) or Broadwell (06_4F), use 15.3 micro-Joules as the energy unit for DRAM. */
            if (*g_platform.intel_arch == 63 || *g_platform.intel_arch == 79)
            {
                *units = (double)(*bits) / STD_ENERGY_UNIT;
#ifdef VARIORUM_DEBUG
                fprintf(stderr, "DEBUG: (translate_dram) %f is %f joules\n", (double)*bits, *units);
#endif
                return 0;
            }
        /* No break statement, if not Haswell or Broadwell, use energy units defined in MSR_RAPL_POWER_UNIT. */
        case BITS_TO_JOULES:
            *units = (double)(*bits) / ru[socket].joules;
            break;
        case WATTS_TO_BITS:
            *bits  = (uint64_t)((*units) / ru[socket].watts);
            break;
        case JOULES_TO_BITS:
            /// @todo Currently unused, but if it ever is used, we need a fix for Haswell.
            *bits  = (uint64_t)((*units) * ru[socket].joules);
            break;
        case BITS_TO_SECONDS_STD:
            timeval_y = *bits & 0x1F;
            timeval_z = (*bits & 0x60) >> 5;
            /* Dividing by time unit because it's stored as (1/(2^TU))^-1. */
            *units = ((1 + 0.25 * timeval_z) * pow(2.0,(double)timeval_y)) / ru[socket].seconds;
            // Temporary fix for haswell
            //    if (model == 0x3F)
            //    {
            //        *units = *units * 2.5 + 15.0;
            //    }
#ifdef LIBMSR_DEBUG
            fprintf(stderr, "%s %s::%d DEBUG: timeval_z is %lx, timeval_y is %lx, units is %lf, bits is %lx\n", getenv("HOSTNAME"), __FILE__, __LINE__, timeval_z, timeval_y, *units, *bits);
#endif
            break;
        case SECONDS_TO_BITS_STD:
            // Temporary fix for haswell
            //    if (model == 0x3F)
            //    {
            //        *units = *units / 2.5 - 15;
            //    }
            /* Store the whole number part of the log2. */
            timeval_y = (uint64_t)log2(*units * ru[socket].seconds);
            /* Store the mantissa of the log2. */
            logremainder = (double)log2(*units * ru[socket].seconds) - (double)timeval_y;
            timeval_z = 0;
            /* Based on the mantissa, choose the appropriate multiplier. */
            if (logremainder > 0.15 && logremainder <= 0.45)
            {
                timeval_z = 1;
            }
            else if (logremainder > 0.45 && logremainder <= 0.7)
            {
                timeval_z = 2;
            }
            else if (logremainder > 0.7)
            {
                timeval_z = 3;
            }
            /* Store the bits in the Intel specified format. */
            *bits = (uint64_t)(timeval_y | (timeval_z << 5));
#ifdef LIBMSR_DEBUG
            fprintf(stderr, "%s %s::%d DEBUG: timeval_z is %lx, timeval_y is %lx, units is %lf, bits is %lx, remainder is %lf\n", getenv("HOSTNAME"), __FILE__, __LINE__, timeval_z, timeval_y, *units, *bits, logremainder);
#endif
            break;
        default:
            fprintf(stderr, "%s:%d  Unknown value %d.  This is bad.\n", __FILE__, __LINE__, type);
            *bits = -1;
            *units= -1.0;
            break;
    }
    return 0;
}

static int calc_rapl_bits(const unsigned socket, struct rapl_limit *limit, const unsigned offset, off_t msr)
{
    uint64_t watts_bits = 0;
    uint64_t seconds_bits = 0;

    watts_bits = MASK_VAL(limit->bits, 14+offset, 0+offset);
    seconds_bits = MASK_VAL(limit->bits, 23+offset, 17+offset);

#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d DEBUG: (calc_rapl_bits)\n", getenv("HOSTNAME"), __FILE__, __LINE__);
#endif
    /*
     * We have been given watts and seconds and need to translate these into
     * bit values.
     * If offset is >= 32 (we are setting the 2nd pkg limit), we don't need time
     * conversion.
     */
    translate(socket, &seconds_bits, &limit->seconds, SECONDS_TO_BITS_STD, msr);
    if (offset >= 32)
    {
        seconds_bits = (uint64_t)limit->seconds; // unit is milliseconds
        //translate(socket, &seconds_bits, &limit->seconds, SECONDS_TO_BITS_STD);
    }
    else
    {
        translate(socket, &seconds_bits, &limit->seconds, SECONDS_TO_BITS_STD, msr);
    }
    /* There is only 1 translation for watts (so far). */
    translate(socket, &watts_bits, &limit->watts, WATTS_TO_BITS, msr);
#ifdef VARIORUM_DEBUG
    fprintf(stderr, "Converted %lf watts into %lx bits.\n", limit->watts, watts_bits);
    fprintf(stderr, "Converted %lf seconds into %lx bits.\n", limit->seconds, seconds_bits);
#endif
    /* Check to make sure the specified watts is not larger than the allowed
     * (15 bits). */
    if ((double)watts_bits > (pow(2,15))-1)
    {
        variorum_error_handler("Power limit is too large", VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    watts_bits <<= 0 + offset;
    // @todo Bounds check for time window still needs work.
    ///* Check to make sure the specified time window is not larger
    // * than allowed (7 bits). */
    //if ((double)seconds_bits > (pow(2,7)-1))
    //{
    //    libmsr_error_handler("calc_rapl_bits(): Time window value is too large", LIBMSR_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __LINE__);
    //    return -1;
    //}

    seconds_bits <<= 17 + offset;
    limit->bits |= watts_bits;
    limit->bits |= seconds_bits;
#ifdef VARIORUM_DEBUG
    fprintf(stderr, "calculated rapl bits\n");
#endif
    return 0;
}

static int calc_rapl_from_bits(const unsigned socket, struct rapl_limit *limit, const unsigned offset, off_t msr)
{
    uint64_t watts_bits = 0;
    uint64_t seconds_bits = 0;
    int ret = 0;

    sockets_assert(&socket, __LINE__, __FILE__);

#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d DEBUG: (calc_rapl_from_bits)\n", getenv("HOSTNAME"), __FILE__, __LINE__);
#endif
    watts_bits = MASK_VAL(limit->bits, 14+offset, 0+offset);
    seconds_bits = MASK_VAL(limit->bits, 23+offset, 17+offset);

    // We have been given the bits to be written to the msr.
    // For sake of completeness, translate these into watts and seconds.
    ret = translate(socket, &watts_bits, &limit->watts, BITS_TO_WATTS, msr);
    // If the offset is > 31 (we are writing the upper PKG limit), then no
    // translation needed
    ret += translate(socket, &seconds_bits, &limit->seconds, BITS_TO_SECONDS_STD, msr);
//    if (offset < 32)
//    {
//        ret += translate(socket, &seconds_bits, &limit->seconds, BITS_TO_SECONDS_STD, msr);
//    }
//    else
//    {
//        limit->seconds = seconds_bits;
//    }
    if (ret < 0)
    {
        variorum_error_handler("Translation from bits to values failed", VARIORUM_ERROR_RAPL_INIT, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return ret;
    }
    return 0;
}

static int calc_package_rapl_limit(const unsigned socket, struct rapl_limit *limit1, struct rapl_limit *limit2, off_t msr)
{
    /* If we have been given a lower rapl limit. */
    if (limit1 != NULL)
    {
        if (limit1->translate_bits == 1)
        {
            if (calc_rapl_from_bits(socket, limit1, 0, msr))
            {
                return -1;
            }
        }
        else if (limit1->translate_bits == 0)
        {
            if (calc_rapl_bits(socket, limit1, 0, msr))
            {
                return -1;
            }
        }
    }
    /* If we have been given an upper rapl limit. */
    if (limit2 != NULL)
    {
        if (limit2->translate_bits == 1)
        {
            if (calc_rapl_from_bits(socket, limit2, 32, msr))
            {
                return -1;
            }
        }
        else if (limit2->translate_bits == 0)
        {
            if (calc_rapl_bits(socket, limit2, 32, msr))
            {
                return -1;
            }
        }
    }
#ifdef VARIORUM_DEBUG
    fprintf(stderr, "pkg calculated\n");
#endif
    return 0;
}

static int calc_dram_rapl_limit(const unsigned socket, struct rapl_limit *limit, off_t msr)
{
    if (limit != NULL)
    {
        if (limit->translate_bits == 1)
        {
            if (calc_rapl_from_bits(socket, limit, 0, msr))
            {
                return -1;
            }
        }
        else if (limit->translate_bits == 0)
        {
            if (calc_rapl_bits(socket, limit, 0, msr))
            {
                return -1;
            }
        }
    }

    return 0;
}

static void create_rapl_data_batch(struct rapl_data *rapl, off_t msr_pkg_energy_status, off_t msr_dram_energy_status)
{
    int nsockets;
    variorum_set_topology(&nsockets, NULL, NULL);

    allocate_batch(RAPL_DATA, 2UL * nsockets);

    rapl->pkg_bits = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
    rapl->pkg_joules = (double *) calloc(nsockets, sizeof(double));
    rapl->old_pkg_bits = (uint64_t *) calloc(nsockets, sizeof(uint64_t));
    rapl->old_pkg_joules = (double *) calloc(nsockets, sizeof(double));
    rapl->pkg_delta_joules = (double *) calloc(nsockets, sizeof(double));
    rapl->pkg_delta_bits = (uint64_t *) calloc(nsockets, sizeof(double));
    rapl->pkg_watts = (double *) calloc(nsockets, sizeof(double));
    load_socket_batch(msr_pkg_energy_status, rapl->pkg_bits, RAPL_DATA);

    rapl->dram_bits = (uint64_t **) calloc(nsockets, sizeof(uint64_t *));
    rapl->old_dram_bits = (uint64_t *) calloc(nsockets, sizeof(uint64_t));
    rapl->dram_joules = (double *) calloc(nsockets, sizeof(double));
    rapl->old_dram_joules = (double *) calloc(nsockets, sizeof(double));
    rapl->dram_delta_joules = (double *) calloc(nsockets, sizeof(double));
    rapl->dram_watts = (double *) calloc(nsockets, sizeof(double));
    load_socket_batch(msr_dram_energy_status, rapl->dram_bits, RAPL_DATA);

    //if (*rapl_flags & PKG_PERF_STATUS)
    //{
    //    rapl->pkg_perf_count = (uint64_t **) libmsr_calloc(sockets * threadsPerCore, sizeof(uint64_t));
    //    load_socket_batch(MSR_PKG_PERF_STATUS, rapl->pkg_perf_count, RAPL_DATA);
    //}
    //if (*rapl_flags & DRAM_PERF_STATUS)
    //{
    //    rapl->dram_perf_count = (uint64_t **) libmsr_calloc(sockets * threadsPerCore, sizeof(uint64_t));
    //    load_socket_batch(MSR_DRAM_PERF_STATUS, rapl->dram_perf_count, RAPL_DATA);
    //}
}

int get_rapl_power_unit(struct rapl_units *ru, off_t msr)
{
    static int init_get_rapl_power_unit = 0;
    static uint64_t **val = NULL;
    static int nsockets, ncores, nthreads;
    int i;

    variorum_set_topology(&nsockets, &ncores, &nthreads);
    if (!init_get_rapl_power_unit)
    {
        init_get_rapl_power_unit = 1;
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(RAPL_UNIT, nsockets);
        load_socket_batch(msr, val, RAPL_UNIT);
    }
    read_batch(RAPL_UNIT);

    /* Initialize the units used for each socket. */
    for (i = 0; i < nsockets; i++)
    {
        // See figure 14-16 for bit fields.
        //  1  1 1  1 1
        //  9  6 5  2 1  8 7  4 3  0
        //
        //  1010 0001 0000 0000 0011
        //
        //     A    1    0    0    3
        //ru[i].msr_rapl_power_unit = 0xA1003;

        ru[i].msr_rapl_power_unit = *val[i];
        /* Default is 1010b or 976 microseconds. */
        /* Storing (1/(2^TU))^-1 for maximum precision. */
        ru[i].seconds = (double)(1 << (MASK_VAL(ru[i].msr_rapl_power_unit, 19, 16)));
        /* Default is 10000b or 15.3 microjoules. */
        /* Storing (1/(2^ESU))^-1 for maximum precision. */
        ru[i].joules = (double)(1 << (MASK_VAL(ru[i].msr_rapl_power_unit, 12, 8)));
        /* Default is 0011b or 1/8 Watts. */
        ru[i].watts = ((1.0)/((double)(1 << (MASK_VAL(ru[i].msr_rapl_power_unit, 3, 0)))));
#ifdef VARIORUM_DEBUG
        fprintf(stdout, "Pkg %d MSR_RAPL_POWER_UNIT\n", i);
        fprintf(stdout, "Raw: %f sec, %f J, %f watts\n", ru[i].seconds, ru[i].joules, ru[i].watts);
        fprintf(stdout, "Adjusted: %f sec, %f J, %f watts\n", 1/ru[i].seconds, 1/ru[i].joules, ru[i].watts);
#endif
    }

//    /* Check consistency between packages. */
//    uint64_t *tmp = (uint64_t *) libmsr_calloc(sockets, sizeof(uint64_t));
//    for (i = 0; i < sockets; i++)
//    {
//        read_msr_by_coord(i, 0, 0, MSR_RAPL_POWER_UNIT, tmp);
//        double energy = (double)(1 << (MASK_VAL(ru[i].msr_rapl_power_unit, 12, 8)));
//        double seconds = (double)(1 << (MASK_VAL(ru[i].msr_rapl_power_unit, 19, 16)));
//        double power = ((1.0)/((double)(1 << (MASK_VAL(ru[i].msr_rapl_power_unit, 3, 0)))));
//        if (energy != ru[i].joules || power != ru[i].watts || seconds != ru[i].seconds)
//        {
//            variorum_error_handler("Inconsistent rapl power units across packages", VARIORUM_ERROR_RUNTIME, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
//        }
//    }
    return 0;
}

void dump_rapl_power_unit(FILE *writedest, off_t msr)
{
    int socket;
    struct rapl_units *ru;
    char hostname[1024];
    int nsockets;

    variorum_set_topology(&nsockets, NULL, NULL);
    gethostname(hostname, 1024);

    ru = (struct rapl_units *) malloc(nsockets * sizeof(struct rapl_units));
    get_rapl_power_unit(ru, msr);

    fprintf(writedest, "_RAPL_POWER_UNITS | Offset | Host | Socket | Bits | WattsUnit | JoulesUnit | SecondsUnit\n");
    for (socket = 0; socket < nsockets; socket++)
    {
        fprintf(writedest, "_RAPL_POWER_UNITS | 0x%lx | %s | %d | 0x%lx | %f | %f | %f\n", msr, hostname, socket, ru[socket].msr_rapl_power_unit, ru[socket].watts, 1/ru[socket].joules, 1/ru[socket].seconds);
    }
    free(ru);
}

void print_rapl_power_unit(FILE *writedest, off_t msr)
{
    int socket;
    struct rapl_units *ru;
    char hostname[1024];
    int nsockets;

    variorum_set_topology(&nsockets, NULL, NULL);
    gethostname(hostname, 1024);

    ru = (struct rapl_units *) malloc(nsockets * sizeof(struct rapl_units));
    get_rapl_power_unit(ru, msr);

    for (socket = 0; socket < nsockets; socket++)
    {
        fprintf(writedest, "_RAPL_POWER_UNITS Offset: 0x%lx Host: %s Socket: %d Bits: 0x%lx PowerUnit: %fW EnergyUnit: %fJ TimeUnit: %f\n", msr, hostname, socket, ru[socket].msr_rapl_power_unit, ru[socket].watts, 1/ru[socket].joules, 1/ru[socket].seconds);
    }
    free(ru);
}

void dump_package_power_info(FILE *writedest, off_t msr, int socket)
{
    struct rapl_power_info info;
    char hostname[1024];
    static int init_dump_package_power_info = 0;

    gethostname(hostname, 1024);

    if (!init_dump_package_power_info)
    {
        init_dump_package_power_info = 1;
        fprintf(writedest, "_PACKAGE_POWER_INFO | Offset | Host | Socket | Bits | MaxPower | MinPower | MaxTimeWindow | ThermPower\n");
    }

    if (!get_rapl_power_info(socket, &info, msr))
    {
        fprintf(writedest, "_PACKAGE_POWER_INFO | 0x%lx | %s | %d | 0x%lx | %lf | %lf | %lf | %lf\n", msr, hostname, socket, info.msr_pkg_power_info, info.pkg_max_power, info.pkg_min_power, info.pkg_max_window, info.pkg_therm_power);
    }
}

void print_package_power_info(FILE *writedest, off_t msr, int socket)
{
    struct rapl_power_info info;
    char hostname[1024];

    gethostname(hostname, 1024);

    if (!get_rapl_power_info(socket, &info, msr))
    {
        fprintf(writedest, "_PACKAGE_POWER_INFO Offset: 0x%lx Host: %s Socket: %d Bits: 0x%lx MaxPower: %lfW MinPower: %lfW MaxWindow: %lf sec ThermPower: %lfW\n", msr, hostname, socket, info.msr_pkg_power_info, info.pkg_max_power, info.pkg_min_power, info.pkg_max_window, info.pkg_therm_power);
    }
}

void dump_package_power_limit(FILE *writedest, off_t msr_power_limit, off_t msr_rapl_unit, int socket)
{
    struct rapl_limit l1, l2;
    static int init_dump_package_power_limit = 0;
    char hostname[1024];
    int nsockets;

    variorum_set_topology(&nsockets, NULL, NULL);
    gethostname(hostname, 1024);

    if (!init_dump_package_power_limit)
    {
        init_dump_package_power_limit = 1;
        fprintf(writedest, "_PACKAGE_POWER_LIMITS | Offset | Host | Socket | PowerLimBits | Watts1 | Seconds1 | Watts2 | Seconds2\n");
    }

    if (!get_package_rapl_limit(socket, &l1, &l2, msr_power_limit, msr_rapl_unit))
    {
        fprintf(writedest, "_PACKAGE_POWER_LIMITS | 0x%lx | %s | %d | 0x%lx | %lf | %lf | %lf | %lf\n", msr_power_limit, hostname, socket, l1.bits, l1.watts, l1.seconds, l2.watts, l2.seconds);
    }
}

void dump_dram_power_limit(FILE *writedest, off_t msr_power_limit, off_t msr_rapl_unit, int socket)
{
    struct rapl_limit l1;
    static int init_dump_dram_power_limit = 0;
    char hostname[1024];
    int nsockets;

    variorum_set_topology(&nsockets, NULL, NULL);
    gethostname(hostname, 1024);

    if (!init_dump_dram_power_limit)
    {
        init_dump_dram_power_limit = 1;
        fprintf(writedest, "_DRAM_POWER_LIMIT | Offset | Host | Socket | PowerLimBits | Watts | Seconds\n");
    }

    if (!get_dram_rapl_limit(socket, &l1, msr_power_limit, msr_rapl_unit))
    {
        fprintf(writedest, "_DRAM_POWER_LIMIT | 0x%lx | %s | %d | 0x%lx | %lf | %lf\n", msr_power_limit, hostname, socket, l1.bits, l1.watts, l1.seconds);
    }
}

void print_package_power_limit(FILE *writedest, off_t msr_power_limit, off_t msr_rapl_unit, int socket)
{
    struct rapl_limit l1, l2;
    char hostname[1024];

    gethostname(hostname, 1024);

    if (!get_package_rapl_limit(socket, &l1, &l2, msr_power_limit, msr_rapl_unit))
    {
        fprintf(writedest, "_PACKAGE_POWER_LIMIT Offset: 0x%lx Host: %s Socket: %d Bits: 0x%lx WattsPowerLim1: %lfW SecTimeWin1: %lf sec WattsPowerLim2: %lfW SecTimeWin2: %lf sec\n", msr_power_limit, hostname, socket, l1.bits, l1.watts, l1.seconds, l2.watts, l2.seconds);
    }
}

void print_dram_power_limit(FILE *writedest, off_t msr_power_limit, off_t msr_rapl_unit, int socket)
{
    struct rapl_limit l1;
    char hostname[1024];

    gethostname(hostname, 1024);

    if (!get_dram_rapl_limit(socket, &l1, msr_power_limit, msr_rapl_unit))
    {
        fprintf(writedest, "_DRAM_POWER_LIMIT Offset: 0x%lx Host: %s Socket: %d Bits: 0x%lx WattsPowerLim: %lfW SecTimeWin: %lf sec\n", msr_power_limit, hostname, socket, l1.bits, l1.watts, l1.seconds);
    }
}

int get_package_rapl_limit(const unsigned socket, struct rapl_limit *limit1, struct rapl_limit *limit2, off_t msr_power_limit, off_t msr_rapl_unit)
{
    if (limit1 != NULL)
    {
        read_msr_by_coord(socket, 0, 0, msr_power_limit, &(limit1->bits));
        limit1->translate_bits = 1;
    }
    if (limit2 != NULL)
    {
        read_msr_by_coord(socket, 0, 0, msr_power_limit, &(limit2->bits));
        limit2->translate_bits = 1;
    }
    calc_package_rapl_limit(socket, limit1, limit2, msr_rapl_unit);
    return 0;
}

int get_dram_rapl_limit(const unsigned socket, struct rapl_limit *limit, off_t msr_power_limit, off_t msr_rapl_unit)
{
    if (limit != NULL)
    {
        read_msr_by_coord(socket, 0, 0, msr_power_limit, &(limit->bits));
        limit->translate_bits = 1;
    }
    calc_dram_rapl_limit(socket, limit, msr_rapl_unit);
    return 0;
}

int set_package_power_limit(const unsigned socket, int package_power_limit, off_t msr_power_limit, off_t msr_rapl_unit)
{
    uint64_t val = 0;
    uint64_t currentval = 0;
    int ret = 0;
    struct rapl_limit *limit1 = (struct rapl_limit *) malloc(sizeof(struct rapl_limit));
    struct rapl_limit *limit2 = NULL;

    limit1->watts = package_power_limit;
    limit1->seconds = 1;
    limit1->bits = 0;
    limit1->translate_bits = 0;

    sockets_assert(&socket, __LINE__, __FILE__);

    /* If there is only one limit, grab the other existing one. */
    if (limit1 == NULL)
    {
#ifdef VARIORUM_DEBUG
        fprintf(stderr, "%s %s::%d DEBUG: only one rapl limit, retrieving any existing power limits\n", getenv("HOSTNAME"), __FILE__, __LINE__);
#endif
        ret = read_msr_by_coord(socket, 0, 0, msr_power_limit, &currentval);
        /* We want to keep the lower limit so mask off all other bits. */
        val |= currentval & 0x00000000FFFFFFFF;
    }
    else if (limit2 == NULL)
    {
#ifdef VARIORUM_DEBUG
        fprintf(stderr, "%s %s::%d DEBUG: only one rapl limit, retrieving any existing power limits\n", getenv("HOSTNAME"), __FILE__, __LINE__);
#endif
        ret = read_msr_by_coord(socket, 0, 0, msr_power_limit, &currentval);
        /* We want to keep the upper limit so mask off all other bits. */
        val |= currentval & 0xFFFFFFFF00000000;
    }

    if (calc_package_rapl_limit(socket, limit1, limit2, msr_rapl_unit))
    {
        return -1;
    }
    /* Enable the rapl limit (15 && 47) and turn on clamping (16 && 48). */
    if (limit1 != NULL)
    {
        val |= limit1->bits | (1LL << 15) | (1LL << 16);
    }
    if (limit2 != NULL)
    {
        val |= limit2->bits | (1LL << 47) | (1LL << 48);
    }
    if (limit1 != NULL || limit2 != NULL)
    {
        ret += write_msr_by_coord(socket, 0, 0, msr_power_limit, val);
    }

    free(limit1);
    return ret;
}

int get_rapl_power_info(const unsigned socket, struct rapl_power_info *info, off_t msr)
{
    uint64_t val = 0;

    sockets_assert(&socket, __LINE__, __FILE__);

#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d DEBUG: (get_rapl_power_info)\n", getenv("HOSTNAME"), __FILE__, __LINE__);
#endif

    read_msr_by_coord(socket, 0, 0, msr, &(info->msr_pkg_power_info));
    val = MASK_VAL(info->msr_pkg_power_info, 54, 48);
    translate(socket, &val, &(info->pkg_max_window), BITS_TO_SECONDS_STD, msr);

    val = MASK_VAL(info->msr_pkg_power_info, 46, 32);
    translate(socket, &val, &(info->pkg_max_power), BITS_TO_WATTS, msr);

    val = MASK_VAL(info->msr_pkg_power_info, 30, 16);
    translate(socket, &val, &(info->pkg_min_power), BITS_TO_WATTS, msr);

    val = MASK_VAL(info->msr_pkg_power_info, 14, 0);
    translate(socket, &val, &(info->pkg_therm_power), BITS_TO_WATTS, msr);

    return 0;
}

int rapl_storage(struct rapl_data **data)
{
    static struct rapl_data *rapl = NULL;
    static int nsockets = 0;
    static int init = 0;

    if (!init)
    {
        init = 1;
        variorum_set_topology(&nsockets, NULL, NULL);

        rapl = (struct rapl_data *) malloc(nsockets * sizeof(struct rapl_data));

        if (data != NULL)
        {
            *data = rapl;
        }
#ifdef VARIORUM_DEBUG
        fprintf(stderr, "%s %s::%d DEBUG: (storage) initialized rapl data at %p\n", getenv("HOSTNAME"), __FILE__, __LINE__, rapl);
        fprintf(stderr, "DEBUG: socket 0 has pkg_bits at %p\n", &rapl[0].pkg_bits);
#endif
        return 0;
    }
    /* If the data pointer is not null, it should point to the rapl array. */
    if (data != NULL)
    {
        *data = rapl;
    }
    return 0;
}

int get_power(off_t msr_rapl_unit, off_t msr_pkg_energy_status, off_t msr_dram_energy_status)
{
    static struct rapl_data *rapl = NULL;
    int nsockets;

    variorum_set_topology(&nsockets, NULL, NULL);

#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d DEBUG: (get_power) socket=%lu\n", getenv("HOSTNAME"), __FILE__, __LINE__, nsockets);
#endif

    if (rapl == NULL)
    {
        if (rapl_storage(&rapl))
        {
            return -1;
        }
    }

    if (rapl == NULL)
    {
        variorum_error_handler("RAPL storage failed", VARIORUM_ERROR_RAPL_INIT, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    read_rapl_data(msr_rapl_unit, msr_pkg_energy_status, msr_dram_energy_status);
    delta_rapl_data(msr_rapl_unit);

    return 0;
}

int delta_rapl_data(off_t msr_rapl_unit)
{
    /* The energy status register holds 32 bits, this is max unsigned int. */
    static double max_joules = UINT_MAX;
    static int init = 0;
    static int nsockets = 0;
    static struct rapl_data *rapl;
    int i = 0;

#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d DEBUG: (delta_rapl_data)\n", getenv("HOSTNAME"), __FILE__, __LINE__);
#endif
    if (!init)
    {
        variorum_set_topology(&nsockets, NULL, NULL);
        if (rapl_storage(&rapl))
        {
            return -1;
        }
        for (i = 0; i < nsockets; i++)
        {
            rapl->pkg_watts[i] = 0.0;
            rapl->pkg_joules[i] = 0.0;
            rapl->dram_watts[i] = 0.0;
            rapl->dram_joules[i] = 0.0;
        }
        init = 1;
        rapl->elapsed = 0;
        return 0;
    }
    /*
     * Get delta joules.
     * Now handles wraparound.
     * Make sure the pkg energy status register exists
     */
    for (i = 0; i < nsockets; i++)
    {
        /* Check to see if there was wraparound and use corresponding translation. */
        if ((double)*rapl->pkg_bits[i] - (double)rapl->old_pkg_bits[i] < 0)
        {
            rapl->pkg_delta_bits[i] = (uint64_t)((*rapl->pkg_bits[i] + (uint64_t)max_joules) - rapl->old_pkg_bits[i]);
            translate(i, &rapl->pkg_delta_bits[i], &rapl->pkg_delta_joules[i], BITS_TO_JOULES, msr_rapl_unit);
#ifdef VARIORUM_DEBUG
            fprintf(stderr, "OVF pkg%d new=0x%lx old=0x%lx -> %lf\n", i, *rapl->pkg_bits[i], rapl->old_pkg_bits[i], rapl->pkg_delta_joules[i]);
#endif
        }
        else
        {
            rapl->pkg_delta_joules[i] = rapl->pkg_joules[i] - rapl->old_pkg_joules[i];
#ifdef VARIORUM_DEBUG
            fprintf(stderr, "pkg%d pkg_joules[%d] = %lf, old_pkg_joules[%d] = %lf, pkg_delta_joules[%d] = %lf\n", i, i, rapl->pkg_joules[i], i, rapl->old_pkg_joules[i], i, rapl->pkg_delta_joules[i]);
#endif
        }
        /* This case should not happen. */
        if (rapl->pkg_delta_joules[i] < 0)
        {
            variorum_error_handler("Energy used since last same is negative", VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        }
        if (rapl->dram_joules[i] - rapl->old_dram_joules[i] < 0)
        {
            rapl->dram_delta_joules[i] = (rapl->dram_joules[i] + max_joules) - rapl->old_dram_joules[i];
        }
        else
        {
            rapl->dram_delta_joules[i] = rapl->dram_joules[i] - rapl->old_dram_joules[i];
        }
        /* Get watts. */
        if (rapl->elapsed > 0.0L)
        {
            rapl->pkg_watts[i]  = rapl->pkg_delta_joules[i]  / rapl->elapsed;
            rapl->dram_watts[i] = rapl->dram_delta_joules[i] / rapl->elapsed;
        }
        else
        {
            rapl->pkg_watts[i] = 0.0;
            rapl->dram_watts[i] = 0.0;
        }
    }
    return 0;
}

void print_power_data(FILE *writedest, off_t msr_power_limit, off_t msr_rapl_unit, off_t msr_pkg_energy_status, off_t msr_dram_energy_status)
{
    static int init = 0;
    static struct rapl_data *rapl = NULL;
    static struct timeval start;
    int nsockets = 0;
    struct timeval now;
    char hostname[1024];
    int i;

    gethostname(hostname, 1024);
    variorum_set_topology(&nsockets, NULL, NULL);

    get_power(msr_rapl_unit, msr_pkg_energy_status, msr_dram_energy_status);

    if (!init)
    {
        init = 1;
        gettimeofday(&start, NULL);
        rapl_storage(&rapl);
    }
    gettimeofday(&now, NULL);
    for (i = 0; i < nsockets; i++)
    {
#ifdef VARIORUM_DEBUG
        fprintf(writedest, "pkg%d_bits = %8.4lx   pkg%d_joules= %8.4lf\n", i, *rapl->pkg_bits[i], rapl->pkg_joules[i]);
#endif
        fprintf(writedest, "_PACKAGE_ENERGY_STATUS Offset: 0x%lx Host: %s Socket: %d Bits: 0x%lx Joules: %lf Watts: %lf Elapsed: %lf s Timestamp: %lf s\n",
                msr_pkg_energy_status, hostname, i, *rapl->pkg_bits[i], rapl->pkg_joules[i], rapl->pkg_watts[i], rapl->elapsed, now.tv_sec-start.tv_sec + (now.tv_usec-start.tv_usec)/1000000.0);
        fprintf(writedest, "_DRAM_ENERGY_STATUS Offset: 0x%lx Host: %s Socket: %d Bits: 0x%lx Joules: %lf Watts: %lf Elapsed: %lf s Timestamp: %lf s\n",
                msr_dram_energy_status, hostname, i, *rapl->dram_bits[i], rapl->dram_joules[i], rapl->dram_watts[i], rapl->elapsed, now.tv_sec-start.tv_sec + (now.tv_usec-start.tv_usec)/1000000.0);
    }
}

void dump_power_data(FILE *writedest, off_t msr_power_limit, off_t msr_rapl_unit, off_t msr_pkg_energy_status, off_t msr_dram_energy_status)
{
    static int init = 0;
    static struct rapl_data *rapl = NULL;
    static struct timeval start;
    int nsockets = 0;
    struct timeval now;
    char hostname[1024];
    int i;

    gethostname(hostname, 1024);
    variorum_set_topology(&nsockets, NULL, NULL);

    get_power(msr_rapl_unit, msr_pkg_energy_status, msr_dram_energy_status);

    if (!init)
    {
        gettimeofday(&start, NULL);
        fprintf(writedest, "_PACKAGE_ENERGY_STATUS | Offset | Host | Socket | Bits | Joules | Watts | Elapsed | Timestamp\n");
        rapl_storage(&rapl);
    }
    gettimeofday(&now, NULL);
    for (i = 0; i < nsockets; i++)
    {
        fprintf(writedest, "_PACKAGE_ENERGY_STATUS | 0x%lx | %s | %d | 0x%lx | %lf | %lf | %lf | %lf\n",
                msr_pkg_energy_status, hostname, i, *rapl->pkg_bits[i], rapl->pkg_joules[i], rapl->pkg_watts[i], rapl->elapsed, now.tv_sec-start.tv_sec + (now.tv_usec-start.tv_usec)/1000000.0);
    }

    if (!init)
    {
        fprintf(writedest, "_DRAM_ENERGY_STATUS | Offset | Host | Socket | Bits | Joules | Watts | Elapsed | Timestamp\n");
        init = 1;
    }
    for (i = 0; i < nsockets; i++)
    {
        fprintf(writedest, "_DRAM_ENERGY_STATUS | 0x%lx | %s | %d | 0x%lx | %lf | %lf | %lf | %lf\n",
                msr_dram_energy_status, hostname, i, *rapl->dram_bits[i], rapl->dram_joules[i], rapl->dram_watts[i], rapl->elapsed, now.tv_sec-start.tv_sec + (now.tv_usec-start.tv_usec)/1000000.0);
    }
}

//int dump_rapl_data(FILE *writedest)
//{
//    static int init = 0;
//    static uint64_t *rapl_flags = NULL;
//    static struct rapl_data *r = NULL;
//    static struct timeval start;
//    static uint64_t sockets = 0;
//    struct timeval now;
//
//    if (!init)
//    {
//        sockets = num_sockets();
//        init = 1;
//        gettimeofday(&start, NULL);
//        if (rapl_storage(&r, &rapl_flags))
//        {
//            return -1;
//        }
//    }
//#ifdef LIBMSR_DEBUG
//    fprintf(writedest, "pkg_bits = %8.4lx   pkg_joules= %8.4lf\n", *r->pkg_bits, r->pkg_joules);
//#endif
//    gettimeofday(&now, NULL);
//    int s;
//    for (s = 0; s < sockets; s++)
//    {
//        fprintf(writedest, "Socket: %d\n", s);
//        if (*rapl_flags & PKG_ENERGY_STATUS)
//        {
//            fprintf(writedest, "pkg_watts = %8.4lf   elapsed= %8.5lf   timestamp= %9.6lf\n", r->pkg_watts[s], r->elapsed, now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec)/1000000.0);
//        }
//        if (*rapl_flags & DRAM_ENERGY_STATUS)
//        {
//            fprintf(writedest, "dram_watts= %8.4lf   elapsed= %8.5lf   timestamp= %9.6lf\n", r->dram_watts[s], r->elapsed, now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec)/1000000.0);
//        }
//    }
//    return 0;
//}

int read_rapl_data(off_t msr_rapl_unit, off_t msr_pkg_energy_status, off_t msr_dram_energy_status)
{
    static struct rapl_data *rapl = NULL;
    static int init = 0;
    static int nsockets = 0;
    int i;

    if (!init)
    {
        variorum_set_topology(&nsockets, NULL, NULL);
        if (rapl_storage(&rapl))
        {
            return -1;
        }
        create_rapl_data_batch(rapl, msr_pkg_energy_status, msr_dram_energy_status);
        rapl->now.tv_sec = 0;
        rapl->now.tv_usec = 0;
        rapl->old_now.tv_sec = 0;
        rapl->old_now.tv_usec = 0;
        rapl->elapsed = 0;
        for (i = 0; i < nsockets; i++)
        {
            rapl->pkg_joules[i] = 0;
            rapl->old_pkg_joules[i] = 0;
        }
    }
#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d DEBUG: (read_rapl_data): socket=%lu at address %p\n", getenv("HOSTNAME"), __FILE__, __LINE__, nsockets, rapl);
#endif
    /* Move current variables to "old" variables. */
    rapl->old_now.tv_sec = rapl->now.tv_sec;
    rapl->old_now.tv_usec = rapl->now.tv_usec;
    /* Grab a timestamp. */
    gettimeofday(&(rapl->now), NULL);
    if (init)
    {
        rapl->elapsed = (rapl->now.tv_sec - rapl->old_now.tv_sec) + (rapl->now.tv_usec - rapl->old_now.tv_usec)/1000000.0;
        /* This case should not happen. */
        if (rapl->elapsed < 0) {
            variorum_error_handler("Elapsed time since last sample is negative", VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        }
        for (i = 0; i < nsockets; i++)
        {
#ifdef VARIORUM_DEBUG
            fprintf(stderr, "DEBUG: socket %lu msr 0x611 has destination %p\n", nsockets, rapl->pkg_bits);
#endif
            rapl->old_pkg_bits[i] = *rapl->pkg_bits[i];
            rapl->old_pkg_joules[i] = rapl->pkg_joules[i];
#ifdef VARIORUM_DEBUG
            fprintf(stderr, "DEBUG: (read_rapl_data): made it to 1st mark\n");
#endif

            rapl->old_dram_bits[i]	= *rapl->dram_bits[i];
            rapl->old_dram_joules[i] = rapl->dram_joules[i];

            ///* Make sure the pkg perf status register exists. */
            //if (*rapl_flags & PKG_PERF_STATUS)
            //{
            //    libmsr_error_handler("read_rapl_data(): MSR_PKG_PERF_STATUS not yet implemented", LIBMSR_ERROR_NOT_IMPLEMENTED_YET, getenv("HOSTNAME"), __FILE__, __LINE__);
            //}
            ///* Make sure the dram perf status register exists. */
            //if (*rapl_flags & DRAM_PERF_STATUS)
            //{
            //    libmsr_error_handler("read_rapl_data(): MSR_DRAM_PERF_STATUS not yet implemented", LIBMSR_ERROR_NOT_IMPLEMENTED_YET, getenv("HOSTNAME"), __FILE__, __LINE__);
            //}
        }
    }
    read_batch(RAPL_DATA);
    for (i = 0; i < nsockets; i++)
    {
//        if (*rapl_flags & DRAM_ENERGY_STATUS)
//        {
//#ifdef VARIORUM_DEBUG
//            fprintf(stderr, "DEBUG: (read_rapl_data): translating dram\n");
//#endif
//            translate(s, rapl->dram_bits[s], &rapl->dram_joules[s], BITS_TO_JOULES_DRAM);
//        }
#ifdef VARIORUM_DEBUG
        fprintf(stderr, "DEBUG: (read_rapl_data): translating pkg\n");
#endif
        translate(i, rapl->pkg_bits[i], &rapl->pkg_joules[i], BITS_TO_JOULES, msr_rapl_unit);
        translate(i, rapl->dram_bits[i], &rapl->dram_joules[i], BITS_TO_JOULES_DRAM, msr_rapl_unit);
#ifdef VARIORUM_DEBUG
        fprintf(stderr, "DEBUG: socket %d\n", i);
        fprintf(stderr, "DEBUG: elapsed %f\n", rapl->elapsed);
        fprintf(stderr, "DEBUG: pkg_bits %lx\n", *rapl->pkg_bits[i]);
        fprintf(stderr, "DEBUG: pkg_joules %lf\n", rapl->pkg_joules[i]);
        fprintf(stderr, "DEBUG: pkg_watts %lf\n", rapl->pkg_watts[i]);
        fprintf(stderr, "DEBUG: delta_joules %lf\n", rapl->pkg_delta_joules[i]);
#endif
    }
    init = 1;
    return 0;
}

void get_all_power_data(FILE *writedest, off_t msr_pkg_power_limit, off_t msr_dram_power_limit, off_t msr_rapl_unit, off_t msr_package_energy_status, off_t msr_dram_energy_status)
{
    // The length of the rlim array assumes dual socket system.
    static struct rapl_limit *rlim;
    //static struct rapl_limit rlim[6];
    static struct rapl_data *rapl = NULL;
    static int init_get_power_data = 0;
    static int nsockets;
    char hostname[1024];
    int i;
    int rlim_idx = 0;

    variorum_set_topology(&nsockets, NULL, NULL);
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
        fprintf(writedest, "\n");
    }

    rlim_idx = 0;
    fprintf(writedest, "_POWMON | %ld", now_ms());
    for (i = 0; i < nsockets; i++)
    {
        fprintf(writedest, " | %lf | %lf | %lf | %lf | %lf", rapl->pkg_delta_joules[i], rlim[rlim_idx].watts, rlim[rlim_idx+1].watts, rapl->dram_delta_joules[i], rlim[rlim_idx+2].watts);
        rlim_idx += 3;
    }
    fprintf(writedest, "\n");
}
