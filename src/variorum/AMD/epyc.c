// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include <config_architecture.h>
#include <epyc.h>
#include <variorum_error.h>
#include <e_smi/e_smi.h>

#include "msr_core.h"
#include "energy_feature.h"

int epyc_get_power(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    static int initial = 0;
    static struct timeval start;
    struct timeval now;
    char hostname[1024];

    int i, ret;
    uint32_t current_power;

    gethostname(hostname, 1024);

    if (!initial)
    {   
        initial = 1;
        gettimeofday(&start, NULL);
        if (long_ver == 0)
        {   
            fprintf(stdout,
                    "_AMDPOWER Host Socket PWRCPU_W Timestamp_sec\n");
        }   
    }   


// DELETE    fprintf(stdout, "Socket | Power(Watts)    |\n");
    for (i = 0; i < g_platform.num_sockets; i++)
    {
        current_power = 0;
    	gettimeofday(&now, NULL);
        ret = esmi_socket_power_get(i, &current_power);
        if (ret != 0)
        {
            fprintf(stdout, "Failed to get socket[%d] _POWER, "
                    "Err[%d]:%s\n", i, ret, esmi_get_err_msg(ret));
            return ret;
        }
        else
        {
	     if (long_ver == 0) {
            	fprintf(stdout, "_AMDPOWER %s %d %12.03f %lf\n",
                hostname, i, (double)current_power / 1000,
                now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
	     	/*DELETE     fprintf(stdout, "%6d | %12.03f    |\n",
                	    i, (double)current_power / 1000); */
	     } 
	     else 
	     {
		    fprintf(stdout,
                "_AMDPOWER Host: %s, Socket: %d, PWRCPU: %12.03f W, Timestamp: %lf sec\n",
                hostname, i, (double)current_power / 1000,
                now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
	     } 
        }
    }
    return 0;
}

int epyc_get_power_limits(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    int i, ret;
    uint32_t power, pcap_current, pcap_max;

    fprintf(stdout,
            "Socket | Power(Watts)    | PowerCap(Watts) | MaxPowerCap(Watts) |\n");
    for (i = 0; i < g_platform.num_sockets; i++)
    {
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
        fprintf(stdout, "%6d | %14.03f  | %14.03f  | %14.03f     |\n",
                i, (double)power / 1000, (double)pcap_current / 1000,
                (double)pcap_max / 1000);
    }

    return 0;
}

int epyc_set_and_verify_best_effort_node_power_limit(int pcap_new)
{
#ifdef VARIORUM_LOG
    printf("Running %s with value %d\n", __FUNCTION__, pcap_new);
#endif

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

    for (i = 0; i < g_platform.num_sockets; i++)
    {
        pcap_test = 0;
        ret = esmi_socket_power_cap_max_get(i, &max_power);
        if ((ret == 0) && (pcap_new > max_power))
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

        if (pcap_new != pcap_test)
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

int epyc_set_socket_power_limit(int pcap_new)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    int i, ret;
    uint32_t max_power = 0;

    /*
     * Convert the pcap value to mWatt as library takes
     * value in mWatt.
     */
    pcap_new = pcap_new * 1000;

    fprintf(stdout, "Socket |  Powercap(Watts)  |\n");
    for (i = 0; i < g_platform.num_sockets; i++)
    {
        ret = esmi_socket_power_cap_max_get(i, &max_power);
        if ((ret == 0) && (pcap_new > max_power))
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
            fprintf(stdout, "%6d | %14.03f    | successfully set\n",
                    i, (double)pcap_new / 1000);
        }
    }
    return 0;
}

static struct EPYC_19h_offsets msrs =
{
    .msr_rapl_power_unit         = 0xC0010299,
    .msr_core_energy_stat        = 0xC001029A,
    .msr_pkg_energy_stat         = 0xC001029B
};

int epyc_print_energy()
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    int ret;
    if (!esmi_init())
    {
        int i;
        uint64_t energy;

        fprintf(stdout, "_SOCKET_ENERGY :\n");
        fprintf(stdout, " Socket |  Energy (uJoules) |\n");
        for (i = 0; i < g_platform.num_sockets; i++)
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
                fprintf(stdout, "%6d  | %17.06f | \n",
                        i, (double)energy / 1000000);
            }
        }
        printf("\n_CORE_ENERGY :\n");
        fprintf(stdout, "   Core |  Energy (uJoules) |\n");
        for (i = 0; i < g_platform.total_cores; i++)
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
                fprintf(stdout, " %6d | %17.06f | \n",
                        i, (double)energy / 1000000);
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

int epyc_print_boostlimit()
{
#ifdef VARIORUM_LOG
    printf("Running %s\n\n", __FUNCTION__);
#endif

    int i, ret;
    uint32_t boostlimit;

    fprintf(stdout, " Core   | Freq (MHz)  |\n");
    for (i = 0; i < g_platform.total_cores; i++)
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
            fprintf(stdout, "%6d  | %10u  |\n", i, boostlimit);
        }
    }
    return 0;
}

int epyc_set_each_core_boostlimit(int boostlimit)
{
#ifdef VARIORUM_LOG
    printf("Running %s with value %u\n\n", __FUNCTION__, boostlimit);
#endif

    int i, ret;
    uint32_t core_boost_lim = 0;

    for (i = 0; i < g_platform.total_cores; i++)
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
    fprintf(stdout, "Values are input:%2u MHz, test=%2u MHz\n",
            boostlimit, core_boost_lim);
#endif

    return 0;
}


/*
int epyc_set_and_verify_core_boostlimit(int core, unsigned int boostlimit)
{
#ifdef VARIORUM_LOG
    printf("Running %s with value %u\n\n", __FUNCTION__, boostlimit);
#endif

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


int epyc_set_socket_boostlimit(int socket, int boostlimit)
{
#ifdef VARIORUM_LOG
    printf("Running %s with value %u\n\n", __FUNCTION__, boostlimit);
#endif

    int ret;
    uint32_t blimit = 0;
    uint32_t online_core;

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
