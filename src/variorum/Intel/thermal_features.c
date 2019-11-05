// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <thermal_features.h>
#include <config_architecture.h>
#include <msr_core.h>
#include <variorum_error.h>

void get_temp_target(struct msr_temp_target *s, off_t msr)
{
    int nsockets;
    static uint64_t **val = NULL;
    static int init_tt = 0;
    int i;

    variorum_set_topology(&nsockets, NULL, NULL);

    if (!init_tt)
    {
        init_tt = 1;
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(TEMP_TARGET, nsockets);
        load_socket_batch(msr, val, TEMP_TARGET);
    }

    read_batch(TEMP_TARGET);

    for (i = 0; i < nsockets; i++)
    {
        s[i].raw = *val[i];

        // Minimum temperature at which PROCHOT will be asserted in degree
        // Celsius (probably the TCC Activation Temperature).
        s[i].temp_target = MASK_VAL(s[i].raw, 23, 16);
    }
}


void get_therm_stat(struct therm_stat *s, off_t msr)
{
    int nthreads;
    static uint64_t **val = NULL;
    static int init_ts = 0;
    int i;

    variorum_set_topology(NULL, NULL, &nthreads);

    if (!init_ts)
    {
        init_ts = 1;
        val = (uint64_t **) malloc(nthreads * sizeof(uint64_t *));
        allocate_batch(THERM_STAT, nthreads);
        load_thread_batch(msr, val, THERM_STAT);
    }

    read_batch(THERM_STAT);

    for (i = 0; i < nthreads; i++)
    {
        s[i].raw = *val[i];

        // Indicates whether the digital thermal sensor high-temperature output
        // signal (PROCHOT#) is currently active.
        // (1=active)
        s[i].status = MASK_VAL(s[i].raw, 0, 0);

        // Indicates the history of the thermal sensor high temperature output
        // signal (PROCHOT#).
        // If equals 1, PROCHOT# has been asserted since a previous RESET or
        // clear 0 by user.
        s[i].status_log = MASK_VAL(s[i].raw, 1, 1);

        // Indicates whether PROCHOT# or FORCEPR# is being asserted by another
        // agent on the platform.
        s[i].PROCHOT_or_FORCEPR_event = MASK_VAL(s[i].raw, 2, 2);

        // Indicates whether PROCHOT# or FORCEPR# has been asserted by another
        // agent on the platform since the last clearing of the bit or a reset.
        // (1=has been externally asserted)
        // (0 to clear)
        // External PROCHOT# assertions are only acknowledged if the
        // Bidirectional Prochot feature is enabled.
        s[i].PROCHOT_or_FORCEPR_log = MASK_VAL(s[i].raw, 3, 3);

        // Indicates whether actual temp is currently higher than or equal to
        // the value set in Thermal Thresh 1.
        // (0 then actual temp is lower)
        // (1 then equal to or higher)
        s[i].crit_temp_status = MASK_VAL(s[i].raw, 4, 4);

        // Sticky bit indicates whether the crit temp detector output signal
        // has been asserted since the last reset or clear
        // (0 cleared) (1 asserted)
        s[i].crit_temp_log = MASK_VAL(s[i].raw, 5, 5);

        // Indicates whether actual temp is currently higher than or equal to
        // the value set in Thermal Threshold 1.
        // (0 actual temp is lower)
        // (1 actual temp is greater than or equal to TT#1)
        s[i].therm_thresh1_status = MASK_VAL(s[i].raw, 6, 6);

        // Sticky bit indicates whether Thermal Threshold #1 has been reached
        // since last reset or clear 0.
        s[i].therm_thresh1_log = MASK_VAL(s[i].raw, 7, 7);

        // Same as therm_thresh1_status, except for Thermal Threshold #2.
        s[i].therm_thresh2_status = MASK_VAL(s[i].raw, 8, 8);

        // Same as therm_thresh1_log, except for Thermal Threshold #2.
        s[i].therm_thresh2_log = MASK_VAL(s[i].raw, 9, 9);

        // Indicates whether the processor is currently operating below
        // OS-requested P-state (specified in IA32_PERF_CTL), or OS-requested
        // clock modulation duty cycle (in IA32_CLOCK_MODULATION).
        // This field supported only if CPUID.06H:EAX[bit 4] = 1.
        // Package level power limit notification can be delivered
        // independently to IA32_PACKAGE_THERM_STATUS MSR.
        s[i].power_limit_status = MASK_VAL(s[i].raw, 10, 10);

        // Sticky bit indicates the processor went below OS-requested P-state
        // or OS-requested clock modulation duty cycle since last RESET or
        // clear 0.
        // Supported only if CPUID.06H:EAX[bit 4] = 1.
        // Package level power limit notification is indicated independently in
        // IA32_PACKAGE_THERM_STATUS MSR.
        s[i].power_notification_log = MASK_VAL(s[i].raw, 11, 11);

        // Digital temperature reading in 1 degree Celsius relative to the TCC
        // activation temperature.
        // (0: TCC Activation temperature)
        // (1: (TCC Activation -1)... etc.)
        s[i].readout = MASK_VAL(s[i].raw, 22, 16);

        // Specifies the resolution (tolerance) of the digital thermal sensor.
        // The value is in degrees Celsius. Recommended that new threshold
        // values be offset from the current temperature by at least the
        // resolution + 1 in order to avoid hysteresis of interrupt generation.
        s[i].resolution_deg_celsius = MASK_VAL(s[i].raw, 30, 27);

        // Indicates if the digital readout is valid (valid if = 1).
        s[i].readout_valid = MASK_VAL(s[i].raw, 31, 31);
    }
}

