// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <inttypes.h>
#include <stdlib.h>

#include <misc_features.h>
#include <config_architecture.h>
#include <msr_core.h>
#include <variorum_error.h>

/* 02/25/19 SB
 * This format will be used moving forward for Xeon
 * I am currently batching the read of the turbo ratio limit, which is per
 * socket. Should we be? May be too much overhead. Is it necessary to read off
 * both sockets?"
 */
int get_max_non_turbo_ratio(off_t msr_platform_info)
{
    static int init = 0;
    static int nsockets = 0;
    static uint64_t **val = NULL;
    int max_non_turbo_ratio;

    variorum_get_topology(&nsockets, NULL, NULL);
    if (!init)
    {
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(PLATFORM_INFO, nsockets);
        load_socket_batch(msr_platform_info, val, PLATFORM_INFO);
        init = 1;
    }

    read_batch(PLATFORM_INFO);
    max_non_turbo_ratio = (int)(MASK_VAL(*val[0], 15, 8));
    /// Do sockets match?
    if (nsockets != 1)
    {
        if (max_non_turbo_ratio != (int)(MASK_VAL(*val[1], 15, 8)))
        {
            return VARIORUM_ERROR_INVAL;
        }
    }
    /// 100 MHz multiplier
    return max_non_turbo_ratio * 100;
}

/* 02/25/19 SB
 * This format will be used moving forward for Xeon
 * I am currently batching the read of the turbo ratio limit, which is per
 * socket. Should we be? May be too much overhead. Is it necessary to read off
 * both sockets?"
 */
int get_max_efficiency_ratio(off_t msr_platform_info)
{
    static int init = 0;
    static int nsockets = 0;
    static uint64_t **val = NULL;
    int max_efficiency_ratio;
    int socket;

    variorum_get_topology(&nsockets, NULL, NULL);
    if (!init)
    {
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(MAX_EFFICIENCY, nsockets);
        load_socket_batch(msr_platform_info, val, MAX_EFFICIENCY);
        init = 1;
    }

    read_batch(MAX_EFFICIENCY);
    max_efficiency_ratio = (int)(MASK_VAL(*val[0], 47, 40));
    /// Do sockets match?
    if (nsockets != 1)
    {
        if (max_efficiency_ratio != (int)(MASK_VAL(*val[1], 47, 40)))
        {
            return VARIORUM_ERROR_INVAL;
        }
    }
    /// 100 MHz multiplier
    return max_efficiency_ratio * 100;
}


/* 02/25/19 SB
 * This format will be used moving forward for Xeon
 * I am currently batching the read of the turbo ratio limit, which is per
 * socket. Should we be? May be too much overhead. Is it necessary to read off
 * both sockets?"
 */
int get_min_operating_ratio(off_t msr_platform_info)
{
    static int init = 0;
    static int nsockets = 0;
    static uint64_t **val = NULL;
    int min_operating_ratio;
    int socket;

    variorum_get_topology(&nsockets, NULL, NULL);
    if (!init)
    {
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(MIN_OPERATING_RATIO, nsockets);
        load_socket_batch(msr_platform_info, val, MIN_OPERATING_RATIO);
        init = 1;
    }

    read_batch(MIN_OPERATING_RATIO);
    min_operating_ratio = (int)(MASK_VAL(*val[0], 55, 48));
    /// Do sockets match?
    if (nsockets != 1)
    {
        if (min_operating_ratio != (int)(MASK_VAL(*val[1], 55, 48)))
        {
            return VARIORUM_ERROR_INVAL;
        }
    }
    /// 100 MHz multiplier
    return min_operating_ratio * 100;
}

