// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hwloc.h>
#include <assert.h>

#include <config_architecture.h>
#include <variorum_config.h>
#include <variorum_error.h>

#ifdef VARIORUM_WITH_INTEL
#include <config_intel.h>
#include <msr_core.h>
#endif

#ifdef VARIORUM_WITH_IBM
#include <config_ibm.h>
#endif

#ifdef VARIORUM_WITH_NVIDIA
#include <config_nvidia.h>
#endif

#ifdef VARIORUM_WITH_ARM
#include <config_arm.h>
#endif

#ifdef VARIORUM_LOG
int variorum_enter(const char *filename, const char *func_name, int line_num)
#else
int variorum_enter()
#endif
{
    int err = 0;
    int i;
#ifdef VARIORUM_LOG
    printf("_LOG_VARIORUM_ENTER:%s:%s::%d\n", filename, func_name, line_num);
#endif

    printf("Number of registered platforms: %d\n", P_NUM_PLATFORMS);

    variorum_init_func_ptrs();

    //Triggers initialization on first call.  Errors assert.
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        variorum_get_topology(NULL, NULL, NULL, i);
    }
    err = variorum_detect_arch();
    if (err)
    {
        variorum_error_handler("Cannot detect architecture", err,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
        return err;
    }
    err = variorum_set_func_ptrs();
    if (err)
    {
        variorum_error_handler("Cannot set function pointers", err,
                               getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                               __LINE__);
        return err;
    }
    return err;
}

#ifdef VARIORUM_LOG
int variorum_exit(const char *filename, const char *func_name, int line_num)
#else
int variorum_exit()
#endif
{
    int err = 0;
    int i;
#ifdef VARIORUM_LOG
    printf("_LOG_VARIORUM_EXIT:%s:%s::%d\n", filename, func_name, line_num);
#endif

#ifdef VARIORUM_WITH_INTEL
    err = finalize_msr();
    if (err)
    {
        return err;
    }
#endif

    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        free(g_platform[i].arch_id);
    }

    return err;
}

int variorum_detect_arch(void)
{
    int i;
#ifdef VARIORUM_WITH_INTEL
    g_platform[P_INTEL_IDX].arch_id = detect_intel_arch();
    printf("Intel -- idx%d\n", P_INTEL_IDX);
#endif
#ifdef VARIORUM_WITH_AMD
    //g_platform[P_AMD_IDX].arch_id = detect_amd_arch();
    //printf("AMD -- idx%d\n", idx);
#endif
#ifdef VARIORUM_WITH_IBM
    g_platform[P_IBM_IDX].arch_id = detect_ibm_arch();
    printf("IBM -- idx%d\n", P_IBM_IDX);
#endif
#ifdef VARIORUM_WITH_NVIDIA
    g_platform[P_NVIDIA_IDX].arch_id = detect_gpu_arch();
    printf("Nvidia -- idx%d\n", P_NVIDIA_IDX);
#endif
#ifdef VARIORUM_WITH_ARM
    g_platform.arm_arch = detect_arm_arch();
    printf("ARM -- idx%d\n", P_ARM_IDX);
#endif

    // @todo Need to know what idx maps to which architecture
#if defined(VARIORUM_LOG) && defined(VARIORUM_WITH_INTEL)
    printf("Intel Model: 0x%lx\n", *g_platform[P_INTEL_IDX].arch_id);
#endif
#if defined(VARIORUM_LOG) && defined(VARIORUM_WITH_IBM)
    printf("IBM Model: 0x%lx\n", *g_platform[P_IBM_IDX].arch_id);
#endif
#if defined(VARIORUM_LOG) && defined(VARIORUM_WITH_NVIDIA)
    printf("Nvidia Model: 0x%lx\n", *g_platform[P_NVIDIA_IDX].arch_id);
#endif
#if defined(VARIORUM_LOG) && defined(VARIORUM_WITH_ARM)
    printf("Arm Model: 0x%lx\n", *g_platform[P_ARM_IDX].arch_id);
#endif

    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].arch_id == NULL)
        {
            variorum_error_handler("No architectures detected", VARIORUM_ERROR_RUNTIME,
                                   getenv("HOSTNAME"), __FILE__, __FUNCTION__,
                                   __LINE__);
            return VARIORUM_ERROR_UNSUPPORTED_ARCH;
        }
    }

    return 0;
}