int get_pkg_therm_stat(struct pkg_therm_stat *s, off_t msr)
{
    int nsockets;
    static uint64_t **val = NULL;
    static int init_pkg_ts = 0;
    int i;

    variorum_set_topology(&nsockets, NULL, NULL);

    if (!init_pkg_ts)
    {
        init_pkg_ts = 1;
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(PKG_THERM_STAT, nsockets);
        load_socket_batch(msr, val, PKG_THERM_STAT);
    }

    read_batch(PKG_THERM_STAT);
    for (i = 0; i < nsockets; i++)
    {
        s[i].raw = *val[i];

        // Pkg digital temp reading in 1 degree Celsius relative to the pkg TCC
        // activation temp.
        // (0 = Package TCC activation temp)
        // (1 = (PTCC Activation - 1) etc.
        // Note: lower reading actually higher temp
        s[i].readout = MASK_VAL(s[i].raw, 22, 16);

        // Indicates whether the digital thermal sensor high-temp output signal
        // (PROCHOT#) for the pkg currently active. (1=active)
        s[i].status = MASK_VAL(s[i].raw, 0, 0);

        // Indicates the history of thermal sensor high temp output signal
        // (PROCHOT#) of pkg.
        // (1= pkg PROCHOT# has been asserted since previous reset or last time
        // software cleared bit.
        s[i].status_log = MASK_VAL(s[i].raw, 1, 1);

        // Indicates whether pkg PROCHOT# is being asserted by another agent on
        // the platform.
        s[i].PROCHOT_event = MASK_VAL(s[i].raw, 2, 2);

        // Indicates whether pkg PROCHOT# has been asserted by another agent on
        // the platform since the last clearing of the bit by software or
        // reset. (1= has been externally asserted) (write 0 to clear).
        s[i].PROCHOT_log = MASK_VAL(s[i].raw, 3, 3);

        // Indicates whether pkg crit temp detector output signal is currently
        // active (1=active).
        s[i].crit_temp_status = MASK_VAL(s[i].raw, 4, 4);

        // Indicates whether pkg crit temp detector output signal been asserted
        // since the last clearing of bit or reset.
        // (1=has been asserted) (set 0 to clear).
        s[i].crit_temp_log = MASK_VAL(s[i].raw, 5, 5);

        // Indicates whether actual pkg temp is currently higher than or equal
        // to value set in Package Thermal Threshold #1.
        // (0=actual temp lower) (1= actual temp >= PTT#1).
        s[i].therm_thresh1_status = MASK_VAL(s[i].raw, 6, 6);

        // Indicates whether pkg therm threshold #1 has been reached since last
        // software clear of bit or reset. (1=reached) (clear with 0).
        s[i].therm_thresh1_log = MASK_VAL(s[i].raw, 7, 7);

        // Same as above (therm_thresh1_stat) except it is for threshold #2.
        s[i].therm_thresh2_status = MASK_VAL(s[i].raw, 8, 8);

        // Same as above (therm_thresh2_log) except it is for threshold #2
        s[i].therm_thresh2_log = MASK_VAL(s[i].raw, 9, 9);

        // Indicates pkg power limit forcing 1 or more processors to operate
        // below OS-requested P-state.
        // (Note: pkg power limit violation may be caused by processor cores or
        // by devices residing in the uncore - examine IA32_THERM_STATUS to
        // determine if cause from processor core).
        s[i].power_limit_status = MASK_VAL(s[i].raw, 10, 10);

        // Indicates any processor from package went below OS-requested P-state
        // or OS-requested clock modulation duty cycle since last clear or
        // RESET.
        s[i].power_notification_log = MASK_VAL(s[i].raw, 11, 11);
    }
    return 0;
}

