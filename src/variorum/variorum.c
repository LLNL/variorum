// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <hwloc.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#include <config_architecture.h>
#include <variorum.h>
#include <variorum_error.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MEM_FILE "/proc/meminfo"
#define CPU_FILE "/proc/stat"

uint64_t lastSum = 0, lastUserTime = 0, lastMemFree = 0, lastMemTotal = 0,
         lastSysTime = 0, lastIdle = 0;
int state = 0;
long prevTime = 0;
long prevMem = 0;
int g_socket;
int g_core;
FILE *fp = 0;
static void print_children(hwloc_topology_t topology, hwloc_obj_t obj,
                           int depth)
{
    unsigned i;

    if (depth == hwloc_get_type_depth(topology, HWLOC_OBJ_SOCKET))
    {
        g_socket = obj->os_index;
    }
    if (depth == hwloc_get_type_depth(topology, HWLOC_OBJ_CORE))
    {
        g_core = obj->logical_index;
    }
    if (depth == hwloc_get_type_depth(topology, HWLOC_OBJ_PU))
    {
        printf("%3u %6u %8u %4u\n", obj->logical_index, obj->os_index, g_core,
               g_socket);
    }
    for (i = 0; i < obj->arity; i++)
    {
        print_children(topology, obj->children[i], depth + 1);
    }
}

int variorum_tester(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_poll_power(FILE *output)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_poll_power == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_poll_power(output);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_monitoring(FILE *output)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_monitoring == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return 0;
        }
        err = g_platform[i].variorum_monitoring(output);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_power_limit(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_power_limit == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_power_limit(0);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_power_limit(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_power_limit == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_power_limit(1);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

void variorum_print_topology(void)
{
    hwloc_topology_t topo;
    int i;

    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        hwloc_topology_init(&topo);
        hwloc_topology_load(topo);

        variorum_get_topology(NULL, NULL, NULL, i);

        fprintf(stdout, "=================\n");
        fprintf(stdout, "Platform Topology\n");
        fprintf(stdout, "=================\n");
        fprintf(stdout, "  Hostname            : %s\n", g_platform[i].hostname);
        fprintf(stdout, "  Num Sockets         : %d\n", g_platform[i].num_sockets);
        fprintf(stdout, "  Num Cores per Socket: %d\n",
                g_platform[i].num_cores_per_socket);
        fprintf(stdout, "  Num Threads per Core: %d\n",
                g_platform[i].num_threads_per_core);
        if (g_platform[i].num_threads_per_core == 1)
        {
            fprintf(stdout, "    Hyperthreading    : No\n");
        }
        else
        {
            fprintf(stdout, "    Hyperthreading    : Yes\n");
        }
        fprintf(stdout, "\n");
        fprintf(stdout, "  Total Num of Cores  : %d\n", g_platform[i].total_cores);
        fprintf(stdout, "  Total Num of Threads: %d\n", g_platform[i].total_threads);
        fprintf(stdout, "\n");
        fprintf(stdout, "Layout:\n");
        fprintf(stdout, "-------\n");
        fprintf(stdout, "Thread HWThread Core Socket\n");
        print_children(topo, hwloc_get_root_obj(topo), 0);

        hwloc_topology_destroy(topo);
    }

    return;
}

// The best effort node power limit is a node-level API, and cannot be implemented
// at a per-platform level. This can only be set by what we define as the
// 'primary' platform, e.g. IBM Power9 CPU or Intel and AMD CPUs, and internally,
// the implementation can determine how much power to allocate to CPU, GPUs and
// memory where applicable. Current implementation is basic, and in the future,
// we will update this API to do something reasonable at the node-level for
// integration with tools such as Flux or SLURM.

int variorum_cap_best_effort_node_power_limit(int node_power_limit)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }

    // Obtain the index corresponding to the primary platform.
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
#ifdef VARIORUM_WITH_INTEL_CPU
        i = P_INTEL_CPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_IBM_CPU
        i = P_IBM_CPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_AMD_CPU
        i = P_AMD_CPU_IDX;
        break;
