// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <config_architecture.h>
#include <Power9.h>
#include <ibm_power_features.h>
#include <variorum_error.h>

#ifdef LIBJUSTIFY_FOUND
#include <cprintf.h>
#endif

/* Figure out the right spot for this at some point */
static pthread_mutex_t mlock;
struct thread_args th_args;
pthread_attr_t mattr;
pthread_t mthread;

/* For the get_energy sampling thread */
static int active_sampling = 0;

int ibm_cpu_p9_get_power(int long_ver)
{
    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    void *buf;
    int fd;
    int rc;
    int bytes;
    unsigned iter = 0;
    unsigned nsockets = 0;

#ifdef VARIORUM_WITH_IBM_CPU
    variorum_get_topology(&nsockets, NULL, NULL, P_IBM_CPU_IDX);
#endif

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

int ibm_cpu_p9_get_power_limits(int long_ver)
{
    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    char hostname[1024];
    FILE *fp = NULL;
    int pcap_current = 0;
    int pcap_max = 0;
    int pcap_min = 0;
    int psr_1 = 0;
    int psr_2 = 0;

    gethostname(hostname, 1024);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "r");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files -- powercap-current",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    fscanf(fp, "%d", &pcap_current);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-max", "r");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files -- powercap-max",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    fscanf(fp, "%d", &pcap_max);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-min", "r");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files -- powercap-min",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    fscanf(fp, "%d", &pcap_min);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/psr/cpu_to_gpu_0", "r");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files -- cpu_to_gpu_0",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    fscanf(fp, "%d", &psr_1);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/psr/cpu_to_gpu_8", "r");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files -- cpu_to_gpu_8",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    fscanf(fp, "%d", &psr_2);
    fclose(fp);

    if (long_ver == 0)
    {
#ifdef LIBJUSTIFY_FOUND
        cfprintf(stdout,
                 "%s %s %s %s %s %s%% %s%%\n", "_POWERCAP", "Host", "CurrentPower_W",
                 "MaxPower_W", "MinPower_W", "PSR_CPU_to_GPU_0_", "PSR_CPU_to_GPU_8_");
        cfprintf(stdout,
                 "%s %s %d %d %d %d %d\n", "_POWERCAP", hostname, pcap_current, pcap_max,
                 pcap_min, psr_1, psr_2);
#else
        fprintf(stdout,
                "_POWERCAP Host CurrentPower_W MaxPower_W MinPower_W PSR_CPU_to_GPU_0_%% PSR_CPU_to_GPU_8_%%\n");
        fprintf(stdout, "_POWERCAP %s %d %d %d %d %d \n",
                hostname, pcap_current, pcap_max, pcap_min, psr_1, psr_2);
#endif
    }
    else
    {
#ifdef LIBJUSTIFY_FOUND
        cfprintf(stdout,
                 "%s: %s, %s: %d W, %s: %d W, %s: %d W, %s: %d%%, %s: %d%%\n",
                 "_POWERCAP Host:", hostname, "CurrentPower:", pcap_current, "MaxPower:",
                 pcap_max, "MinPower:", pcap_min, "PSR_CPU_to_GPU_0:", psr_1,
                 "PSR_CPU_to_GPU_8:", psr_2);
#else
        fprintf(stdout,
                "_POWERCAP Host: %s, CurrentPower: %d W, MaxPower: %d W, MinPower: %d W, PSR_CPU_to_GPU_0: %d%%, PSR_CPU_to_GPU_8: %d%%\n",
                hostname, pcap_current, pcap_max, pcap_min, psr_1, psr_2);
#endif
    }
#ifdef LIBJUSTIFY_FOUND
    cflush();
#endif
    return 0;
}

int ibm_cpu_p9_cap_and_verify_node_power_limit(int pcap_new)
{
    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s with value %d\n", __FUNCTION__, pcap_new);
    }

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

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current",
               "w+");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    fprintf(fp, "%d", pcap_new);
    fclose(fp);

    usleep(100000);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "r");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    fscanf(fp, "%d", &pcap_test);
    fclose(fp);
