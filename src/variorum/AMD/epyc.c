// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <config_architecture.h>
#include <epyc.h>
#include <variorum_error.h>
#include <e_smi/e_smi.h>

int epyc_get_power(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    int i, ret;
    uint32_t current_power;

    fprintf(stdout, "Socket | Power(Watts)    |\n");
    for (i = 0; i < g_platform.num_sockets; i++)
    {
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
            fprintf(stdout, "%6d | %12.03f    |\n",
                    i, (double)current_power/1000);
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

    fprintf(stdout, "Socket | Power(Watts)    | PowerCap(Watts) | MaxPowerCap(Watts) |\n");
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
                i, (double)power/1000, (double)pcap_current/1000,
                (double)pcap_max/1000);
    }

    return 0;
}

int epyc_set_and_verify_node_power_limit(int pcap_new)
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
     */
    pcap_new = pcap_new * 1000;

    for (i = 0; i < g_platform.num_sockets; i++)
    {
        pcap_test = 0;
        ret = esmi_socket_power_cap_max_get(i, &max_power);
        if ((ret == 0) && (pcap_new > max_power))
        {
            printf("Input power is more than max limit,"
                   " So sets to default max %.3f Watts\n\n",
                   (double)max_power/1000);
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
            (double)pcap_new/1000, (double)pcap_test/1000);
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
            (double)pcap_new/1000);
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
                   (double)max_power/1000);
            pcap_new = max_power;
        }
        ret = esmi_socket_power_cap_set(i, (uint32_t)pcap_new);
        if(ret != 0)
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
                    i, (double)pcap_new/1000);
        }
    }
    return 0;
}