#endif
    }

    if (g_platform[i].variorum_cap_best_effort_node_power_limit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform[i].variorum_cap_best_effort_node_power_limit(
              node_power_limit);
    if (err)
    {
        return -1;
    }

    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_cap_gpu_power_ratio(int gpu_power_ratio)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_cap_gpu_power_ratio == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return 0;
        }
        err = g_platform[i].variorum_cap_gpu_power_ratio(gpu_power_ratio);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_cap_each_socket_power_limit(int socket_power_limit)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_cap_each_socket_power_limit == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return 0;
        }
        err = g_platform[i].variorum_cap_each_socket_power_limit(socket_power_limit);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_cap_each_core_frequency_limit(int core_freq_mhz)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_cap_each_core_frequency_limit == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return 0;
        }
        err = g_platform[i].variorum_cap_each_core_frequency_limit(core_freq_mhz);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_cap_socket_frequency_limit(int socketid, int socket_freq_mhz)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_cap_socket_frequency_limit == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return -1;
        }
        err = g_platform[i].variorum_cap_socket_frequency_limit(socketid,
                socket_freq_mhz);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_cap_each_gpu_power_limit(int gpu_power_limit)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_cap_each_gpu_power_limit == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return 0;
        }
        err = g_platform[i].variorum_cap_each_gpu_power_limit(gpu_power_limit);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_features(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_features == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_features();
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_thermals(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_thermals == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_thermals(0);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_thermals(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_thermals == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_thermals(1);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_counters(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_counters == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_counters(0);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_counters(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_counters == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_counters(1);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_frequency(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_frequency == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_frequency(0);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_frequency(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_frequency == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_frequency(1);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_power(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_power == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_power(0);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_power(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_power == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_power(1);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_hyperthreading(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        int hyperthreading = (g_platform[i].num_threads_per_core == 1) ? 0 : 1;
        if (hyperthreading == 1)
        {
            fprintf(stdout, "  Hyperthreading:       Enabled\n");
            fprintf(stdout, "  Num Thread Per Core:  %d\n",
                    g_platform[i].num_threads_per_core);
        }
        else
        {
            fprintf(stdout, "  Hyperthreading:       Disabled\n");
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_turbo(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_turbo == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_turbo();
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_gpu_utilization(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_gpu_utilization == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_gpu_utilization(0);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_gpu_utilization(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_gpu_utilization == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            continue;
        }
        err = g_platform[i].variorum_print_gpu_utilization(1);
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_enable_turbo(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_enable_turbo == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return 0;
        }
        err = g_platform[i].variorum_enable_turbo();
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_disable_turbo(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_disable_turbo == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                                   getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return 0;
        }
        err = g_platform[i].variorum_disable_turbo();
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

// The variorum_get_node_power_json is a node-level API, and cannot be implemented
// at a per-component (eg CPU, GPU) level. This can only be captured by what we
// define as the 'primary' platform, e.g. IBM Power9 CPU or Intel and AMD CPUs,
// and internally, the implementation can update power usage of CPU, GPUs and
// memory where applicable. Current implementation is basic, and in the future,
// we will update this API to do obtain and return the GPU power usage using a new
// JSON API for GPU power.

int variorum_get_node_power_json(char **get_power_obj_str)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }

    // Obtain the index corresponding to the primary platform.
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
#ifdef VARIORUM_WITH_INTEL_CPU
        i = P_INTEL_CPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_IBM_CPU
        i = P_IBM_CPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_AMD_CPU
        i = P_AMD_CPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_ARM_CPU
        i = P_ARM_CPU_IDX;
        break;
#endif
    }

    if (g_platform[i].variorum_get_node_power_json == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        // For the JSON functions, we return a -1 here, so users don't need
        // to explicitly check for NULL strings.
        return -1;
    }

    err = g_platform[i].variorum_get_node_power_json(get_power_obj_str);
    if (err)
    {
        return -1;
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_get_node_utilization_json(char **get_util_obj_str)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }

    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    char hostname[1024];
    struct timeval tv;
    uint64_t ts;
    json_t *get_util_obj = json_object();

    gethostname(hostname, 1024);
    gettimeofday(&tv, NULL);
    ts = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
    json_object_set_new(get_util_obj, "host", json_string(hostname));
    json_object_set_new(get_util_obj, "timestamp", json_integer(ts));

    char str[100];
    const char d[2] = " ";
    char *token, *s, *p;
    int i = 0;
    uint64_t sum = 0, idle = 0, userTime = 0, niceTime = 0, sumUserTime = 0,
             iowait = 0;
    double cpuUtil = 0.0, userUtil = 0.0, sysUtil = 0.0, memUtil = 0.0;
    int rc, j;
    char lbuf[256];
    char metric_name[256];
    uint64_t metric_value;
    uint64_t memTotal = 0, memFree = 0, sysTime = 0;
    int strcp;

    fp = fopen(CPU_FILE, "r");
    if (fp == NULL)
    {
        return -1;
    }
    if (fgets(str, 100, fp) == NULL)
    {
        return -1;
    }
    if (str != NULL)
    {
        token = strtok(str, d);
        sum = 0;
        while (token != NULL)
        {
            token = strtok(NULL, d);
            if (token != NULL)
            {
                sum += strtol(token, &p, 10);
                if (i == 3)
                {
                    idle = strtol(token, &p, 10);
                    //break;
                }
                if (i == 0)
                {
                    userTime = strtol(token, &p, 10);
                }
                if (i == 1)
                {
                    niceTime = strtol(token, &p, 10);
                }
                if (i == 2)
                {
                    sysTime = strtol(token, &p, 10);
                }
                if (i == 4)
                {
                    iowait = strtol(token, &p, 10);
                }
                idle = idle + iowait;
                sumUserTime = userTime + niceTime;
                i++;
            }
        }
    }

    fclose(fp);
    // make the utilization metrics 0 at the first sample
    if (state)
    {
        userUtil = ((sumUserTime - lastUserTime) / (double)(sum - lastSum)) * 100;
        sysUtil = ((sysTime - lastSysTime) / (double)(sum - lastSum)) * 100;
        cpuUtil = (1 - ((idle - lastIdle) / (double)(sum - lastSum))) * 100;

    }
    else
    {
        userUtil = 0.0;
        sysUtil = 0.0;
        cpuUtil = 0.0;
    }

    lastUserTime = sumUserTime;
    lastSum = sum;
    lastSysTime = sysTime;
    lastIdle = idle;
    json_object_set_new(get_util_obj, "cpu util", json_real(cpuUtil));
    json_object_set_new(get_util_obj, "user util", json_real(userUtil));
    json_object_set_new(get_util_obj, "system util", json_real(sysUtil));
    fp = fopen(MEM_FILE, "r");
    if (fp == NULL)
    {
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    do
    {
        s = fgets(lbuf, sizeof(lbuf), fp);
        if (!s)
            break;

        rc = sscanf(lbuf, "%s%" PRIu64, metric_name, &metric_value);
        if (rc < 2)
            break;

        /* Strip the colon from metric name if present */
        j = strlen(metric_name);
        if (i && metric_name[j - 1] == ':')
        {
            metric_name[j - 1] = '\0';
        }
        strcp = strcmp(metric_name, "MemTotal");
        if (strcp == 0)
        {
            memTotal = metric_value;
        }
        strcp = strcmp(metric_name, "MemFree");
        if (strcp == 0)
        {
            memFree = metric_value;
        }
    }
    while (s);

    if (state)
    {
        memUtil = (1 - (double)(memFree) / (memTotal)) * 100;
    }
    else
    {
        memUtil = 0.0;
    }

    fclose(fp);

    json_object_set_new(get_util_obj, "memory util", json_real(memUtil));
    variorum_get_gpu_utilization_json(get_util_obj);
    //variorum_get_gpu_utilization_json(get_util_obj);
    *get_util_obj_str = json_dumps(get_util_obj, JSON_INDENT(4));
    json_decref(get_util_obj);
    state = 1;
    return 0;

}

int variorum_get_gpu_utilization_json(json_t
                                      *get_gpu_util_obj)//char** get_gpu_util_obj_str)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }

    //json_t *get_gpu_util_obj = json_object();
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
#ifdef VARIORUM_WITH_INTEL_GPU
        i = P_INTEL_GPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_NVIDIA_GPU
        i = P_NVIDIA_GPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_AMD_GPU
        i = P_AMD_GPU_IDX;
        break;
#endif
    }

    if (g_platform[i].variorum_get_gpu_utilization_json == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
    }
    err = g_platform[i].variorum_get_gpu_utilization_json(get_gpu_util_obj);
    if (err)
    {
        return -1;
    }


    //*get_gpu_util_obj_str = json_dumps(get_gpu_util_obj, JSON_INDENT(4));
    //json_decref(get_gpu_util_obj);

    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

// The variorum_get_node_power_domain_info_json is a node-level API, and cannot
// be implemented at a per-component (eg CPU, GPU) level. This can only be available
// on what we define as the 'primary' platform, e.g. IBM Power9 CPU or Intel and AMD CPUs,
// and internally, the implementation can update domain information of CPU, GPUs and
// memory where applicable. Current implementation is basic, and in the future,
// we will update this API to do obtain and return the GPU domain info using a new
// JSON API for GPU power domain info.

int variorum_get_node_power_domain_info_json(char **get_domain_obj_str)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }

    // Obtain the index corresponding to the primary platform.
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
#ifdef VARIORUM_WITH_INTEL_CPU
        i = P_INTEL_CPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_IBM_CPU
        i = P_IBM_CPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_AMD_CPU
        i = P_AMD_CPU_IDX;
        break;
#endif
#ifdef VARIORUM_WITH_ARM_CPU
        i = P_ARM_CPU_IDX;
        break;
#endif
    }

    if (g_platform[i].variorum_get_node_power_domain_info_json == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        // For the JSON functions, we return a -1 here, so users don't need
        // to explicitly check for NULL strings.
        return -1;
    }
    err = g_platform[i].variorum_get_node_power_domain_info_json(
              get_domain_obj_str);
    if (err)
    {
        return -1;
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_available_frequencies(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_available_frequencies == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED, getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return 0;
        }
        err = g_platform[i].variorum_print_available_frequencies();
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_energy(void)
{
    int err = 0;
    int i;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    for (i = 0; i < P_NUM_PLATFORMS; i++)
    {
        if (g_platform[i].variorum_print_energy == NULL)
        {
            variorum_error_handler("Feature not yet implemented or is not supported",
                                   VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED, getenv("HOSTNAME"), __FILE__,
                                   __FUNCTION__, __LINE__);
            return 0;
        }
        err = g_platform[i].variorum_print_energy();
        if (err)
        {
            return -1;
        }
    }
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

char *variorum_get_current_version()
{
    return QuoteMacro(VARIORUM_VERSION);
}