int print_therm_temp_reading(FILE *writedest, off_t msr_therm_stat, off_t msr_pkg_therm_stat, off_t msr_temp_target)
{
    struct pkg_therm_stat *pkg_stat = NULL;
    struct msr_temp_target *t_target = NULL;
    struct therm_stat *t_stat = NULL;
    int idx;
    int i, j, k;
    int nsockets, ncores, nthreads;

    variorum_set_topology(&nsockets, &ncores, &nthreads);

    pkg_stat = (struct pkg_therm_stat *) malloc(nsockets * sizeof(struct pkg_therm_stat));
    get_pkg_therm_stat(pkg_stat, msr_pkg_therm_stat);

    t_target = (struct msr_temp_target *) malloc(nsockets * sizeof(struct msr_temp_target));
    get_temp_target(t_target, msr_temp_target);

    t_stat = (struct therm_stat *) malloc(nthreads * sizeof(struct therm_stat));
    get_therm_stat(t_stat, msr_therm_stat);

    for (i = 0; i < nsockets; i++)
    {
        for (j = 0; j < ncores/nsockets; j++)
        {
            for (k = 0; k < nthreads/ncores; k++)
            {
                idx = (k * nsockets * (ncores/nsockets)) + (i * (ncores/nsockets)) + j;
                fprintf(writedest, "socket: %d, phys_core: %d, log_thread: %d, ", i, j, idx);
                fprintf(writedest, "TCC: %d C, ", (int)t_target[i].temp_target);
                fprintf(writedest, "pkg_reading: %d C, ", pkg_stat[i].readout);
                fprintf(writedest, "pkg_actual: %d C, ", (int)t_target[i].temp_target-pkg_stat[i].readout);
                fprintf(writedest, "thread_reading: %d C, ", t_stat[idx].readout);
                fprintf(writedest, "thread_actual: %d C, ", (int)t_target[i].temp_target-t_stat[idx].readout);
                fprintf(writedest, "thread_digital_reading_valid: %d\n", t_stat[idx].readout_valid);
            }
        }
    }

    if (pkg_stat != NULL)
    {
        free(pkg_stat);
    }
    if (t_stat != NULL)
    {
        free(t_stat);
    }
    if (t_target != NULL)
    {
        free(t_target);
    }
    return 0;
}

