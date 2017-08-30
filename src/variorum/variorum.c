#include <stdio.h>

#include <config_architecture.h>
#include <variorum.h>

int tester(void)
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

int dump_power_limits(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int print_power_limits(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int print_topology(void)
{
    int err = 0;
    int i;
    int hyperthreading = 0;

    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }

    fprintf(stdout, "Platform Toplogy\n");
    fprintf(stdout, "  Hostname:             %s\n", g_platform.hostname);
    fprintf(stdout, "  Num Sockets:          %d\n", g_platform.num_sockets);
    fprintf(stdout, "  Num Cores per Socket: %d\n", g_platform.num_cores_per_socket);
    fprintf(stdout, "  Num Threads per Core: %d\n", g_platform.num_threads_per_core);
    hyperthreading = (g_platform.num_threads_per_core == 1) ? 0 : 1;
    if (hyperthreading == 1)
    {
        fprintf(stdout, "  Hyperthreading:       Yes\n");
    }
    else
    {
        fprintf(stdout, "  Hyperthreading:       No\n");
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  Total Num of Cores:   %d\n", g_platform.total_cores);
    fprintf(stdout, "  Total Num of Threads: %d\n", g_platform.total_threads);
    fprintf(stdout, "\n");
    fprintf(stdout, "  Physical (OS) Thread to Logical Core Map\n");
    for (i = 0; i < g_platform.total_threads; i++)
    {
        fprintf(stdout, "    PU#%d = Core#%d\n", i, g_platform.map_pu_to_core[i].physical_core_idx);
    }

    err = variorum_exit(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
        return -1;
    }
    return err;
}

int set_each_socket_power_limit(int socket_power_limit)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int print_features(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int dump_thermals(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int print_thermals(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int dump_counters(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int print_counters(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int dump_clock_speed(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int print_clock_speed(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int dump_power(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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

int print_power(void)
{
    int err = 0;
    err = variorum_enter(__FILE__, __FUNCTION__, __LINE__);
    if (err)
    {
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
