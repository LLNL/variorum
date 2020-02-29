// Copyright 2019 Lawrence Livermore National Security, LLC and other
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

static void print_children(hwloc_topology_t topology, hwloc_obj_t obj, int depth)
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
        printf("%3u %6u %8u %4u\n", obj->logical_index, obj->os_index, g_core, g_socket);
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
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    err = g_platform.poll_power(output);
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

int variorum_monitoring(FILE *output)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    err = g_platform.monitoring(output);
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

int variorum_print_power_limits(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_power_limits == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_power_limits(0);
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

int variorum_print_verbose_power_limits(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_power_limits == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_power_limits(1);
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

int variorum_print_topology(void)
{
    int err = 0;
    int i;
    int hyperthreading = 0;
    hwloc_topology_t topo;

    hwloc_topology_init(&topo);
    hwloc_topology_load(topo);

    err = variorum_get_topology();
    if (err)
    {
        variorum_error_handler("Cannot get topology", err, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    fprintf(stdout, "=================\n");
    fprintf(stdout, "Platform Topology\n");
    fprintf(stdout, "=================\n");
    fprintf(stdout, "  Hostname            : %s\n", g_platform.hostname);
    fprintf(stdout, "  Num Sockets         : %d\n", g_platform.num_sockets);
    fprintf(stdout, "  Num Cores per Socket: %d\n", g_platform.num_cores_per_socket);
    fprintf(stdout, "  Num Threads per Core: %d\n", g_platform.num_threads_per_core);
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

    return err;
}

int variorum_set_node_power_limit(int node_power_limit)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.set_node_power_limit == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.set_node_power_limit(node_power_limit);
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


int variorum_set_and_verify_node_power_limit(int node_power_limit)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.set_and_verify_node_power_limit == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.set_and_verify_node_power_limit(node_power_limit);
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


int variorum_set_gpu_power_ratio(int gpu_power_ratio)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.set_gpu_power_ratio == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.set_gpu_power_ratio(gpu_power_ratio);
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

int variorum_set_each_socket_power_limit(int socket_power_limit)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.set_each_socket_power_limit == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.set_each_socket_power_limit(socket_power_limit);
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

int variorum_set_each_core_frequency(int core_freq_mhz)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.set_each_core_frequency == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.set_each_core_frequency(core_freq_mhz);
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

int variorum_print_features(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.print_features == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.print_features();
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

int variorum_print_thermals(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_thermals == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_thermals(0);
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

int variorum_print_verbose_thermals(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_thermals == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_thermals(1);
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

int variorum_print_counters(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_counters == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_counters(0);
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

int variorum_print_verbose_counters(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_counters == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_counters(1);
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

int variorum_print_clock_speed(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_clocks == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_clocks(0);
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

int variorum_print_verbose_clock_speed(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_clocks == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_clocks(1);
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

int variorum_print_power(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_power == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_power(0);
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

int variorum_print_verbose_power(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_power == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_power(1);
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

int variorum_print_hyperthreading(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    int hyperthreading = (g_platform.num_threads_per_core == 1) ? 0 : 1;
    if (hyperthreading == 1)
    {
        fprintf(stdout, "  Hyperthreading:       Enabled\n");
        fprintf(stdout, "  Num Thread Per Core:  %d\n", g_platform.num_threads_per_core);
    }
    else
    {
        fprintf(stdout, "  Hyperthreading:       Disabled\n");
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
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.dump_turbo == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.dump_turbo();
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

int variorum_enable_turbo(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.enable_turbo == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.enable_turbo();
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

int variorum_disable_turbo(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    if (g_platform.disable_turbo == NULL)
    {
        variorum_error_handler("Null function pointer", VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    err = g_platform.disable_turbo();
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