int dump_therm_temp_reading(FILE *writedest, off_t msr_therm_stat, off_t msr_pkg_therm_stat, off_t msr_temp_target)
{
    struct pkg_therm_stat *pkg_stat = NULL;
    struct msr_temp_target *t_target = NULL;
    struct therm_stat *t_stat = NULL;
    int idx;
    int i, j, k;
    int nsockets, ncores, nthreads;

    variorum_set_topology(&nsockets, &ncores, &nthreads);

    pkg_stat = (struct pkg_therm_stat *) malloc(nsockets * sizeof(struct pkg_therm_stat));
    get_pkg_therm_stat(pkg_stat, msr_pkg_therm_stat);

    t_target = (struct msr_temp_target *) malloc(nsockets * sizeof(struct msr_temp_target));
    get_temp_target(t_target, msr_temp_target);

    t_stat = (struct therm_stat *) malloc(nthreads * sizeof(struct therm_stat));
    get_therm_stat(t_stat, msr_therm_stat);

    fprintf(writedest, "_THERMALS | socket | phys_core | log_thread | TCC_celsius | pkg_reading_celsius | pkg_actual_celsius | thread_reading_celsius | thread_actual_celsius | thread_digital_reading_valid\n");
    for (i = 0; i < nsockets; i++)
    {
        for (j = 0; j < ncores/nsockets; j++)
        {
            for (k = 0; k < nthreads/ncores; k++)
            {
                idx = (k * nsockets * (ncores/nsockets)) + (i * (ncores/nsockets)) + j;
                fprintf(writedest, "_THERMALS | %d | %d | %d | ", i, j, idx);
                fprintf(writedest, "%d | ", (int)t_target[i].temp_target);
                fprintf(writedest, "%d | ", pkg_stat[i].readout);
                fprintf(writedest, "%d | ", (int)t_target[i].temp_target-pkg_stat[i].readout);
                fprintf(writedest, "%d | ", t_stat[idx].readout);
                fprintf(writedest, "%d | ", (int)t_target[i].temp_target-t_stat[idx].readout);
                fprintf(writedest, "%d\n", t_stat[idx].readout_valid);
            }
        }
    }

    free(pkg_stat);
    free(t_stat);
    free(t_target);
    return 0;
}


