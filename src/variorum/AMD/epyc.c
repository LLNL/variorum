// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <config_architecture.h>
#include <epyc.h>
#include <variorum_error.h>
#include <e_smi/e_smi.h>

#include "msr_core.h"
#include "amd_power_features.h"

#ifdef CPRINTF_FOUND
#include <cprintf.h>
#endif

int amd_cpu_epyc_get_power(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    int i, ret;
    uint32_t current_power;

    static int initial = 0;
    static struct timeval start;
    struct timeval now;
    char hostname[1024];

    gethostname(hostname, 1024);

    if (!initial)
    {
        initial = 1;
        gettimeofday(&start, NULL);
        if (long_ver == 0)
        {
            #ifdef CPRINTF_FOUND
                cfprintf(stdout, "%s %s %s %s %s\n",
                         "_AMDPOWER", "Host", "Socket", "Power_W", "Timestamp_sec");
            #else
                fprintf(stdout,
                    "_AMDPOWER Host Socket Power_W Timestamp_sec\n");
            #endif
        }
    }

    // DELETE    fprintf(stdout, "Socket | Power(Watts)    |\n");
#ifdef VARIORUM_WITH_AMD_CPU
    for (i = 0; i < g_platform[P_AMD_CPU_IDX].num_sockets; i++)
#endif
    {
        gettimeofday(&now, NULL);

        current_power = 0;
        ret = esmi_socket_power_get(i, &current_power);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to get socket[%d] _POWER, "
                    "Err[%d]:%s\n", i, ret, esmi_get_err_msg(ret));
            return ret;
        }
        else
        {
            if (long_ver == 0)
            {
                #ifdef CPRINTF_FOUND
                    cfprintf(stdout, "%s %s %d %f %lf\n",
                             "_AMDPOWER", hostname, i, (double)current_power / 1000,
                             now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
                #else
                    fprintf(stdout, "_AMDPOWER %s %d %f %lf\n",
                            hostname, i, (double)current_power / 1000,
                            now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);\
                #endif
                /*DELETE     fprintf(stdout, "%6d | %12.03f    |\n",
                            i, (double)current_power / 1000); */
            }
            else
            {
                #ifdef CPRINTF_FOUND
                    cfprintf(stdout, "%s: %s, %s: %d, %s: %f W, %s: %lf\n",
                            "_AMDPOWER Host", hostname, 
                            "Socket", i, 
                            "Power", (double)current_power / 1000, 
                            "Timestamp", now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
                #else
                    fprintf(stdout,
                            "_AMDPOWER Host: %s, Socket: %d, Power: %f W, Timestamp: %lf sec\n",
                            hostname, i, (double)current_power / 1000,
                            now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
                #endif
            }
        }
    }
#ifdef CPRINTF_FOUND
    cflush()
#endif
    return 0;
}

int amd_cpu_epyc_get_power_limits(int long_ver)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    int i, ret;
    uint32_t power, pcap_current, pcap_max;

    static int initial = 0;
    static struct timeval start;
    struct timeval now;
    char hostname[1024];

    gethostname(hostname, 1024);

    if (!initial)
    {
        initial = 1;
        gettimeofday(&start, NULL);
        if (long_ver == 0)
        {
            fprintf(stdout,
                    "_AMDPOWER Host Socket Power_W PowerCap_W MaxPowerCap_W Timestamp_sec\n");
        }
    }

    // DELETE fprintf(stdout,
    // "Socket | Power(Watts)    | PowerCap(Watts) | MaxPowerCap(Watts) |\n");
#ifdef VARIORUM_WITH_AMD_CPU
    for (i = 0; i < g_platform[P_AMD_CPU_IDX].num_sockets; i++)
#endif
    {
        gettimeofday(&now, NULL);

        power = 0;
        pcap_current = 0;
        pcap_max = 0;
        ret = esmi_socket_power_get(i, &power);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to get socket[%d] _POWER, Err[%d]:%s\n",
                    i, ret, esmi_get_err_msg(ret));
            return ret;
        }
        ret = esmi_socket_power_cap_get(i, &pcap_current);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to get socket[%d] _POWERCAP, Err[%d]:%s\n",
                    i, ret, esmi_get_err_msg(ret));
            return ret;
        }
        ret = esmi_socket_power_cap_max_get(i, &pcap_max);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to get socket[%d] _POWERCAPMAX, Err[%d]:%s\n",
                    i, ret, esmi_get_err_msg(ret));
            return ret;
        }
        if (long_ver == 0)
        {
            #ifdef CPRINTF_FOUND
                cfprintf(stdout, "%s %s %d %f %f %f %lf\n",
                         "_AMDPOWER", hostname, i, (double)power / 1000, (double)pcap_current / 1000,
                         (double)pcap_max / 1000,
                         now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
            #else
                fprintf(stdout, "_AMDPOWER %s %d %f %f %f %lf\n",
                        hostname, i, (double)power / 1000, (double)pcap_current / 1000,
                        (double)pcap_max / 1000,
                        now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
            #endif
            /*DELETE     fprintf(stdout, "%6d | %12.03f    |\n",
                        i, (double)current_power / 1000); */
        }
        else
        {
            #ifdef CPRINTF_FOUND
                cfprintf(stdout, "%s: %s, %s: %d, %s: %f W, %s: %f W, %s: %f W, %s: %lf\n",
                        "_AMDPOWER Host", hostname, 
                        "Socket", i, 
                        "Power", (double)power / 1000,
                        "PowerCap", (double)pcap_current / 1000, "MaxPowerCap", (double)pcap_max / 1000,
                        "Timestamp",
                        now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
            #else
                fprintf(stdout,
                        "_AMDPOWER Host: %s, Socket: %d, Power: %f W, PowerCap: %f W, MaxPowerCap: %f W, Timestamp: %lf sec\n",
                        hostname, i, (double)power / 1000, (double)pcap_current / 1000,
                        (double)pcap_max / 1000,
                        now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
            #endif
        }
        /* DELETE fprintf(stdout, "%6d | %14.03f  | %14.03f  | %14.03f     |\n",
                 i, (double)power / 1000, (double)pcap_current / 1000,
                 (double)pcap_max / 1000); */
    }

    #ifdef CPRINTF_FOUND
        cflush()
    #endif

    return 0;
}

int amd_cpu_epyc_set_and_verify_best_effort_node_power_limit(int pcap_new)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s with value %d\n", __FUNCTION__, pcap_new);
    }

    int i, ret;
    uint32_t pcap_test;
    uint32_t max_power = 0;

    /*
     * Convert the pcap value to mWatt as library takes
     * value in mWatt.
     * Also, divide the input by 2, as this sets socket power
     * with best effort. No platform node power cap is available.
     */
    pcap_new = (pcap_new / 2) * 1000;

#ifdef VARIORUM_WITH_AMD_CPU
    for (i = 0; i < g_platform[P_AMD_CPU_IDX].num_sockets; i++)
#endif
    {
        pcap_test = 0;
        ret = esmi_socket_power_cap_max_get(i, &max_power);
        if ((ret == 0) && (pcap_new > (int)max_power))
        {
            printf("Input power is more than max limit,"
                   " So sets to default max %.3f Watts\n\n",
                   (double)max_power / 1000);
            pcap_new = max_power;
        }
        ret = esmi_socket_power_cap_set(i, (uint32_t)pcap_new);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to set socket[%d] _POWERCAP, Err[%d]:%s\n",
                    i, ret, esmi_get_err_msg(ret));
            if (ret == ESMI_PERMISSION)
            {
                variorum_error_handler("Incorrect permissions",
                                       VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                                       __FILE__, __FUNCTION__, __LINE__);
                return -1;
            }
            return ret;
        }
        usleep(100000);

        ret = esmi_socket_power_cap_get(i, &pcap_test);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to get socket[%d] _POWERCAP, Err[%d]:%s\n",
                    i, ret, esmi_get_err_msg(ret));
            return ret;
        }

