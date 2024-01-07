// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <config_architecture.h>
#include <variorum_error.h>

#include "msr_core.h"
#include "amd_power_features.h"

#ifdef LIBJUSTIFY_FOUND
#include <cprintf.h>
#endif

static void create_rapl_data_batch(struct rapl_data *rapl,
                                   off_t msr_core_energy_status)
{
    unsigned ncores;
#ifdef VARIORUM_WITH_AMD_CPU
    variorum_get_topology(NULL, &ncores, NULL, P_AMD_CPU_IDX);
#endif

    allocate_batch(RAPL_DATA, 2UL * ncores);

    rapl->core_bits = (uint64_t **) calloc(ncores, sizeof(uint64_t *));
    rapl->core_joules = (double *) calloc(ncores, sizeof(double));
    load_thread_batch(msr_core_energy_status, rapl->core_bits, RAPL_DATA);
}

static int rapl_storage(struct rapl_data **data)
{
    static struct rapl_data *rapl = NULL;
    static unsigned ncores = 0;
    static int init = 0;

    if (!init)
    {
        init = 1;
#ifdef VARIORUM_WITH_AMD_CPU
        variorum_get_topology(NULL, &ncores, NULL, P_AMD_CPU_IDX);
#endif

        rapl = (struct rapl_data *) malloc(ncores * sizeof(struct rapl_data));

        if (data != NULL)
        {
            *data = rapl;
        }
#ifdef VARIORUM_DEBUG
        fprintf(stderr, "%s %s::%d DEBUG: (storage) initialized rapl data at %p\n",
                getenv("HOSTNAME"), __FILE__, __LINE__, rapl);
        fprintf(stderr, "DEBUG: socket 0 has pkg_bits at %p\n",
                &rapl[0].core_bits);
#endif
        return 0;
    }
    if (data != NULL)
    {
        *data = rapl;
    }
    return 0;
}

static int read_rapl_data(off_t msr_core_energy_status)
{
    static struct rapl_data *rapl = NULL;
    static int init = 0;
    static unsigned ncores = 0;
    int i;

    if (!init)
    {
#ifdef VARIORUM_WITH_AMD_CPU
        variorum_get_topology(NULL, &ncores, NULL, P_AMD_CPU_IDX);
#endif
        if (rapl_storage(&rapl))
        {
            return -1;
        }
        create_rapl_data_batch(rapl, msr_core_energy_status);
        for (i = 0; i < (int)ncores; i++)
        {
            rapl->core_joules[i] = 0;
        }
    }
    read_batch(RAPL_DATA);
    init = 1;
    return 0;
}

static int get_rapl_unit(off_t msr_rapl_unit, double *energy_val)
{
    static struct rapl_units *ru = NULL;
    static uint64_t **val = NULL;

    ru = (struct rapl_units *) malloc(1 * sizeof(struct rapl_units));
    val = (uint64_t **) malloc(1 * sizeof(uint64_t *));
    allocate_batch(RAPL_UNIT, 1);
    load_socket_batch(msr_rapl_unit, val, RAPL_UNIT);
    read_batch(RAPL_UNIT);
    ru[1].msr_rapl_power_unit = *val[1];
    ru[1].joules = (double)(1 << (MASK_VAL(ru[1].msr_rapl_power_unit, 12, 8)));
    *energy_val = (1 / ru[1].joules);

    free(ru);
    ru = NULL;
    free(val);
    val = NULL;
    return 0;
}

int print_energy_data(FILE *writedest, off_t msr_rapl_unit,
                      off_t msr_core_energy_status)
{
    static struct rapl_data *rapl = NULL;
    static int init = 0;
    unsigned ncores = 0;
    // TODO: We can't test this API yet due to privilege issues. We need to
    // update the printing format here to include hostname and prefix
    // _AMDENERGY once we have ability to test.
    // char hostname[1024];
    int i, batchfd;
    double val;

    if ((batchfd = open(MSR_BATCH_PATH, O_RDWR)) < 0)
    {
        perror(MSR_BATCH_PATH);
        return batchfd;
    }

#ifdef VARIORUM_WITH_AMD_CPU
    variorum_get_topology(NULL, &ncores, NULL, P_AMD_CPU_IDX);
#endif

    read_rapl_data(msr_core_energy_status);

    if (!init)
    {
        rapl_storage(&rapl);
        init = 1;
    }

    get_rapl_unit(msr_rapl_unit, &val);

#ifdef LIBJUSTIFY_FOUND
    cfprintf(writedest, "%s  | %s  |\n", "Core", "Energy (J)");
#else
    fprintf(writedest, " Core   | Energy (J)   |\n");
#endif

    for (i = 0; i < (int)ncores; i++)
    {
#ifdef LIBJUSTIFY_FOUND
        cprintf(writedest, "%d  | %f  |\n", i, (*rapl->core_bits[i]) * val);
#else
        fprintf(writedest, "%6d  | %10f  |\n", i, (*rapl->core_bits[i]) * val);
#endif
    }

#ifdef LIBJUSTIFY_FOUND
    cflush();
#endif

    return 0;
}
