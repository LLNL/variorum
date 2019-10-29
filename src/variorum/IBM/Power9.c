// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <config_architecture.h>
#include <Power9.h>
#include <ibm_sensors.h>
#include <variorum_error.h>

int p9_get_power(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    void *buf;
    int fd;
    int rc;
    int bytes;
    int initial_bytes;
    int iter = 0;
    int nsockets;

    variorum_set_topology(&nsockets, NULL, NULL);

    fd = open("/sys/firmware/opal/exports/occ_inband_sensors", O_RDONLY);
    if (fd < 0)
    {
        printf("Failed to open occ_inband_sensors file\n");
        return -1;
    }

    for (iter = 0; iter < nsockets; iter++)
    {
        lseek(fd, iter * OCC_SENSOR_DATA_BLOCK_SIZE, SEEK_SET);

        buf = malloc(OCC_SENSOR_DATA_BLOCK_SIZE);

        if (!buf)
        {
            printf("Failed to allocate\n");
            return -1;
        }

        for (rc = bytes = 0; bytes < OCC_SENSOR_DATA_BLOCK_SIZE; bytes += rc)
        {
            rc = read(fd, buf + bytes, OCC_SENSOR_DATA_BLOCK_SIZE - bytes);

            if (!rc || rc < 0)
            {
                break;
            }
        }

        if (bytes != OCC_SENSOR_DATA_BLOCK_SIZE)
        {
            printf("Failed to read data\n");
            free(buf);
            return -1;
        }

        print_power_sensors(iter, long_ver, stdout, buf);
        free(buf);
    }
    close(fd);
    return 0;
}

int p9_get_power_limits(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    char hostname[1024];
    FILE *fp = NULL;
    int pcap_current = 0;
    int pcap_max = 0;
    int pcap_min = 0;
    int psr_1 = 0;
    int psr_2 = 0;

    gethostname(hostname, 1024);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "r");
    fscanf(fp, "%d", &pcap_current);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-max", "r");
    fscanf(fp, "%d", &pcap_max);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-min", "r");
    fscanf(fp, "%d", &pcap_min);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/psr/cpu_to_gpu_0", "r");
    fscanf(fp, "%d", &psr_1);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/psr/cpu_to_gpu_8", "r");
    fscanf(fp, "%d", &psr_2);
    fclose(fp);

    fprintf(stdout, "_POWERCAP | CurrentWatts | MaxWatts | MinWatts | CPU_to_GPU_0 PSR | CPU_to_GPU_8 PSR\n");
    fprintf(stdout, "_POWERCAP | %d | %d | %d | %d | %d \n", pcap_current, pcap_max, pcap_min, psr_1, psr_2);
    return 0;
}

int p9_set_and_verify_node_power_limit(int pcap_new)
{
#ifdef VARIORUM_LOG
    printf("Running %s with value %d\n", __FUNCTION__, pcap_new);
#endif

    char hostname[1024];
    FILE *fp = NULL;
    int pcap_test = 0;

    gethostname(hostname, 1024);

    /* Strange problem and portability note from T. Patki, 11/27/18, Alehouse:
     * Here, we are doing a write followed by a read. If we want to do this,
     * usleep is needed because file operations seem to take some time to update and
     * return (most likely cause is a lazy-write and seek time). After a bit of testing, 100ms seems like
     * a good sleep time for alehouse to finish doing an fprintf and allow for a fscanf.
     * Using the same file pointer doesn't work, file has to be closed and reopened. I'm afraid
     * the 100ms value may not be portable. I've tried reusing the file pointer
     * with rewind/fseek/fflush, and that didn't help. Maybe I'm missing something.
     * We could completely bypass this approach of write-and-test, and only go with a simple fprintf
     * but that would not be the most user-friendly and we will not be able to confirm a power cap change.
     * Running this function back-to-back can also cause issues.
     * We can decide later what's best for production.
     * */

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "w+");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files", VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    fprintf(fp, "%d", pcap_new);
    fclose(fp);

    usleep(100000);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "r");
    fscanf(fp, "%d", &pcap_test);
    fclose(fp);
#ifdef VARIORUM_DEBUG
    fprintf(stdout, "Values are input=%d, test=%d\n", pcap_new, pcap_test);
#endif

    if (pcap_new != pcap_test)
    {
        fprintf(stdout, "Could not verify if the power cap was set correctly.\n");
        fprintf(stdout, "Verification check after 100ms failed.\n");
        fprintf(stdout, "Please verify again with dump_power_limits.\n");
        return -1;
    }

    fprintf(stdout, "Changed node power cap to %d W.\n", pcap_new);
    return 0;
}