#ifdef VARIORUM_DEBUG
        fprintf(stdout, "Values are input:%2.03f, test=%2.03f\n",
                (double)pcap_new / 1000, (double)pcap_test / 1000);
#endif

        if (pcap_new != (int)pcap_test)
        {
            fprintf(stdout, "Could not verify if the power cap "
                    "was set correctly.\n");
            fprintf(stdout, "Verification check after 100ms failed.\n");
            fprintf(stdout, "Please verify again with dump_power_limits.\n");
            return -1;
        }
    }

    fprintf(stdout, "Changed node power cap to %2.03f W.\n",
            (double)pcap_new / 1000);
    return 0;
}

int amd_cpu_epyc_set_socket_power_limit(int pcap_new)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    int i, ret;
    uint32_t max_power = 0;

    /*
     * Convert the pcap value to mWatt as library takes
     * value in mWatt.
     */
    pcap_new = pcap_new * 1000;

    #ifdef CPRINTF_FOUND
        cfprintf(stdout, "%s |  %s  |\n", "Socket", "Powercap(Watts)");
    #else
        fprintf(stdout, "Socket |  Powercap(Watts)  |\n");
    #endif

#ifdef VARIORUM_WITH_AMD_CPU
    for (i = 0; i < g_platform[P_AMD_CPU_IDX].num_sockets; i++)