#ifdef VARIORUM_DEBUG
    fprintf(stdout, "Values are input=%d, test=%d\n", pcap_new, pcap_test);
#endif

    if (pcap_new != pcap_test)
    {
        fprintf(stdout,
                "IBM systems may encounter a delay when setting power limits on the node.");
        fprintf(stdout, "We could not verify if the power cap was set correctly.\n");
        fprintf(stdout, "The verification check after 100ms failed.\n");
        fprintf(stdout, "Please verify again with variorum_print_power_limit().\n");
        return -1;
    }

    fprintf(stdout, "Changed node power cap to %d W.\n", pcap_new);
    return 0;
}

int ibm_cpu_p9_cap_gpu_power_ratio(int gpu_power_ratio)
{
    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s with value %d\n", __FUNCTION__, gpu_power_ratio);
    }

    char hostname[1024];
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;

    gethostname(hostname, 1024);

    fp1 = fopen("/sys/firmware/opal/psr/cpu_to_gpu_0", "w+");
    fp2 = fopen("/sys/firmware/opal/psr/cpu_to_gpu_8", "w+");

    // Fail if both the files cannot be opened.
    if (fp1 == NULL || fp2 == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files",
                               VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    fprintf(fp1, "%d", gpu_power_ratio);
    fprintf(fp2, "%d", gpu_power_ratio);

    fclose(fp1);
    fclose(fp2);

    /* Similar to cap_and_verify, the delay here is about 100ms after
     * experimentation. Here, we don't implement two separate functions to
     * ensure simplicity of user-facing API. We assume that users would like to
     * verify that their GPU power ratio has been set correctly.
     * */

    usleep(100000);

    fprintf(stdout, "Changed power shifting ratio on both sockets to %d percent.\n",
            gpu_power_ratio);
    fprintf(stdout, "\nNOTE: \n");
    fprintf(stdout,
            "  A 100ms delay was added to allow for file operations to propagate out of band.\n"
            "  This delay may not be sufficient, so please verify again with\n"
            "  variorum_print_power_limit() before setting the ratio again. We have seen delays of up to\n"
            "  2 seconds with out of band enforcement for GPU ratio.\n");

    return 0;
}

int ibm_cpu_p9_monitoring(FILE *output)
{
    /* @todo There will be repeat implementation from the get_power API above.
     * We might want to extract that out into a static P9-get-power function,
     * and call then from the ibm_cpu_p9_monitoring() and ibm_cpu_p9_get_power() APIs.
     * fprintf(output, "to be implemented\n");
     * For the first cut, we are just printing power info, we can add other info later.
     * */

    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    void *buf;
    int fd;
    int rc;
    int bytes;
    unsigned iter = 0;
    unsigned nsockets = 0;
    static unsigned count = 0;

#ifdef VARIORUM_WITH_IBM_CPU
    variorum_get_topology(&nsockets, NULL, NULL, P_IBM_CPU_IDX);
#endif

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

        if (count < nsockets)
        {
            print_all_sensors_header(iter, output, buf);
            count++;
        }

        print_all_sensors(iter, output, buf);
        free(buf);
    }
    close(fd);
    return 0;
}

int ibm_cpu_p9_cap_socket_power_limit(int long_ver)
{
    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    if (long_ver == 0 || long_ver == 1)
    {
        printf("Socket limits cannot be set separately on IBM Power9 architecture. Please use cap_node_power_limit API.\n");
    }

    return 0;
}

int ibm_cpu_p9_get_power_json(json_t *get_power_obj)
{
    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    void *buf;
    int fd;
    int rc;
    int bytes;
    unsigned iter = 0;
    unsigned nsockets = 0;

#ifdef VARIORUM_WITH_IBM_CPU
    variorum_get_topology(&nsockets, NULL, NULL, P_IBM_CPU_IDX);
#endif

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

        json_get_power_sensors(iter, get_power_obj, buf);
        free(buf);
    }

    close(fd);
    return 0;
}