int p9_set_node_power_limit(int pcap_new)
{
#ifdef VARIORUM_LOG
    printf("Running %s with value %d\n", __FUNCTION__, pcap_new);
#endif

    char hostname[1024];
    FILE *fp = NULL;

    gethostname(hostname, 1024);

    /* Strange problem and portability note from T. Patki, 11/27/18, Alehouse:
     * Here, we are doing a write and not verifying whether the power cap was set.
     * Introduced as per Barry's suggestion, so users can decide if they want a 10ms delay
     * that we need for verification.
     * */

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "w+");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files", VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    fprintf(fp, "%d", pcap_new);
    fclose(fp);

    fprintf(stdout, "Changed node power cap to %d W.\n", pcap_new);
    fprintf(stdout, "\nNOTE: \n");
    fprintf(stdout, "  It may take 100-500ms for power cap to propagate out of band, so please\n"
            "  verify again with dump_power_limits before setting the cap again.\n");
    return 0;
}

int p9_set_gpu_power_ratio(int gpu_power_ratio)
{
#ifdef VARIORUM_LOG
    printf("Running %s with value %d\n", __FUNCTION__, gpu_power_ratio);
#endif

    char hostname[1024];
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;

    gethostname(hostname, 1024);

    fp1 = fopen("/sys/firmware/opal/psr/cpu_to_gpu_0", "w+");
    fp2 = fopen("/sys/firmware/opal/psr/cpu_to_gpu_8", "w+");

    // Fail if both the files cannot be opened.
    if (fp1 == NULL || fp2 == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files", VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    fprintf(fp1, "%d", gpu_power_ratio);
    fprintf(fp2, "%d", gpu_power_ratio);

    fclose(fp1);
    fclose(fp2);

    /* Similar to set_and_verify, the delay here is about 100ms after
     * experimentation. Here, we don't implement two separate functions to
     * ensure simplicity of user-facing API. We assume that users would like to
     * verify that their GPU power ratio has been set correctly.
     * */

    usleep(100000);

    fprintf(stdout, "Changed power shifting ratio on both sockets to %d percent.\n", gpu_power_ratio);
    fprintf(stdout, "\nNOTE: \n");
    fprintf(stdout, "  A 100ms delay was added to allow for file operations to propagate out of band.\n"
            "  This delay may not be sufficient, so please verify again with\n"
            "  dump_power_limits before setting the ratio again. We have seen delays of up to\n"
            "  2 seconds with out of band enforcement for GPU ratio. \n");

    return 0;
}

int p9_monitoring(FILE *output)
{
    /* @todo There will be repeat implementation from the get_power API above.
     * We might want to extract that out into a static P9-get-power function,
     * and call then from the p9_monitoring() and p9_get_power() APIs.
     * fprintf(output, "to be implemented\n");
     * For the first cut, we are just printing power info, we can add other info later.
     * */

#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    void *buf;
    int fd;
    int rc;
    int bytes;
    int initial_bytes;
    int iter = 0;
    int nsockets;
    int long_ver = 0;

    variorum_set_topology(&nsockets, NULL, NULL);

    fd = open("/sys/firmware/opal/exports/occ_inband_sensors", O_RDONLY);
    if (fd < 0)
    {
        printf("Failed to open occ_inband_sensors file\n");
        return -1;
    }

    for (iter = 0; iter < nsockets; iter++)
    {
        lseek(fd, iter * OCC_SENSOR_DATA_BLOCK_SIZE, SEEK_SET);

        buf = malloc(OCC_SENSOR_DATA_BLOCK_SIZE);

        if (!buf)
        {
            printf("Failed to allocate\n");
            return -1;
        }

        for (rc = bytes = 0; bytes < OCC_SENSOR_DATA_BLOCK_SIZE; bytes += rc)
        {
            rc = read(fd, buf + bytes, OCC_SENSOR_DATA_BLOCK_SIZE - bytes);

            if (!rc || rc < 0)
            {
                break;
            }
        }

        if (bytes != OCC_SENSOR_DATA_BLOCK_SIZE)
        {
            printf("Failed to read data\n");
            free(buf);
            return -1;
        }

        print_all_sensors(iter, output, buf);
        free(buf);
    }
    close(fd);
    return 0;
}

int p9_set_socket_power_limit (int pcap_new)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    printf("Socket limits cannot be set separately on IBM Power9 architecture. Please use set_node_power_limit API.\n");

    return 0;
}