#endif
    {
        ret = esmi_socket_power_cap_max_get(i, &max_power);
        if ((ret == 0) && (pcap_new > (int)max_power))
        {
            printf("Input power is more than max limit,"
                   " So sets to default max %.3f Watts\n\n",
                   (double)max_power / 1000);
            pcap_new = max_power;
        }
        ret = esmi_socket_power_cap_set(i, (uint32_t)pcap_new);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to set socket[%d] _POWERCAP, Err[%d]:%s\n",
                    i, ret, esmi_get_err_msg(ret));
            if (ret == ESMI_PERMISSION)
            {
                variorum_error_handler("Incorrect permissions",
                                       VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                                       __FILE__, __FUNCTION__, __LINE__);
                return -1;
            }
            return ret;
        }
        else
        {
            #ifdef CPRINTF_FOUND
                cfprintf(stdout, "%d |  %.03f  | %s\n",
                        i, (double)pcap_new / 1000, "successfully set");
            #else
                fprintf(stdout, "%6d | %14.03f    | successfully set\n",
                        i, (double)pcap_new / 1000);
            #endif
        }
    }
#ifdef CPRINTF_FOUND
    cflush();
#endif
    return 0;
}

static struct EPYC_19h_offsets msrs =
{
    .msr_rapl_power_unit         = 0xC0010299,
    .msr_core_energy_stat        = 0xC001029A,
    .msr_pkg_energy_stat         = 0xC001029B
};

int amd_cpu_epyc_print_energy()
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    int ret;
    if (!esmi_init())
    {
        int i;
        uint64_t energy;
        fprintf(stdout, "_SOCKET_ENERGY :\n");
        #ifdef CPRINTF_FOUND
            cfprintf(stdout, "%s |  %s |\n", "Socket", "Energy(uJoules)");
        #else
            fprintf(stdout, " Socket |  Energy (uJoules) |\n");
        #endif

#ifdef VARIORUM_WITH_AMD_CPU
        for (i = 0; i < g_platform[P_AMD_CPU_IDX].num_sockets; i++)
#endif
        {
            energy = 0;
            ret = esmi_socket_energy_get(i, &energy);
            if (ret != 0)
            {
                fprintf(stdout, "Failed to get socket[%d] _SOCKENERGY, Err[%d]:%s\n",
                        i, ret, esmi_get_err_msg(ret));
                goto energy_batch;
            }
            else
            {
                #ifdef CPRINTF_FOUND
                    cfprintf(stdout, "%d |  %.06f |\n",
                            i, (double)energy / 1000000);
                #else
                    fprintf(stdout, "%6d  | %17.06f | \n",
                            i, (double)energy / 1000000);
                #endif
            }
        }

        #ifdef CPRINTF_FOUND
            cflush();
            printf("\n_CORE_ENERGY :\n");
            cfprintf(stdout, "%s |  %s |\n", "Core", "Energy(uJoules)");
        #else
            printf("\n_CORE_ENERGY :\n");
            fprintf(stdout, "   Core |  Energy (uJoules) |\n");
        #endif

#ifdef VARIORUM_WITH_AMD_CPU
        for (i = 0; i < g_platform[P_AMD_CPU_IDX].total_cores; i++)
#endif
        {
            energy = 0;
            ret = esmi_core_energy_get(i, &energy);
            if (ret != 0)
            {
                fprintf(stdout, "Failed to get core[%d] _COREENERGY, Err[%d]:%s\n",
                        i, ret, esmi_get_err_msg(ret));
                continue;
            }
            else
            {
                #if CPRINTF_FOUND
                    cfprintf(stdout, "%d |  %.06f |\n",
                            i, (double)energy / 1000000);
                #else
                    fprintf(stdout, " %6d | %17.06f | \n",
                            i, (double)energy / 1000000);
                #endif
            }
        }
        return 0;
    }
energy_batch:
    fprintf(stdout, "Msg: Checking msr-safe module...\n");
    ret = print_energy_data(stdout, msrs.msr_rapl_power_unit,
                            msrs.msr_core_energy_stat);
    return ret;
}

