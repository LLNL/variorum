// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <power_features.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <variorum_timers.h>
#include <sys/time.h>

void get_power_data(int chipid, int total_sockets, int long_ver, FILE *output)
{
    rsmi_status_t ret;
    uint32_t num_devices;
    uint16_t dev_id;
    int gpus_per_socket;
    char hostname[1024];
    static int init = 0;
    static struct timeval start;
    struct timeval now;

    gethostname(hostname, 1024);

    // We will skip return code checks for this example, but it
    // is recommended to always check this as some calls may not
    // apply for some devices or ROCm releases

    ret = rsmi_init(0);
    ret = rsmi_num_monitor_devices(&num_devices);

    gpus_per_socket = num_devices / total_sockets;

    // printf("\nSanity check, num_devices: %d, total_sockets: %d, gpus_per_sokcet: %d",
    //    num_devices, total_sockets, gpus_per_socket);

    if (!init)
    {
        init = 1;
        gettimeofday(&start, NULL);
        if (long_ver == 0)
        {
            fprintf(output,
                    "_AMD_GPU_POWER Host Socket DeviceID Power Timestamp_sec\n");
        }
    }

    gettimeofday(&now, NULL);

    for (int i = chipid * gpus_per_socket;
         i < (chipid + 1) * gpus_per_socket; i++)
    {
        uint64_t pwr_val = -1;
        double pwr_val_flt = -1.0;

        // We don't need to get a device ID apparently. Just index into the number
        // of devices returned by rsmi_num_monitor_devices
        // ret = rsmi_dev_id_get(i, &dev_id);

        // printf("\nSanity check. ChipID: %d, i:%d, dev_id: %d", chipid, i, dev_id);

        ret = rsmi_dev_power_ave_get(i, 0, &pwr_val);

        pwr_val_flt = (double)(pwr_val / (1000 * 1000)); // Convert to Watts.

        if (long_ver == 0)
        {
            fprintf(output,
                    "_AMD_GPU_POWER_USAGE Host: %s, Socket: %d, DeviceID: %d,"
                    " Power: %lf W, Timestamp: %lf sec\n",
                    hostname, chipid, i, pwr_val_flt,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
        }
        else
        {
            fprintf(output, "_AMD_GPU_POWER_USAGE %s %d %d %lf %lf\n",
                    hostname, chipid, i, pwr_val_flt,
                    (now.tv_usec - start.tv_usec) / 1000000.0);
        }

    }
    ret = rsmi_shut_down();
}

