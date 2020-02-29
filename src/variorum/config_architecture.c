// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hwloc.h>

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

int variorum_enter(const char *filename, const char *func_name, int line_num)
{
    int err = 0;
#ifdef VARIORUM_LOG
    printf("_LOG_VARIORUM_ENTER:%s:%s::%d\n", filename, func_name, line_num);
#endif

    variorum_init_func_ptrs();

    err = variorum_get_topology();
    if (err)
    {
        variorum_error_handler("Cannot get topology", err, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return err;
    }
    err = variorum_detect_arch();
    if (err)
    {
        variorum_error_handler("Cannot detect architecture", err, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return err;
    }
    err = variorum_set_func_ptrs();
    if (err)
    {
        variorum_error_handler("Cannot set function pointers", err, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return err;
    }
    return err;
}

int variorum_exit(const char *filename, const char *func_name, int line_num)
{
    int err = 0;
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

#ifdef VARIORUM_WITH_INTEL
    free(g_platform.intel_arch);
#endif
#ifdef VARIORUM_WITH_AMD
    free(g_platform.amd_arch);
#endif
#ifdef VARIORUM_WITH_IBM
    free(g_platform.ibm_arch);
#endif
#ifdef VARIORUM_WITH_GPU
    free(g_platform.gpu_arch);
#endif

    return err;
}

int variorum_detect_arch(void)
{
#ifdef VARIORUM_WITH_INTEL
    g_platform.intel_arch = detect_intel_arch();
#endif
#ifdef VARIORUM_WITH_AMD
    //g_platform.amd_arch = detect_amd_arch();
#endif
#ifdef VARIORUM_WITH_IBM
    g_platform.ibm_arch = detect_ibm_arch();
#endif
#ifdef VARIORUM_WITH_GPU
    //g_platform.gpu_arch = detect_gpu_arch();
#endif

#if defined(VARIORUM_LOG) && defined(VARIORUM_WITH_INTEL)
    printf("Intel Model: 0x%lx\n", *g_platform.intel_arch);
#endif
#if defined(VARIORUM_LOG) && defined(VARIORUM_WITH_IBM)
    printf("IBM Model: 0x%lx\n", *g_platform.ibm_arch);
#endif

    if (g_platform.intel_arch == NULL &&
        g_platform.amd_arch   == NULL &&
        g_platform.ibm_arch   == NULL &&
        g_platform.gpu_arch   == NULL)
    {
        variorum_error_handler("No architectures detected", VARIORUM_ERROR_RUNTIME, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return VARIORUM_ERROR_UNSUPPORTED_ARCH;
    }

    return 0;
}

void variorum_set_topology(int *nsockets, int *ncores, int *nthreads)
{
    if (nsockets != NULL)
    {
        *nsockets = g_platform.num_sockets;
    }
    if (ncores != NULL)
    {
        *ncores = g_platform.total_cores;
    }
    if (nthreads != NULL)
    {
        *nthreads = g_platform.total_threads;
    }
}

int variorum_get_topology(void)
{
    hwloc_topology_t topology;
    hwloc_obj_t obj;
    unsigned int i;
    unsigned int core_depth, pu_depth;
    static int init_variorum_get_toplogy = 0;

    gethostname(g_platform.hostname, 1024);

    if (!init_variorum_get_toplogy)
    {
        hwloc_topology_init(&topology);  // initialization
        hwloc_topology_load(topology);   // actual detection

        core_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_CORE);
        pu_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);

        g_platform.num_sockets = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_SOCKET);
        g_platform.total_cores = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_CORE);
        g_platform.total_threads = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PU);

        g_platform.num_cores_per_socket = g_platform.total_cores/g_platform.num_sockets;
        g_platform.num_threads_per_core = g_platform.total_threads/g_platform.total_cores;

        hwloc_topology_destroy(topology);
    }
    return 0;
}

void variorum_init_func_ptrs()
{
    g_platform.dump_power_limits = NULL;
    g_platform.set_node_power_limit = NULL;
    g_platform.set_and_verify_node_power_limit = NULL;
    g_platform.set_gpu_power_ratio = NULL;
    g_platform.set_each_socket_power_limit = NULL;
    g_platform.print_features = NULL;
    g_platform.dump_thermals = NULL;
    g_platform.dump_counters = NULL;
    g_platform.dump_clocks = NULL;
    g_platform.dump_power = NULL;
    g_platform.enable_turbo = NULL;
    g_platform.disable_turbo = NULL;
    g_platform.dump_turbo = NULL;
}

int variorum_set_func_ptrs()
{
    int err = 0;

#ifdef VARIORUM_WITH_INTEL
    err = set_intel_func_ptrs();
    if (err)
    {
        return err;
    }
    err = init_msr();
#endif
#ifdef VARIORUM_WITH_IBM
    err = set_ibm_func_ptrs();
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