int amd_cpu_epyc_print_boostlimit()
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n\n", __FUNCTION__);
    }

    int i, ret;
    uint32_t boostlimit;

    #ifdef CPRINTF_FOUND
        cfprintf(stdout, "%s |  %s  |\n", "Core", "Freq(MHz)");
    #else
        fprintf(stdout, " Core   | Freq (MHz)  |\n");
    #endif

#ifdef VARIORUM_WITH_AMD_CPU
    for (i = 0; i < g_platform[P_AMD_CPU_IDX].total_cores; i++)
#endif
    {
        boostlimit = 0;
        ret = esmi_core_boostlimit_get(i, &boostlimit);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to get core[%u] _BOOSTLIMIT, Err[%d]:%s\n",
                    i, ret, esmi_get_err_msg(ret));
            return ret;
        }
        else
        {   
            #ifdef CPRINTF_FOUND
                cfprintf(stdout, "%d |  %u  |\n", i, boostlimit);
            #else
                fprintf(stdout, "%6d  | %10u  |\n", i, boostlimit);
            #endif
        }
    }
    
#ifdef CPRINTF_FOUND
    cflush();
#endif

    return 0;
}

int amd_cpu_epyc_set_each_core_boostlimit(int boostlimit)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s with value %u\n\n", __FUNCTION__, boostlimit);
    }

    int i, ret;

#ifdef VARIORUM_WITH_AMD_CPU
    for (i = 0; i < g_platform[P_AMD_CPU_IDX].total_cores; i++)
#endif
    {
        ret = esmi_core_boostlimit_set(i, boostlimit);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to set core[%u] _BOOSTLIMIT, Err[%d]:%s\n",
                    i, ret, esmi_get_err_msg(ret));
            if (ret == ESMI_PERMISSION)
            {
                variorum_error_handler("Incorrect permissions",
                                       VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                                       __FILE__, __FUNCTION__, __LINE__);
                return -1;
            }
        }
    }

#ifdef VARIORUM_DEBUG
    fprintf(stdout, "Values are input:%2u MHz\n",
            boostlimit);
#endif

    return 0;
}


/*
int amd_cpu_epyc_set_and_verify_core_boostlimit(int core, unsigned int boostlimit)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s with value %u\n\n", __FUNCTION__, boostlimit);
    }

    int i, ret;
    uint32_t core_boost_lim = 0;

    ret = esmi_core_boostlimit_set(core, boostlimit);
    if (ret != 0)
    {
        fprintf(stdout, "Failed to set core[%u] _BOOSTLIMIT, Err[%d]:%s\n",
                core, ret, esmi_get_err_msg(ret));
        if (ret == ESMI_PERMISSION)
        {
            variorum_error_handler("Incorrect permissions",
                                   VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                                   __FILE__, __FUNCTION__, __LINE__);
            return -1;
        }
        return ret;
    }
    usleep(100000);

    ret = esmi_core_boostlimit_get(core, &core_boost_lim);
    if (ret == 0)
    {
        if (core_boost_lim < boostlimit)
            printf("Set to possible max boostlimit: %u MHz\n\n", core_boost_lim);
        else if (core_boost_lim > boostlimit)
            printf("Set to possible min boostlimit: %u MHz\n\n", core_boost_lim);
        boostlimit = core_boost_lim;
    }

#ifdef VARIORUM_DEBUG
    fprintf(stdout, "Values are input:%2u MHz, test=%2u MHz\n",
            boostlimit, core_boost_lim);
#endif

    if (boostlimit != core_boost_lim)
    {
        fprintf(stdout, "Could not verify if the boostlimit "
                "was set correctly.\n");
        fprintf(stdout, "Verification check after 100ms failed.\n");
        fprintf(stdout, "Please verify again with set boostlimits.\n");
        return -1;
    }

    fprintf(stdout, "Changed core[%d] boostlimit to %2u MHz.\n",
            core, core_boost_lim);

    return 0;
}
*/


int amd_cpu_epyc_set_socket_boostlimit(int socket, int boostlimit)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s with value %u\n\n", __FUNCTION__, boostlimit);
    }
    int ret;

    ret = esmi_socket_boostlimit_set(socket, boostlimit);
    if (ret != 0)
    {
        fprintf(stdout, "Failed to set socket[%d] _BOOSTLIMIT, Err[%d]:%s\n",
                socket, ret, esmi_get_err_msg(ret));
        if (ret == ESMI_PERMISSION)
        {
            variorum_error_handler("Incorrect permissions",
                                   VARIORUM_ERROR_INVAL, getenv("HOSTNAME"),
                                   __FILE__, __FUNCTION__, __LINE__);
            ret = -1;
        }
    }
    return ret;
}