void variorum_get_topology(int *nsockets, int *ncores, int *nthreads, int idx)
{
    hwloc_topology_t topology;
    int rc;
    static int init_variorum_get_topology = 0;

    gethostname(g_platform[idx].hostname, 1024);

    if (!init_variorum_get_topology)
    {
        //init_variorum_get_topology = 1;

        // hwloc should give us expected results on any reasonable arch.
        // If something goes wrong, there's no sense in trying to keep
        // marching forward.  Thus the asserts.
        rc = hwloc_topology_init(&topology);
        if (rc != 0)
        {
            exit(-1);
        }
        rc = hwloc_topology_load(topology);
        if (rc != 0)
        {
            exit(-1);
        }

        g_platform[idx].num_sockets = hwloc_get_nbobjs_by_type(topology,
                                      HWLOC_OBJ_SOCKET);
        //-1 if Several levels exist with OBJ_SOCKET
        if (g_platform[idx].num_sockets == -1)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc reports that HWLOC_OBJ_SOCKETs exist "
                    "at multiple levels of the topology DAG.  "
                    "Variorum doesn't handle this case.  "
                    "Exiting.", __FILE__, __LINE__);
            exit(-1);
        }
        // 0 if No levels exist with OBJ_SOCKET
        if (g_platform[idx].num_sockets == 0)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc reports no HWLOC_OBJ_SOCKETs exist.  "
                    "Variorum doesn't handle this case.  "
                    "Exiting.", __FILE__, __LINE__);
            exit(-1);
        }

        g_platform[idx].total_cores = hwloc_get_nbobjs_by_type(topology,
                                      HWLOC_OBJ_CORE);
        if (g_platform[idx].total_cores == -1)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc reports HWLOC_OJB_COREs exist "
                    "at multiple levels of the topology DAG.  "
                    "Variorum doesn't handle this case.  "
                    "Exiting.", __FILE__, __LINE__);
            exit(-1);
        }
        if (g_platform[idx].total_cores == 0)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc reports no HWLOC_OBJ_COREs exist."
                    "Variorum doesn't handle this case."
                    "Exiting.", __FILE__, __LINE__);
            exit(-1);
        }

        g_platform[idx].total_threads = hwloc_get_nbobjs_by_type(topology,
                                        HWLOC_OBJ_PU);
        if (g_platform[idx].total_threads == -1)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc reports that HWLOC_OBJ_PUs exist "
                    "at multiple levels of the topology DAG.  "
                    "Variorum doesn't handle this case."
                    "Exiting.", __FILE__, __LINE__);
            exit(-1);
        }
        if (g_platform[idx].total_threads == 0)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc reports no HWLOC_OBJ_COREs exist.  "
                    "Variorum doesn't handle this case.  "
                    "Exiting.", __FILE__, __LINE__);
            exit(-1);
        }

        g_platform[idx].num_cores_per_socket = g_platform[idx].total_cores /
                                               g_platform[idx].num_sockets;
        if (g_platform[idx].total_cores % g_platform[idx].num_sockets != 0)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc reports the number of cores (%d) mod "
                    "the number of sockets (%d) is not zero.  "
                    "Something is amiss.  Exiting.",
                    __FILE__, __LINE__,
                    g_platform[idx].total_cores,
                    g_platform[idx].num_sockets);
            exit(-1);
        }

        g_platform[idx].num_threads_per_core = g_platform[idx].total_threads /
                                               g_platform[idx].total_cores;
        if (g_platform[idx].total_threads % g_platform[idx].total_cores != 0)
        {
            fprintf(stderr, "%s:%d "
                    "hwloc reports the number of threads (%d) mod "
                    "the number of cores (%d) is not zero.  "
                    "Something is amiss.  Exiting.",
                    __FILE__, __LINE__,
                    g_platform[idx].total_threads,
                    g_platform[idx].total_cores);
            exit(-1);
        }

        hwloc_topology_destroy(topology);
    }

    if (nsockets != NULL)
    {
        *nsockets = g_platform[idx].num_sockets;
    }

    if (ncores != NULL)
    {
        *ncores = g_platform[idx].total_cores;
    }

    if (nthreads != NULL)
    {
        *nthreads = g_platform[idx].total_threads;
    }
}

