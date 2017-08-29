#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hwloc.h>

#include <config_architecture.h>
#include <variorum_error.h>
#include <variorum_config.h>
#ifdef VARIORUM_WITH_INTEL
#include <config_intel.h>
#include <msr_core.h>
#endif

int variorum_enter(const char *filename, const char *func_name, int line_num)
{
    int err = 0;
    printf("_LOG_VARIORUM_ENTER:%s:%s::%d\n", filename, func_name, line_num);

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

    printf("_LOG_VARIORUM_EXIT:%s:%s::%d\n", filename, func_name, line_num);

#ifdef VARIORUM_WITH_INTEL
    err = finalize_msr();
    if (err)
    {
        return err;
    }
#endif

    free(g_platform.map_pu_to_core);

#ifdef VARIORUM_WITH_INTEL
    free(g_platform.intel_arch);
#elif VARIORUM_WITH_AMD
    free(g_platform.amd_arch);
#elif VARIORUM_WITH_IBM
    free(g_platform.ibm_arch);
#elif VARIORUM_WITH_GPU
    free(g_platform.gpu_arch);
#endif

    return err;
}

int variorum_detect_arch(void)
{
#ifdef VARIORUM_WITH_INTEL
    g_platform.intel_arch = detect_intel_arch();
#elif VARIORUM_WITH_AMD
    //g_platform.amd_arch = detect_amd_arch();
#elif VARIORUM_WITH_IBM
    //g_platform.amd_arch = detect_ibm_arch();
#elif VARIORUM_WITH_GPU
    //g_platform.amd_arch = detect_gpu_arch();
#else
    return VARIORUM_ERROR_UNSUPPORTED_ARCH;
#endif

    printf("Intel Model: 0x%lx\n", *g_platform.intel_arch);
    return 0;
}

static void get_children_per_core(hwloc_topology_t topology, hwloc_obj_t obj, int curr_depth, int core_depth, int pu_depth, int log_idx)
{
    unsigned i;
    hwloc_obj_t core_obj;

    core_obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_CORE, log_idx);
    if (curr_depth != core_depth)
    {
        g_platform.map_pu_to_core[obj->os_index].physical_core_idx = core_obj->logical_index;
    }
    for (i = 0; i < obj->arity; i++)
    {
        get_children_per_core(topology, obj->children[i], curr_depth + 1, core_depth, pu_depth, log_idx);
    }
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
        g_platform.map_pu_to_core = (struct map *) malloc(g_platform.total_threads * sizeof(struct map));

        for (i = 0; i < g_platform.total_cores; i++)
        {
            obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_CORE, i);
            get_children_per_core(topology, obj, obj->depth, core_depth, pu_depth, i);
        }

#if 0
        printf("confirm map contents\n");
        for (i = 0; i < g_platform.total_threads; i++)
        {
            printf("idx %d = %d\n", i, g_platform.map_pu_to_core[i].physical_core_idx);
        }
#endif

#if 0
        set = hwloc_topology_get_allowed_cpuset(topology);
        hwloc_bitmap_foreach_begin(i, set)
        {
            obj = hwloc_get_pu_obj_by_os_index(topology, i);
            printf("RRR i=%u logical_index=%u depth=%u sibling_rank=%u os_index=%u arity=%u\n",
                   i, obj->logical_index, obj->depth, obj->sibling_rank, obj->os_index, obj->arity);
        }
        hwloc_bitmap_foreach_end();
#endif

        hwloc_topology_destroy(topology);
    }
    return 0;
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
    if (err)
    {
        return err;
    }
#else
    variorum_error_handler("No architectures detected", VARIORUM_ERROR_RUNTIME, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
    return VARIORUM_ERROR_RUNTIME;
#endif
    return err;
}

#if 0
//setfixedcounters = fixed_ctr0,
//featureC = ?? /* does not have max non turbo ratio */
}
};
#endif

#if 0 /* To implement later */
/* Sandy Bridge: 06_2D, 06_2A */
{
    dump_power_limits = therm_status
                        //setfixedcounters = fixed_ctr0,
                        //setpowerlimit = perf_status
                        //featureC = ?? /* does not have max non turbo ratio */
}
/* Haswell, Haswell-E: 06_3C, 06_45, 06_46, 06_3F */
{
    getfixedcounters = max_non_turbo_ratio;
},
/* Broadwell: 06_3D, 06_47, 06_4F, 06_56 */
{
    getfixedcounters = max_non_turbo_ratio;
}
/* Skylake: 06_4E, 06_5E */
{
}
/* Kaby Lake: 06_8E, 06_9E */
{
}
/* Xeon Phi: 06_57 */
{
}
#endif

#if 0 /* Do not want to make 95% of users know how to use this */
struct *platform make_arch(enum arch_e arch)
{
    struct platform *platform_ptr;
    if (arch == SANDY_BRIDGE)
    {
        platform_ptr = &allplatforms[0];
    }
    else if (arch == IVY_BRIDGE)
    {
        platform_ptr = &allplatforms[1];
    }
    else if (arch == HASWELL)
    {
        platform_ptr = &allplatforms[2];
    }
    else if (arch == BROADWELL)
    {
        platform_ptr = &allplatforms[3];
    }

    return platform_ptr;
};

/// Front-facing APIs
struct platform *platform_ptr = make_arch(HASWELL);
#endif