/* NOTE, June 8, 2022. The build and run of the two JSON APIs below has not been
 * tested because we don't have access to a node with the right drivers and
 * micro-architecture. Please use with caution and report any bugs or fixes to
 * the variorum development team.
 * We expect to test and update these two functions when access is made available.
 * */
int amd_cpu_epyc_get_node_power_json(char **get_power_obj_str)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }
    char hostname[1024];
    struct timeval tv;
    uint64_t ts;
    /* AMD authors declared this as uint32_t and typecast it to double,
     * not sure why. Just following their lead from the get_power function*/
    uint32_t current_power;
    double node_power = 0.0;
    int i, ret = 0;
    int sockID_len = 12;
    char sockID[sockID_len];
    json_t *get_power_obj = json_object();

    gethostname(hostname, 1024);
    gettimeofday(&tv, NULL);
    ts = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
    json_object_set_new(get_power_obj, "host", json_string(hostname));
    json_object_set_new(get_power_obj, "timestamp", json_integer(ts));

#ifdef VARIORUM_WITH_AMD_CPU
    for (i = 0; i < g_platform[P_AMD_CPU_IDX].num_sockets; i++)
#endif
    {
        char cpu_str[36] = "power_cpu_watts_socket_";
        char mem_str[36] = "power_mem_watts_socket_";
        char gpu_str[36] = "power_gpu_watts_socket_";

        snprintf(sockID, sockID_len, "%d", i);
        strcat(cpu_str, sockID);
        strcat(mem_str, sockID);
        strcat(gpu_str, sockID);

        current_power = 0;
        ret = esmi_socket_power_get(i, &current_power);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to get socket[%d] _POWER, "
                    "Err[%d]:%s\n", i, ret, esmi_get_err_msg(ret));
            return ret;
        }
        else
        {
            json_object_set_new(get_power_obj, cpu_str,
                                json_real((double)current_power / 1000));
        }

        // GPU power set to -1.0 for vendor neutrality and first cut, as we
        // don't have a way to measure this yet.
        json_object_set_new(get_power_obj, gpu_str, json_real(-1.0));

        // Memory power set to -1.0 as this platform does not expose
        // memory power yet.
        json_object_set_new(get_power_obj, mem_str, json_real(-1.0));

        node_power += ((double)current_power / 1000);
    }

    // Set the node power key with pwrnode value.
    json_object_set_new(get_power_obj, "power_node_watts", json_real(node_power));

    *get_power_obj_str = json_dumps(get_power_obj, 0);
    json_decref(get_power_obj);

    return 0;
}

int amd_cpu_epyc_get_node_power_domain_info_json(char **get_domain_obj_str)
{
    char *val = getenv("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }
    char hostname[1024];
    struct timeval tv;
    uint64_t ts;
    int ret = 0;
    uint32_t max_power = 0;
    char range_str[100];
    json_t *get_domain_obj = json_object();

    //Get max power from E-SMI from socket 0, same for both sockets.
    //E-SMI doesn't expose minimum yet, something we need AMD to help with.
    //Assuming minimum is 50 W.
    ret = esmi_socket_power_cap_max_get(0, &max_power);

    if (ret != 0)
    {
        fprintf(stdout, "Failed to get maximum socket power, "
                "Err[%d]:%s\n", ret, esmi_get_err_msg(ret));
        return ret;
    }
    // Convert to Watts
    max_power = max_power / 1000;

    snprintf(range_str, sizeof range_str, "%s%d%s%d%s",
             "[{min: ", 50,
             ", max: ", max_power, "}]");

    gethostname(hostname, 1024);
    gettimeofday(&tv, NULL);
    ts = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;

    json_object_set_new(get_domain_obj, "host", json_string(hostname));
    json_object_set_new(get_domain_obj, "timestamp", json_integer(ts));

    json_object_set_new(get_domain_obj, "measurement",
                        json_string("[power_cpu]"));
    json_object_set_new(get_domain_obj, "control",
                        json_string("[power_cpu]"));
    json_object_set_new(get_domain_obj, "unsupported",
                        json_string("[power_node, power_mem]"));
    json_object_set_new(get_domain_obj, "measurement_units",
                        json_string("[Watts]"));
    json_object_set_new(get_domain_obj, "control_units",
                        json_string("[Watts]"));
    json_object_set_new(get_domain_obj, "control_range",
                        json_string(range_str));

    *get_domain_obj_str = json_dumps(get_domain_obj, 0);
    json_decref(get_domain_obj);

    return 0;
}