int ibm_cpu_p9_get_node_thermal_json(json_t *get_thermal_obj)
{
    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    void *buf;
    int fd;
    int rc;
    int bytes;
    unsigned iter = 0;
    unsigned nsockets = 0;

#ifdef VARIORUM_WITH_IBM_CPU
    variorum_get_topology(&nsockets, NULL, NULL, P_IBM_CPU_IDX);
#endif

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

        json_get_thermal_sensors(iter, get_thermal_obj, buf);
        free(buf);
    }

    close(fd);
    return 0;
}

int ibm_cpu_p9_get_node_power_domain_info_json(char **get_domain_obj_str)
{
    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    char hostname[1024];
    struct timeval tv;
    uint64_t ts;
    json_t *get_domain_obj = json_object();

    gethostname(hostname, 1024);
    gettimeofday(&tv, NULL);
    ts = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;

    json_t *node_obj = json_object();

    json_object_set_new(get_domain_obj, hostname, node_obj);
    json_object_set_new(node_obj, "timestamp", json_integer(ts));

    json_t *control_obj = json_object();
    json_object_set_new(node_obj, "control", control_obj);

    json_t *control_node_obj = json_object();
    json_object_set_new(control_obj, "power_node", control_node_obj);
    json_object_set_new(control_node_obj, "min", json_integer(500));
    json_object_set_new(control_node_obj, "max", json_integer(3050));
    json_object_set_new(control_node_obj, "units", json_string("Watts"));

    json_t *control_gpu_obj = json_object();
    json_object_set_new(control_obj, "power_gpu", control_gpu_obj);
    json_object_set_new(control_gpu_obj, "min", json_integer(0));
    json_object_set_new(control_gpu_obj, "max", json_integer(100));
    json_object_set_new(control_gpu_obj, "units", json_string("Percentage"));

    json_t *unsupported_features = json_array();
    json_object_set_new(node_obj, "unsupported", unsupported_features);

    json_t *measurement_obj = json_object();
    json_object_set_new(node_obj, "measurement", measurement_obj);

    json_t *measurement_node_obj = json_object();
    json_object_set_new(measurement_obj, "power_node", measurement_node_obj);
    json_object_set_new(measurement_node_obj, "units", json_string("Watts"));

    json_t *measurement_cpu_obj = json_object();
    json_object_set_new(measurement_obj, "power_cpu", measurement_cpu_obj);
    json_object_set_new(measurement_cpu_obj, "units", json_string("Watts"));

    json_t *measurement_mem_obj = json_object();
    json_object_set_new(measurement_obj, "power_mem", measurement_mem_obj);
    json_object_set_new(measurement_mem_obj, "units", json_string("Watts"));

    json_t *measurement_gpu_obj = json_object();
    json_object_set_new(measurement_obj, "power_gpu", measurement_gpu_obj);
    json_object_set_new(measurement_gpu_obj, "units", json_string("Watts"));

    // Export JSON object as a string for returning.
    *get_domain_obj_str = json_dumps(get_domain_obj, JSON_INDENT(4));
    json_decref(get_domain_obj);

    return 0;
}

int ibm_cpu_p9_get_node_frequency_json(json_t *get_frequency_obj_json)
{
    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    void *buf;
    int fd;
    int rc;
    int bytes;
    unsigned iter = 0;
    unsigned nsockets = 0;

#ifdef VARIORUM_WITH_IBM_CPU
    variorum_get_topology(&nsockets, NULL, NULL, P_IBM_CPU_IDX);
#endif

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
        json_get_frequency_sensors(iter, get_frequency_obj_json, buf);
        free(buf);
    }

    close(fd);
    return 0;
}