///// @brief Initialize storage for IA32_THERM_INTERRUPT.
/////
///// @param [out] ti Data for per-core thermal interrupts.
//static void init_therm_interrupt(struct therm_interrupt *ti)
//{
//    uint64_t cores = num_cores();
//    uint64_t threadsPerCore;
//    core_config(NULL, &threadsPerCore, NULL, NULL);
//
//    ti->raw = (uint64_t **) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    ti->low_temp_enable = (int *) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    ti->PROCHOT_enable = (int *) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    ti->FORCEPR_enable = (int *) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    ti->crit_temp_enable = (int *) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    ti->thresh1_val = (int *) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    ti->thresh1_enable = (int *) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    ti->thresh2_val = (int *) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    ti->thresh2_enable = (int *) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    ti->pwr_limit_notification_enable = (int *) libmsr_malloc(cores * threadsPerCore * sizeof(int));
//    printf("init_therm_interrupt allocate batch\n");
//    allocate_batch(THERM_INTERR, cores * threadsPerCore);
//    load_thread_batch(IA32_THERM_INTERRUPT, ti->raw, THERM_INTERR);
////    uint64_t cores =  num_cores();
////
////    ti->raw = (uint64_t **) libmsr_malloc(cores * sizeof(uint64_t *));
////    ti->high_temp_enable = (int *) libmsr_malloc(cores * sizeof(int));
////    ti->low_temp_enable = (int *) libmsr_malloc(cores * sizeof(int));
////    ti->PROCHOT_enable = (int *) libmsr_malloc(cores * sizeof(int));
////    ti->FORCEPR_enable = (int *) libmsr_malloc(cores * sizeof(int));
////    ti->crit_temp_enable = (int *) libmsr_malloc(cores * sizeof(int));
////    ti->thresh1_val = (int *) libmsr_malloc(cores * sizeof(int));
////    ti->thresh1_enable = (int *) libmsr_malloc(cores * sizeof(int));
////    ti->thresh2_val = (int *) libmsr_malloc(cores * sizeof(int));
////    ti->thresh2_enable = (int *) libmsr_malloc(cores * sizeof(int));
////    ti->pwr_limit_notification_enable = (int *) libmsr_malloc(cores * sizeof(int));
////    printf("init_therm_interrupt allocate batch\n");
////    allocate_batch(THERM_INTERR, cores);
////    load_core_batch(IA32_THERM_INTERRUPT, ti->raw, THERM_INTERR);
//}
//
///// @brief Initialize storage for IA32_PACKAGE_THERM_INTERRUPT.
/////
///// @param [out] pti Data for package-level thermal interrupts.
//static void init_pkg_therm_interrupt(struct pkg_therm_interrupt *pti)
//{
//    uint64_t sockets;
//    uint64_t threadsPerCore;
//    core_config(NULL, &threadsPerCore, &sockets, NULL);
//
//    pti->raw = (uint64_t **) libmsr_malloc(sockets * threadsPerCore * sizeof(uint64_t *));
//    pti->high_temp_enable = (int *) libmsr_malloc(sockets * threadsPerCore * sizeof(int));
//    pti->low_temp_enable = (int *) libmsr_malloc(sockets * threadsPerCore * sizeof(int));
//    pti->PROCHOT_enable = (int *) libmsr_malloc(sockets * threadsPerCore * sizeof(int));
//    pti->crit_temp_enable = (int *) libmsr_malloc(sockets * threadsPerCore * sizeof(int));
//    pti->thresh1_val = (int *) libmsr_malloc(sockets * threadsPerCore * sizeof(int));
//    pti->thresh1_enable = (int *) libmsr_malloc(sockets * threadsPerCore * sizeof(int));
//    pti->thresh2_val = (int *) libmsr_malloc(sockets * threadsPerCore * sizeof(int));
//    pti->thresh2_enable = (int *) libmsr_malloc(sockets * threadsPerCore * sizeof(int));
//    pti->pwr_limit_notification_enable = (int *) libmsr_malloc(sockets * threadsPerCore * sizeof(int));
//    allocate_batch(PKG_THERM_INTERR, sockets * threadsPerCore);
//    load_socket_batch(IA32_PACKAGE_THERM_INTERRUPT, pti->raw, PKG_THERM_INTERR);
//}
//
//void store_therm_interrupt(struct therm_interrupt **ti)
//{
//    static struct therm_interrupt t_interrupt;
//    static int init = 0;
//
//    if (!init)
//    {
//        init_therm_interrupt(&t_interrupt);
//        init = 1;
//    }
//    if (ti != NULL)
//    {
//        *ti = &t_interrupt;
//    }
//}
//
//void get_therm_interrupt(struct therm_interrupt *s)
//{
//    uint64_t numCores = num_cores();
//    uint64_t threadsPerCore = 0;
//    int i;
//    core_config(NULL, &threadsPerCore, NULL, NULL);
//
//    read_batch(THERM_INTERR);
//    for (i = 0; i < numCores * threadsPerCore; i++)
//    {
//        // Allows the BIOS to enable the generation of an interrupt on the
//        // transition from low-temp to a high-temp threshold.
//        // 0 (default) disable[i]s interrupts (1 enables interrupts).
//        s->high_temp_enable[i] = MASK_VAL(*s->raw[i], 0, 0);
//
//        // Allows the BIOS to enable generation of an interrupt on the
//        // transition from high temp to low temp (TCC deactivation-activation).
//        // 0 (default) disable[i]s interrupts (1 enables interrupts).
//        s->low_temp_enable[i] = MASK_VAL(*s->raw[i], 1, 1);
//
//        // Allows BIOS or OS to enable generation of an interrupt when PROCHOT
//        // has been asserted by another agent on the platform and the
//        // Bidirectional Prochot feature is enable[i]d.
//        // (0 disable[i]s) (1 enables).
//        s->PROCHOT_enable[i] = MASK_VAL(*s->raw[i], 2, 2);
//
//        // Allows the BIOS or OS to enable generation of an interrupt when
//        // FORCEPR# has been asserted by another agent on the platform.
//        // (0 disable[i]s the interrupt) (2 enables).
//        s->FORCEPR_enable[i] = MASK_VAL(*s->raw[i], 3, 3);
//
//        // Enables generations of interrupt when the critical temperature
//        // detector has detected a critical thermal condition.
//        // Recommended response: system shutdown.
//        // (0 disable[i]s interrupt) (1 enables).
//        s->crit_temp_enable[i] = MASK_VAL(*s->raw[i], 4, 4);
//
//        // A temp threshold. Encoded relative to the TCC Activation temperature
//        // (same format as digital readout) used to generate
//        // therm_thresh1_status and therm_thresh1_log and Threshold #1 thermal
//        // interrupt delivery.
//        s->thresh1_val[i] = MASK_VAL(*s->raw[i], 14, 8);
//
//        // Enables generation of an interrupt when the actual temperature
//        // crosses Threshold #1 setting in any direction.
//        // (ZERO ENABLES the interrupt) (ONE DISABLES the interrupt).
//        s->thresh1_enable[i] = MASK_VAL(*s->raw[i], 15, 15);
//
//        // See above description for thresh1_val (just for thresh2).
//        s->thresh2_val[i] = MASK_VAL(*s->raw[i], 22, 16);
//
//        // See above description for thresh1_enable (just for thresh2).
//        s->thresh2_enable[i] = MASK_VAL(*s->raw[i], 23, 23);
//
//        // Enables generation of power notification events when the processor
//        // went below OS-requested P-state or OS-requested clock modulation
//        // duty cycle.
//        // THIS FIELD SUPPORTED ONLY IF CPUID.06H:EAX[bit 4] = 1.
//        // Package level power limit notification can be enable[i]d
//        // independently by IA32_PACKAGE_THERM_INTERRUPT MSR.
//        s->pwr_limit_notification_enable[i] = MASK_VAL(*s->raw[i], 24, 24);
//    }
//}
//
//void get_pkg_therm_interrupt(struct pkg_therm_interrupt *s)
//{
//    uint64_t sockets = num_sockets();
//    int i;
//
//    read_batch(PKG_THERM_INTERR);
//    for (i = 0; i < sockets; i++)
//    {
//        // Allows the BIOS to enable the generation of an interrupt on
//        // transition from low temp to pkg high temp threshold.
//        // (0 (default)- disables interrupts) (1=enables interrupts)
//        s->high_temp_enable[i] = MASK_VAL(*s->raw[i], 0, 0);
//
//        // Allows BIOS to enable the generation of an interrupt on transition
//        // from high temp to a low temp (TCC deactivation-activation).
//        // (0 (default)- disables interrupts) (1=enables interrupts)
//        s->low_temp_enable[i] = MASK_VAL(*s->raw[i], 1, 1);
//
//        // Allows BIOS or OS to enable generation of an interrupt when pkg
//        // PROCHOT# has been asserted by another agent on the platform and the
//        // Bidirectional Prochot feature is enabled. (0 disables interrupt) (1
//        // enables interrupt)
//        s->PROCHOT_enable[i] = MASK_VAL(*s->raw[i], 2, 2);
//
//        // Enables generation of interrupt when pkg crit temp detector has
//        // detected a crit thermal condition. Recommended response: system shut
//        // down.
//        // (0 disables interrupt) (1 enables)
//        s->crit_temp_enable[i] = MASK_VAL(*s->raw[i], 4, 4);
//
//        // A temp threshold, encoded relative to the Package TCC Activation
//        // temp using format as Digital Readout.
//        // Compared against the Package Digital Readout and used to generate
//        // Package Thermal Threshold #1 status and log bits as well as the
//        // Package Threshold #1 thermal interrupt delivery.
//        s->thresh1_val[i] = MASK_VAL(*s->raw[i], 14, 8);
//
//        // Enables the generation of an interrupt when the actual temp crosses
//        // the thresh1_val setting in any direction.
//        // (0 enables interrupt) (1 disables interrupt)
//        s->thresh1_enable[i] = MASK_VAL(*s->raw[i], 15, 15);
//
//        // See thresh1_val.
//        s->thresh2_val[i] = MASK_VAL(*s->raw[i], 22, 16);
//
//        // See thresh1_enable.
//        s->thresh2_enable[i] = MASK_VAL(*s->raw[i], 23, 23);
//
//        // Enables generation of package power notification events.
//        s->pwr_limit_notification_enable[i] = MASK_VAL(*s->raw[i], 24, 24);
//    }
//}
//
//// probably not worth batching
//
//int therm2_ctl_storage(uint64_t **thermctlref)
//{
//    static uint64_t sockets = 0;
//    static uint64_t *thermctl;
//
//    if (sockets == 0)
//    {
//        sockets = num_sockets();
//        thermctl = (uint64_t *) libmsr_malloc(sockets * sizeof(uint64_t));
//    }
//    if (thermctlref != NULL)
//    {
//        *thermctlref = thermctl;
//    }
//    return 0;
//}
//
//int get_pkg_therm2_ctl(void)
//{
//    uint64_t sockets = num_sockets();
//    uint64_t *thermctl;
//    int ret;
//    int i;
//
//    therm2_ctl_storage(&thermctl);
//    for (i = 0; i < sockets; i++)
//    {
//        ret = read_msr_by_coord(i, 0, 0, MSR_THERM2_CTL, &thermctl[i]);
//        if (ret)
//        {
//            return ret;
//        }
//    }
//    return 0;
//}
//
//int dump_therm2_ctl(FILE *writedest)
//{
//    int ret = 0;
//    int i;
//    uint64_t *thermctl;
//    uint64_t sockets = num_sockets();
//
//    ret = get_pkg_therm2_ctl();
//    ret += therm2_ctl_storage(&thermctl);
//    fprintf(writedest, "Therm2 CTL\nSocket\tValue\n");
//    for (i = 0; i < sockets; i++)
//    {
//        fprintf(writedest, "%d\t%lx\n", i, thermctl[i]);
//    }
//    if (ret < 0)
//    {
//        return ret;
//    }
//    return 0;
//}
//
//void set_therm_stat(struct therm_stat *s)
//{
//    uint64_t numCores = num_cores();
//    int i;
//
//    read_batch(THERM_STAT);
//    for (i = 0; i < numCores; i++)
//    {
//        *s->raw[i] = (*s->raw[i] & (~(1<<1))) | (s->status_log[i] << 1);
//        *s->raw[i] = (*s->raw[i] & (~(1<<3))) | (s->PROCHOT_or_FORCEPR_log[i] << 1);
//        *s->raw[i] = (*s->raw[i] & (~(1<<5))) | (s->crit_temp_log[i] << 1);
//        *s->raw[i] = (*s->raw[i] & (~(1<<7))) | (s->therm_thresh1_log[i] << 1);
//        *s->raw[i] = (*s->raw[i] & (~(1<<9))) | (s->therm_thresh2_log[i] << 1);
//        *s->raw[i] = (*s->raw[i] & (~(1<<11))) | (s->power_notification_log[i] << 1);
//    }
//    write_batch(THERM_STAT);
//    /* Not sure if I should update the struct here or not. */
//}
//
//void set_therm_interrupt(struct therm_interrupt *s)
//{
//    uint64_t numCores = num_cores();
//    int i;
//
//    read_batch(THERM_INTERR);
//    for (i = 0; i < numCores; i++)
//    {
//        *s->raw[i] = (*s->raw[i] & (~(1<<0))) | (s->high_temp_enable[i] << 0);
//        *s->raw[i] = (*s->raw[i] & (~(1<<1))) | (s->low_temp_enable[i] << 1);
//        *s->raw[i] = (*s->raw[i] & (~(1<<2))) | (s->PROCHOT_enable[i] << 2);
//        *s->raw[i] = (*s->raw[i] & (~(1<<3))) | (s->FORCEPR_enable[i] << 3);
//        *s->raw[i] = (*s->raw[i] & (~(1<<4))) | (s->crit_temp_enable[i] << 4);
//        *s->raw[i] = (*s->raw[i] & (~(7<<8))) | (s->thresh1_val[i] << 8);
//        *s->raw[i] = (*s->raw[i] & (~(1<<15))) | (s->thresh1_enable[i] << 15);
//        *s->raw[i] = (*s->raw[i] & (~(7<<16))) | (s->thresh2_val[i] << 16);
//        *s->raw[i] = (*s->raw[i] & (~(1<<23))) | (s->thresh2_enable[i] << 23);
//        *s->raw[i] = (*s->raw[i] & (~(1<<24))) | (s->pwr_limit_notification_enable[i] << 24);
//    }
//    write_batch(THERM_INTERR);
//}
//
//void set_pkg_therm_stat(struct pkg_therm_stat *s)
//{
//    uint64_t sockets = num_sockets();
//    int i;
//
//    read_batch(PKG_THERM_STAT);
//    for (i = 0; i < sockets; i++)
//    {
//        *s->raw[i] = (*s->raw[i] & (~(1<<1))) | (s->status_log[i] << 1);
//        *s->raw[i] = (*s->raw[i] & (~(1<<3))) | (s->PROCHOT_log[i] << 3);
//        *s->raw[i] = (*s->raw[i] & (~(1<<5))) | (s->crit_temp_log[i] << 5);
//        *s->raw[i] = (*s->raw[i] & (~(1<<7))) | (s->therm_thresh1_log[i] << 7);
//        *s->raw[i] = (*s->raw[i] & (~(1<<9))) | (s->therm_thresh2_log[i] << 9);
//        *s->raw[i] = (*s->raw[i] & (~(1<<11))) | (s->power_notification_log[i] << 11);
//    }
//    write_batch(PKG_THERM_STAT);
//}
//
//void set_pkg_therm_interrupt(struct pkg_therm_interrupt *s)
//{
//    uint64_t sockets = num_sockets();
//    read_batch(PKG_THERM_INTERR);
//    int i;
//    for (i = 0; i < sockets; i++)
//    {
//        *s->raw[i] = (*s->raw[i] & (~(1<<0))) | (s->high_temp_enable[i] << 0);
//        *s->raw[i] = (*s->raw[i] & (~(1<<1))) | (s->low_temp_enable[i] << 1);
//        *s->raw[i] = (*s->raw[i] & (~(1<<2))) | (s->PROCHOT_enable[i] << 2);
//        *s->raw[i] = (*s->raw[i] & (~(1<<4))) | (s->crit_temp_enable[i] << 4);
//        *s->raw[i] = (*s->raw[i] & (~(7<<8))) | (s->thresh1_val[i] << 8);
//        *s->raw[i] = (*s->raw[i] & (~(1<<15))) | (s->thresh1_enable[i] << 15);
//        *s->raw[i] = (*s->raw[i] & (~(7<<16))) | (s->thresh2_val[i] << 16);
//        *s->raw[i] = (*s->raw[i] & (~(1<<23))) | (s->thresh2_enable[i] << 23);
//        *s->raw[i] = (*s->raw[i] & (~(1<<24))) | (s->pwr_limit_notification_enable[i] << 24);
//    }
//    write_batch(PKG_THERM_INTERR);
//}