int get_turbo_ratio_limit(off_t msr_turbo_ratio_limit)
{
    static int init = 0;
    static int nsockets = 0;
    static uint64_t **val = NULL;
    int socket, ncores, nbits;

    variorum_get_topology(&nsockets, &ncores, NULL);
    if (!init)
    {
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(TURBO_RATIO_LIMIT, nsockets);
        load_socket_batch(msr_turbo_ratio_limit, val, TURBO_RATIO_LIMIT);
        init = 1;
    }

    read_batch(TURBO_RATIO_LIMIT);

    if (nsockets != 1)
    {
        if (*val[0] != *val[1])
        {
            return VARIORUM_ERROR_INVAL;
        }
    }

    int core = 1;
    for (nbits = 0; nbits < 64; nbits += 8)
    {
        printf("%2dC = %d MHz\n", core, (int)(MASK_VAL(*val[0], nbits + 7,
                                              nbits)) * 100);
        core += 1;
        if (core > ncores)
        {
            break;
        }
    }

    return 0;
}


/* 02/25/19 SB
 * This format will be used moving forward for Xeon
 * I am currently batching the read of the turbo ratio limit, which is per
 * socket. Should we be? May be too much overhead. Is it necessary to read off
 * both sockets?"
 */
int get_turbo_ratio_limits(off_t msr_turbo_ratio_limit,
                           off_t msr_turbo_ratio_limit1)
{
    static int init = 0;
    static int nsockets = 0;
    static uint64_t **val = NULL;
    static uint64_t **val2 = NULL;
    int socket, ncores, nbits;

    variorum_get_topology(&nsockets, &ncores, NULL);
    if (!init)
    {
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        val2 = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(TURBO_RATIO_LIMIT, nsockets);
        allocate_batch(TURBO_RATIO_LIMIT_CORES, nsockets);
        load_socket_batch(msr_turbo_ratio_limit, val, TURBO_RATIO_LIMIT);
        load_socket_batch(msr_turbo_ratio_limit1, val2, TURBO_RATIO_LIMIT1);
        init = 1;
    }

    read_batch(TURBO_RATIO_LIMIT);
    read_batch(TURBO_RATIO_LIMIT1);

    /// Do sockets match?
    if (nsockets != 1)
    {
        if (*val[0] != *val[1] || *val2[0] != *val2[1])
        {
            return VARIORUM_ERROR_INVAL;
        }
    }

    int core = 1;
    for (nbits = 0; nbits < 64; nbits += 8)
    {
        printf("%2dC = %d MHz\n", core, (int)(MASK_VAL(*val[0], nbits + 7,
                                              nbits)) * 100);
        core += 1;
        if (core > ncores)
        {
            break;
        }
    }
    for (nbits = 0; nbits < 64; nbits += 8)
    {
        printf("%2dC = %d MHz\n", core, (int)(MASK_VAL(*val2[0], nbits + 7,
                                              nbits)) * 100);
        core += 1;
        if (core >= ncores)
        {
            break;
        }
    }

    return 0;
}

int get_turbo_ratio_limits_skx(off_t msr_turbo_ratio_limit,
                               off_t msr_turbo_ratio_limit_cores)
{
    static int init = 0;
    static int nsockets = 0;
    static uint64_t **val = NULL;
    static uint64_t **val2 = NULL;
    int socket, ncores, nbits;

    variorum_get_topology(&nsockets, &ncores, NULL);
    if (!init)
    {
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        val2 = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(TURBO_RATIO_LIMIT, nsockets);
        allocate_batch(TURBO_RATIO_LIMIT_CORES, nsockets);
        load_socket_batch(msr_turbo_ratio_limit, val, TURBO_RATIO_LIMIT);
        load_socket_batch(msr_turbo_ratio_limit_cores, val2, TURBO_RATIO_LIMIT_CORES);
        init = 1;
    }

    read_batch(TURBO_RATIO_LIMIT);
    read_batch(TURBO_RATIO_LIMIT_CORES);

    /// Do sockets match?
    if (nsockets != 1)
    {
        if (*val[0] != *val[1] || *val2[0] != *val2[1])
        {
            return VARIORUM_ERROR_INVAL;
        }
    }

    int core;
    for (nbits = 0; nbits < 64; nbits += 8)
    {
        core = (int)(MASK_VAL(*val2[0], nbits + 7, nbits));
        if (core > ncores)
        {
            break;
        }
        printf("%2dC = %d MHz\n", core, (int)(MASK_VAL(*val[0], nbits + 7,
                                              nbits)) * 100);
    }

    return 0;
}