int ibm_cpu_p9_get_energy(int long_ver)
{
    static int init = 0;
    char hostname[1024];
    static struct timeval start;
    struct timeval now;

    gethostname(hostname, 1024);

    if (!init)
    {
        init = 1;
        gettimeofday(&start, NULL);

        if (long_ver == 0)
        {
            printf("_IBMENERGY Host AccumulatedEnergy_J Timestamp_sec\n");
        }
    }

    /* Enter the function the first time */
    if (active_sampling == 0)
    {
        active_sampling = 1;

        gettimeofday(&now, NULL);

        /* Sampling interval is hardcoded at 250ms */
        th_args.sample_interval = 250;
        th_args.energy_acc = 0;

        if (long_ver)
        {
            printf("_IBMENERGY Host: %s, Accumulated Energy: %lu J, Timestamp: %lf sec\n",
                   hostname, th_args.energy_acc,
                   now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
        }
        else
        {
            /* The first call should print zero as energy. */
            printf("%s %s %lu %lf\n",
                   "_IBMENERGY", hostname, th_args.energy_acc,
                   now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
        }

        /* Start power measurement thread. */
        pthread_attr_init(&mattr);
        pthread_attr_setdetachstate(&mattr, PTHREAD_CREATE_DETACHED);
        pthread_mutex_init(&mlock, NULL);
        pthread_create(&mthread, &mattr, power_measurement, NULL);
    }
    else
    {
        /* Stop power measurement thread. */
        active_sampling = 0;

        gettimeofday(&now, NULL);

        pthread_attr_destroy(&mattr);

        pthread_mutex_lock(&mlock);
        if (long_ver)
        {
            printf("_IBMENERGY Host: %s, Accumulated Energy: %lu J, Timestamp: %lf sec\n",
                   hostname, th_args.energy_acc,
                   now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
        }
        else
        {
            printf("%s %s %lu %lf\n",
                   "_IBMENERGY", hostname, th_args.energy_acc,
                   now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
        }
        pthread_mutex_unlock(&mlock);
    }

    return 0;
}

unsigned long take_measurement(int fd)
{
    unsigned long power_sample = 0;

    char *val = ("VARIORUM_LOG");
    if (val != NULL && atoi(val) == 1)
    {
        printf("Running %s\n", __FUNCTION__);
    }

    void *buf;
    int rc;
    int bytes;
    unsigned iter = 0;

    /* We assume that socket 0 on IBM Power9 reports total system power */
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

    power_sample = get_node_power(buf);

    free(buf);
    return power_sample;
}

void *power_measurement(void *arg)
{
    struct mstimer timer;
    unsigned long curr_measurement;
    int fd;

    /* Open inband_sensors file */
    fd = open("/sys/firmware/opal/exports/occ_inband_sensors", O_RDONLY);
    if (fd < 0)
    {
        printf("Failed to open occ_inband_sensors file\n");
    }
    else
    {
        init_msTimer(&timer, th_args.sample_interval);

        timer_sleep(&timer);
        while (active_sampling)
        {
            /* Accummulate energy */
            pthread_mutex_lock(&mlock);
            curr_measurement = take_measurement(fd);
            th_args.energy_acc += curr_measurement * th_args.sample_interval;
            pthread_mutex_unlock(&mlock);
            timer_sleep(&timer);
        }
        /* Close inband_sensors file */
        close(fd);
    }
    return arg;
}

int ibm_cpu_p9_get_node_energy_json(json_t *get_energy_obj)
{
    /* Enter the function the first time */
    if (active_sampling == 0)
    {
        active_sampling = 1;

        /* Sampling interval is hardcoded at 250ms */
        th_args.sample_interval = 250;
        th_args.energy_acc = 0;

        /* Only set node_energy for now */
        json_object_set_new(get_energy_obj, "energy_node_joules",
                            json_integer(th_args.energy_acc));

        /* Start power measurement thread. */
        pthread_attr_init(&mattr);
        pthread_attr_setdetachstate(&mattr, PTHREAD_CREATE_DETACHED);
        pthread_mutex_init(&mlock, NULL);
        pthread_create(&mthread, &mattr, power_measurement, NULL);
    }
    else
    {
        /* Stop power measurement thread. */
        active_sampling = 0;

        /* Commenting out for now, results in invalid pointer and stack trace */
        pthread_attr_destroy(&mattr);

        pthread_mutex_lock(&mlock);

        /* Only set node_energy for now */
        json_object_set_new(get_energy_obj, "energy_node_joules",
                            json_integer(th_args.energy_acc));

        pthread_mutex_unlock(&mlock);
    }

    return 0;
}
