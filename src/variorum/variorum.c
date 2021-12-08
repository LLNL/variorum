// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <hwloc.h>

#include <config_architecture.h>
#include <variorum.h>
#include <variorum_error.h>

int g_socket;
int g_core;

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
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_poll_power(FILE *output)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_poll_power == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_poll_power(output);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_monitoring(FILE *output)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_monitoring == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_monitoring(output);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_power_limit(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_power_limit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_power_limit(0);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_power_limit(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_power_limit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_power_limit(1);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

void variorum_print_topology(void)
{
    hwloc_topology_t topo;

    hwloc_topology_init(&topo);
    hwloc_topology_load(topo);

    variorum_get_topology(NULL, NULL, NULL);

    fprintf(stdout, "=================\n");
    fprintf(stdout, "Platform Topology\n");
    fprintf(stdout, "=================\n");
    fprintf(stdout, "  Hostname            : %s\n", g_platform.hostname);
    fprintf(stdout, "  Num Sockets         : %d\n", g_platform.num_sockets);
    fprintf(stdout, "  Num Cores per Socket: %d\n",
            g_platform.num_cores_per_socket);
    fprintf(stdout, "  Num Threads per Core: %d\n",
            g_platform.num_threads_per_core);
    if (g_platform.num_threads_per_core == 1)
    {
        fprintf(stdout, "    Hyperthreading    : No\n");
    }
    else
    {
        fprintf(stdout, "    Hyperthreading    : Yes\n");
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  Total Num of Cores  : %d\n", g_platform.total_cores);
    fprintf(stdout, "  Total Num of Threads: %d\n", g_platform.total_threads);
    fprintf(stdout, "\n");
    fprintf(stdout, "Layout:\n");
    fprintf(stdout, "-------\n");
    fprintf(stdout, "Thread HWThread Core Socket\n");
    print_children(topo, hwloc_get_root_obj(topo), 0);

    hwloc_topology_destroy(topo);

    return;
}

int variorum_cap_best_effort_node_power_limit(int node_power_limit)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_cap_best_effort_node_power_limit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_cap_best_effort_node_power_limit(node_power_limit);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_cap_gpu_power_ratio(int gpu_power_ratio)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_cap_gpu_power_ratio == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_cap_gpu_power_ratio(gpu_power_ratio);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_cap_each_socket_power_limit(int socket_power_limit)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_cap_each_socket_power_limit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_cap_each_socket_power_limit(socket_power_limit);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_cap_each_core_frequency_limit(int core_freq_mhz)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_cap_each_core_frequency_limit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_cap_each_core_frequency_limit(core_freq_mhz);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_cap_socket_frequency_limit(int socketid, int socket_freq_mhz)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_cap_socket_frequency_limit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.variorum_cap_socket_frequency_limit(socketid, socket_freq_mhz);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_features(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_features == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_features();
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_thermals(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_thermals == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_thermals(0);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_thermals(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_thermals == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_thermals(1);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_counters(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_counters == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_counters(0);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_counters(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_counters == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_counters(1);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_frequency(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_frequency == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_frequency(0);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_frequency(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_frequency == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_frequency(1);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_power(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_power == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_power(0);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_power(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_power == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_power(1);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_hyperthreading(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    int hyperthreading = (g_platform.num_threads_per_core == 1) ? 0 : 1;
    if (hyperthreading == 1)
    {
        fprintf(stdout, "  Hyperthreading:       Enabled\n");
        fprintf(stdout, "  Num Thread Per Core:  %d\n",
                g_platform.num_threads_per_core);
    }
    else
    {
        fprintf(stdout, "  Hyperthreading:       Disabled\n");
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_turbo(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_turbo == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_turbo();
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;

}

int variorum_print_gpu_utilization(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_gpu_utilization == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_gpu_utilization(0);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_verbose_gpu_utilization(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_gpu_utilization == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_gpu_utilization(1);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_enable_turbo(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_enable_turbo == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_enable_turbo();
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_disable_turbo(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_disable_turbo == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_disable_turbo();
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_get_node_power_json(json_t *get_power_obj)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_get_node_power_json == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_get_node_power_json(get_power_obj);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_get_node_power_domain_info_json(json_t *get_domain_obj)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_get_node_power_domain_info_json == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED,
                               getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_get_node_power_domain_info_json(get_domain_obj);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_available_frequencies(void)
{
    int err = 0;
#ifdef VARIORUM_LOG
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_enter();
#endif
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_available_frequencies == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED, getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_available_frequencies();
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_energy(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_energy == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED, getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_energy();
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_print_boostlimit(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_print_boostlimit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED, getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_print_boostlimit();
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_set_and_verify_core_boostlimit(int core, unsigned int boostlimit)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_set_and_verify_core_boostlimit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED, getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_set_and_verify_core_boostlimit(core, boostlimit);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}

int variorum_set_socket_boostlimit(int socket, unsigned int boostlimit)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.variorum_set_socket_boostlimit == NULL)
    {
        variorum_error_handler("Feature not yet implemented or is not supported",
                               VARIORUM_ERROR_FEATURE_NOT_IMPLEMENTED, getenv("HOSTNAME"), __FILE__,
                               __FUNCTION__, __LINE__);
        return 0;
    }
    err = g_platform.variorum_set_socket_boostlimit(socket, boostlimit);
    if (err)
    {
        return -1;
    }
#ifdef VARIORUM_LOG
    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
#else
    err = variorum_exit();
#endif
    if (err)
    {
        return -1;
    }
    return err;
}