void variorum_init_func_ptrs()
{
    int i = 0;
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        g_platform[i].variorum_print_power_limits = NULL;
        g_platform[i].variorum_cap_best_effort_node_power_limit = NULL;
        g_platform[i].variorum_cap_and_verify_node_power_limit = NULL;
        g_platform[i].variorum_cap_gpu_power_ratio = NULL;
        g_platform[i].variorum_cap_each_socket_power_limit = NULL;
        g_platform[i].variorum_print_features = NULL;
        g_platform[i].variorum_print_thermals = NULL;
        g_platform[i].variorum_print_counters = NULL;
        g_platform[i].variorum_print_clocks = NULL;
        g_platform[i].variorum_print_power = NULL;
        g_platform[i].variorum_enable_turbo = NULL;
        g_platform[i].variorum_disable_turbo = NULL;
        g_platform[i].variorum_print_turbo = NULL;
        g_platform[i].variorum_poll_power = NULL;
        g_platform[i].variorum_print_gpu_utilization = NULL;
        g_platform[i].variorum_cap_each_core_frequency = NULL;
        g_platform[i].variorum_monitoring = NULL;
        g_platform[i].variorum_get_node_power_json = NULL;
    }
}

int variorum_set_func_ptrs()
{
    int err = 0;

#ifdef VARIORUM_WITH_INTEL
    err = set_intel_func_ptrs(P_INTEL_IDX);
    if (err)
    {
        return err;
    }
    err = init_msr();
#endif
#ifdef VARIORUM_WITH_IBM
    err = set_ibm_func_ptrs(P_IBM_IDX);
#endif
#ifdef VARIORUM_WITH_NVIDIA
    err = set_nvidia_func_ptrs(P_NVIDIA_IDX);
    if (err)
    {
        return err;
    }
#endif
#ifdef VARIORUM_WITH_ARM
    err = set_arm_func_ptrs(P_ARM_IDX);
#endif
    return err;
}

////setfixedcounters = fixed_ctr0,
////featureC = ?? /* does not have max non turbo ratio */
//}
//};

#if 0 /* To implement later */
///* Sandy Bridge: 06_2D, 06_2A */
//{
//    dump_power_limits = therm_status
//                        //setfixedcounters = fixed_ctr0,
//                        //setpowerlimit = perf_status
//                        //featureC = ?? /* does not have max non turbo ratio */
//}
///* Haswell, Haswell-E: 06_3C, 06_45, 06_46, 06_3F */
//{
//    getfixedcounters = max_non_turbo_ratio;
//},
///* Broadwell: 06_3D, 06_47, 06_4F, 06_56 */
//{
//    getfixedcounters = max_non_turbo_ratio;
//}
///* Skylake: 06_4E, 06_5E */
//{
//}
///* Kaby Lake: 06_8E, 06_9E */
//{
//}
///* Xeon Phi: 06_57 */
//{
//}
#endif

#if 0 /* Do not want to make 95% of users know how to use this */
//struct *platform make_arch(enum arch_e arch)
//{
//    struct platform *platform_ptr;
//    if (arch == SANDY_BRIDGE)
//    {
//        platform_ptr = &allplatforms[0];
//    }
//    else if (arch == IVY_BRIDGE)
//    {
//        platform_ptr = &allplatforms[1];
//    }
//    else if (arch == HASWELL)
//    {
//        platform_ptr = &allplatforms[2];
//    }
//    else if (arch == BROADWELL)
//    {
//        platform_ptr = &allplatforms[3];
//    }
//
//    return platform_ptr;
//};
//
///// Front-facing APIs
//struct platform *platform_ptr = make_arch(HASWELL);
#endif