int config_tdp(int nlevels, off_t msr_config_tdp_level1,
               off_t msr_config_tdp_level2, off_t msr_config_tdp_nominal)
{
    static int init = 0;
    static int nsockets = 0;
    static uint64_t **l1 = NULL;
    static uint64_t **l2 = NULL;
    static uint64_t **nom = NULL;
    int level1, level2, nominal;
    int socket;

    variorum_get_topology(&nsockets, NULL, NULL);
    if (!init)
    {
        l1 = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        l2 = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        nom = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(TDP_CONFIG_L1, nsockets);
        allocate_batch(TDP_CONFIG_L2, nsockets);
        allocate_batch(TDP_CONFIG_NOM, nsockets);
        load_socket_batch(msr_config_tdp_level1, l1, TDP_CONFIG_L1);
        load_socket_batch(msr_config_tdp_level2, l2, TDP_CONFIG_L2);
        load_socket_batch(msr_config_tdp_nominal, nom, TDP_CONFIG_NOM);
        init = 1;
    }

    int err = 0;
    err |= read_batch(TDP_CONFIG_L1);
    err |= read_batch(TDP_CONFIG_L2);
    err |= read_batch(TDP_CONFIG_NOM);
    if (err)
    {
        variorum_error_handler("Batch read error", VARIORUM_ERROR_MSR_BATCH,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    switch (nlevels)
    {
        case 2:
            level2 = (int)(MASK_VAL(*l2[0], 23, 16));
            if (nsockets != 1)
            {
                if (level2 != (int)(MASK_VAL(*l2[1], 23, 16)))
                {
                    return VARIORUM_ERROR_INVAL;
                }
            }
            printf("AVX512  = %d MHz\n", level2 * 100);
        case 1:
            level1 = (int)(MASK_VAL(*l1[0], 23, 16));
            if (nsockets != 1)
            {
                if (level1 != (int)(MASK_VAL(*l1[1], 23, 16)))
                {
                    return VARIORUM_ERROR_INVAL;
                }
            }
            printf("AVX     = %d MHz\n", level1 * 100);
        case 0:
            nominal = (int)(MASK_VAL(*nom[0], 23, 16));
            if (nsockets != 1)
            {
                if (nominal != (int)(MASK_VAL(*nom[1], 23, 16)))
                {
                    return VARIORUM_ERROR_INVAL;
                }
            }
            printf("Non-AVX = %d MHz\n", nominal * 100);
            break;
    }

    return 0;
}

/* HSX/BDX define 2 turbo schedules: AVX and non-AVX
 * SKX defines 3 turbo schedules: AVX512, AVX, and non-AVX
 * PLATFORM_INFO bits [34:33] shows how many alternative TDP definitions are
 * defined by default
 *   - 00 only base TDP
 *   - 01 one alternative definition
 *   - 02 two alternative definitions
 *   - 03 reserved
 *
 * CONFIG_TDP_LEVEL1 649H and CONFIG_TDP_LEVEL2 64AH bits [23:16] define the
 * ratio
 */
void get_avx_limits(off_t msr_platform_info, off_t msr_config_tdp_l1,
                    off_t msr_config_tdp_l2, off_t msr_config_tdp_nominal)
{
    static int init = 0;
    static int nsockets = 0;
    static uint64_t **val, **l1, **l2 = NULL;
    int socket;
    uint64_t nominal;

    variorum_get_topology(&nsockets, NULL, NULL);
    if (!init)
    {
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(TDP_DEFS, nsockets);
        load_socket_batch(msr_platform_info, val, TDP_DEFS);
        init = 1;
    }

    /* P0n
     * ...
     * P01
     * P1
     * ...
     * Pn
     * PAVX2
     * PAVX512
     */
    read_batch(TDP_DEFS);

    /* PLATFORM_INFO [34:33] indicate number of configurable TDP levels on
     * Ivy Bridge and later
     */
    int nvalues = (int)(MASK_VAL(*val[0], 34, 33));
    switch (nvalues)
    {
        case 0:
            // Read 648h = normal P1
            /// @todo Should we be reading from PLATFORM_INFO or CONFIG_TDP_NOMINAL 0x648?
            printf("Non-AVX = %d MHz\n", get_max_non_turbo_ratio(msr_platform_info) * 100);
            break;
        case 1:
        case 2:
            config_tdp(nvalues, msr_config_tdp_l1, msr_config_tdp_l2,
                       msr_config_tdp_nominal);
            break;
        case 3:
            // Reserved
            break;
    }
}

/// For socket level
int set_turbo_on(off_t msr_misc_enable, unsigned int turbo_mode_disable_bit)
{
    int ret = 0;
    int socket;
    int nsockets;
    uint64_t mask = 0;
    uint64_t msr_val = 0;

    variorum_get_topology(&nsockets, NULL, NULL);
    /// Creates mask for turbo disable bit according to the architecture offset
    /// given.
    mask |= 1LL << turbo_mode_disable_bit;

    for (socket = 0; socket < nsockets; socket++)
    {
        ret = read_msr_by_coord(socket, 0, 0, msr_misc_enable, &msr_val);
        if (ret)
        {
            variorum_error_handler("Error reading turbo MSR", VARIORUM_ERROR_MSR_READ,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
            return ret;
        }

        /// Does the msr value have the turbo disable bit on?
        if (msr_val == (msr_val | mask))
        {
            uint64_t write_val = msr_val & ~mask;
            //fprintf(stdout, "Socket %i: Turning Turbo on\n", socket);
            //fprintf(stdout, "Value trying to be written to MSR %i: 0x%lx\n", msr_misc_enable, write_val);
            ret = write_msr_by_coord(socket, 0, 0, msr_misc_enable, write_val);
        }
        if (ret)
        {
            variorum_error_handler("Error writing turbo MSR", VARIORUM_ERROR_MSR_WRITE,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
            return ret;
        }
        else
        {
            fprintf(stdout, "Turbo enabled on socket %d\n", socket);
        }
    }
    return ret;
}

int set_turbo_off(off_t msr_misc_enable, unsigned int turbo_mode_disable_bit)
{
    int ret = 0;
    int socket;
    int nsockets;
    uint64_t mask = 0;
    uint64_t msr_val = 0;

    variorum_get_topology(&nsockets, NULL, NULL);
    /// Creates mask for turbo disable bit according to the architecture offset
    /// given.
    mask |= 1LL << turbo_mode_disable_bit;

    for (socket = 0; socket < nsockets; socket++)
    {
        ret = read_msr_by_coord(socket, 0, 0, msr_misc_enable, &msr_val);
        if (ret)
        {
            variorum_error_handler("Error reading turbo MSR", VARIORUM_ERROR_MSR_READ,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
            return ret;
        }

        /// Does the msr value have the turbo disable bit off?
        if (msr_val != (msr_val | mask))
        {
            uint64_t write_val = msr_val | mask;
            //fprintf(stdout, "Socket %i: Turning Turbo off\n", socket);
            //fprintf(stdout, "Value trying to be written to MSR %i: 0x%lx\n", msr_misc_enable, write_val);
            ret = write_msr_by_coord(socket, 0, 0, msr_misc_enable, write_val);
        }
        if (ret)
        {
            variorum_error_handler("Error writing turbo MSR", VARIORUM_ERROR_MSR_WRITE,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
            return ret;
        }
        else
        {
            fprintf(stdout, "Turbo disabled on socket %d\n", socket);
        }
    }
    return ret;
}

int dump_turbo_status(FILE *writedest, off_t msr_misc_enable,
                      unsigned int turbo_mode_disable_bit)
{
    int ret = 0;
    int socket;
    int nsockets;
    uint64_t mask = 0;
    uint64_t msr_val = 0;

    variorum_get_topology(&nsockets, NULL, NULL);
    mask |= 1LL << turbo_mode_disable_bit;

    for (socket = 0; socket < nsockets; socket++)
    {
        ret = read_msr_by_coord(socket, 0, 0, msr_misc_enable, &msr_val);
        if (ret)
        {
            variorum_error_handler("Could not read MSR_MISC_ENABLE",
                                   VARIORUM_ERROR_MSR_READ, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
            return ret;
        }
        if (msr_val == (msr_val | mask))
        {
            fprintf(writedest, "Turbo disabled on socket %d.\n", socket);
        }
        else
        {
            fprintf(writedest, "Turbo enabled on socket %d.\n", socket);
        }
    }
    return ret;
}

///// For core level
//int set_turbo_on_core(const unsigned socket, const unsigned core, off_t msr_misc_enable, unsigned int turbo_mode_disable_bit)
//{
//  int ret = 0;
//  uint64_t mask = 0;
//  uint64_t msr_val = 0;
//  /// Creates mask for turbo disable bit according to the architecture offset given.
//  mask |= 1LL << turbo_mode_disable_bit;
//  ret = read_msr_by_coord(socket, 0, 0, msr_misc_enable, &msr_val);
//
//  /// Does the msr value have the Turbo disable bit on?
//  if (msr_val == (msr_val | mask))
//  {
//      uint64_t write_val = msr_val & ~mask;
//      //fprintf(stdout, "Socket %i: Turning Turbo on\n", socket);
//      //fprintf(stdout, "Value trying to be written to MSR %i: 0x%lx\n", msr_misc_enable, write_val);
//      ret += write_msr_by_coord(socket, 0, 0, msr_misc_enable, write_val);
//  }
//  if (ret)
//  {
//      fprintf(stderr, "Socket %i: Error Reading/Writing Turbo MSR\n", socket);
//  }
//  else
//  {
//      fprintf(stdout, "Socket %i: Turbo Enabled\n", socket);
//  }
//  return ret;
//}
//
//int set_turbo_off_core(const unsigned socket, const unsigned core, off_t msr_misc_enable, unsigned int turbo_mode_disable_bit)
//{
//  int ret = 0;
//  uint64_t mask = 0;
//  uint64_t msr_val = 0;
//    /// Creates mask for turbo disable bit according to the architecture offset
//    /// given.
//  //fprintf(stdout, "MSR Misc Enable value: %lx\n", msr_misc_enable);
//  mask |= 1LL << turbo_mode_disable_bit;
//  ret = read_msr_by_coord(socket, 0, 0, msr_misc_enable, &msr_val);
//
//  /// Does the msr value have the Turbo disable bit off?
//  if (msr_val != (msr_val | mask))
//  {
//      uint64_t write_val = msr_val | mask;
//      //fprintf(stdout, "Socket %i: Turning Turbo off\n", socket);
//      //fprintf(stdout, "Value trying to be written to MSR %i: 0x%lx\n", msr_misc_enable, write_val);
//      ret += write_msr_by_coord(socket, 0, 0, msr_misc_enable, write_val);
//  }
//  if (ret)
//  {
//      fprintf(stderr, "Socket %i: Error Reading/Writing Turbo MSR\n", socket);
//  }
//  else
//  {
//      fprintf(stdout, "Socket %i: Turbo Disabled\n", socket);
//  }
//  return ret;
//}
//
//int get_turbo_status_core(const unsigned socket, const unsigned core, off_t msr_misc_enable, unsigned int turbo_mode_disable_bit)
//{
//    /// Function returns:
//    /// 1 if Turbo is enabled
//    /// 0 if Turbo is DISabled
//    /// -1 if there was an error
//    int ret = 0;
//    uint64_t mask = 0;
//    uint64_t msr_val = 0;
//
//    mask |= 1LL << turbo_mode_disable_bit;
//    ret = read_msr_by_coord(socket, core, 0, msr_misc_enable, &msr_val);
//    if (ret)
//    {
//        fprintf(stderr, "Socket %d, Core %d: ERROR! Could not read msr 0x%lx", socket, core, msr_misc_enable);
//        return -1;
//    }
//    if (msr_val == (msr_val | mask))
//    {
//        //fprintf(stdout, "Socket %i, Core %d: Turbo is DISabled", socket, core);
//        return 0;
//    }
//    else
//    {
//        //fprintf(stdout, "Socket %i, Core %d: Turbo is enabled", socket, core);
//        return 1;
//    }
//}
