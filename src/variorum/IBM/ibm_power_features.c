// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <ibm_power_features.h>

#ifdef LIBJUSTIFY_FOUND
#include <cprintf.h>
#endif

unsigned long read_counter(const struct occ_sensor_data_header *hb,
                           uint32_t offset)
{
    struct occ_sensor_counter *sping;
    struct occ_sensor_counter *spong;
    struct occ_sensor_counter *sensor = NULL;
    uint8_t *ping;
    uint8_t *pong;

    ping = (uint8_t *)((uint64_t)hb + be32toh(hb->reading_ping_offset));
    pong = (uint8_t *)((uint64_t)hb + be32toh(hb->reading_pong_offset));
    sping = (struct occ_sensor_counter *)((uint64_t)ping + offset);
    spong = (struct occ_sensor_counter *)((uint64_t)pong + offset);

    if (*ping && *pong)
    {
        if (be64toh(sping->timestamp) > be64toh(spong->timestamp))
        {
            sensor = sping;
        }
        else
        {
            sensor = spong;
        }
    }
    else if (*ping && !*pong)
    {
        sensor = sping;
    }
    else if (!*ping && *pong)
    {
        sensor = spong;
    }
    else if (!*ping && !*pong)
    {
        return 0;
    }

    return be64toh(sensor->accumulator);
}

unsigned long read_sensor(const struct occ_sensor_data_header *hb,
                          uint32_t offset, int attr)
{
    struct occ_sensor_record *sping;
    struct occ_sensor_record *spong;
    struct occ_sensor_record *sensor = NULL;
    uint8_t *ping;
    uint8_t *pong;

    ping = (uint8_t *)((uint64_t)hb + be32toh(hb->reading_ping_offset));
    pong = (uint8_t *)((uint64_t)hb + be32toh(hb->reading_pong_offset));
    sping = (struct occ_sensor_record *)((uint64_t)ping + offset);
    spong = (struct occ_sensor_record *)((uint64_t)pong + offset);

    if (*ping && *pong)
    {
        if (be64toh(sping->timestamp) > be64toh(spong->timestamp))
        {
            sensor = sping;
        }
        else
        {
            sensor = spong;
        }
    }
    else if (*ping && !*pong)
    {
        sensor = sping;
    }
    else if (!*ping && *pong)
    {
        sensor = spong;
    }
    else if (!*ping && !*pong)
    {
        return 0;
    }

    switch (attr)
    {
        case SENSOR_SAMPLE:
            return be16toh(sensor->sample);
        case SENSOR_ACCUMULATOR:
            return be64toh(sensor->accumulator);
        default:
            break;
    }

    return 0;
}

uint64_t get_node_power(const void *buf)
{
    int i;
    struct occ_sensor_data_header *hb;
    struct occ_sensor_name *md;
    // Power in watts.
    uint64_t pwrsys = 0;

    hb = (struct occ_sensor_data_header *)(uint64_t)buf;
    md = (struct occ_sensor_name *)((uint64_t)hb + be32toh(hb->names_offset));

    for (i = 0; i < be16toh(hb->nr_sensors); i++)
    {
        uint32_t offset = be32toh(md[i].reading_offset);
        uint32_t scale = be32toh(md[i].scale_factor);
        uint64_t sample = 0;

        // We are not reading counters here because power data doesn't need counter.
        if (md[i].structure_type == OCC_SENSOR_READING_FULL)
        {
            sample = read_sensor(hb, offset, SENSOR_SAMPLE);
        }

        if (strcmp(md[i].name, "PWRSYS") == 0)
        {
            pwrsys = (uint64_t)(sample * TO_FP(scale));
        }
    }
    return (pwrsys);
}

void print_power_sensors(int chipid, int long_ver, FILE *output,
                         const void *buf)
{
    struct occ_sensor_data_header *hb;
    struct occ_sensor_name *md;
    int i = 0;
    static int init = 0;
    char hostname[1024];
    static struct timeval start;
    struct timeval now;
    // Power in watts.
    uint64_t pwrsys = 0;
    uint64_t pwrproc = 0;
    uint64_t pwrmem = 0;
    uint64_t pwrgpu = 0;

    /* Note that the timestamps here will be different for each socket.
     * We don't have the equivalent of batching yet, if we read the whole
     * buffer at once, we can't easily separate out the sockets and we will
     * have to identify OCC_DATA_BLOCK boundaries in the code (which we want to
     * avoid).
     * */

    gethostname(hostname, 1024);

    if (!init)
    {
        init = 1;
        gettimeofday(&start, NULL);
        if (long_ver == 0)
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, "%s %s %s %s %s %s %s %s\n",
                     "_IBMPOWER", "Host", "Socket", "PWRSYS_W",
                     "PWRPROC_W", "PWRMEM_W", "PWRGPU_W", "Timestamp_sec");
#else
            fprintf(output, "%s %s %s %s %s %s %s %s\n",
                    "_IBMPOWER", "Host", "Socket", "PWRSYS_W",
                    "PWRPROC_W", "PWRMEM_W", "PWRGPU_W", "Timestamp_sec");
#endif
        }
    }

    gettimeofday(&now, NULL);

    hb = (struct occ_sensor_data_header *)(uint64_t)buf;
    md = (struct occ_sensor_name *)((uint64_t)hb + be32toh(hb->names_offset));

    for (i = 0; i < be16toh(hb->nr_sensors); i++)
    {
        uint32_t offset = be32toh(md[i].reading_offset);
        uint32_t scale = be32toh(md[i].scale_factor);
        uint64_t sample = 0;

        // We are not reading counters here because power data doesn't need counter.
        if (md[i].structure_type == OCC_SENSOR_READING_FULL)
        {
            sample = read_sensor(hb, offset, SENSOR_SAMPLE);
        }

        /* Ideally, we don't want to use strcmp and use offsets instead.
         * But this was not clear to us at the time of the implementation.
         * OCC_DATA_BLOCK contents are different because of master-slave design
         * across different processor sockets. So the same offset can refer to
         * a different sensor depending on the socket.
         *
         * Note that we're not capturing timestamp here, the common timestamp
         * printed is the one from the end of the loop, where all 336 sensors
         * have been scanned on P9.
         * */

        if (strcmp(md[i].name, "PWRSYS") == 0)
        {
            pwrsys = (uint64_t)(sample * TO_FP(scale));
        }
        if (strcmp(md[i].name, "PWRPROC") == 0)
        {
            pwrproc = (uint64_t)(sample * TO_FP(scale));
        }
        if (strcmp(md[i].name, "PWRMEM") == 0)
        {
            pwrmem = (uint64_t)(sample * TO_FP(scale));
        }
        if (strcmp(md[i].name, "PWRGPU") == 0)
        {
            pwrgpu = (uint64_t)(sample * TO_FP(scale));
        }
    }

    if (long_ver == 0)
    {
#ifdef LIBJUSTIFY_FOUND
        cfprintf(output,
                 "%s %s %d %lu %lu %lu %lu %lf\n",
                 "_IBMPOWER", hostname, chipid,
                 pwrsys, pwrproc, pwrmem, pwrgpu,
                 now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
#else
        fprintf(output,
                "%s %s %d %lu %lu %lu %lu %lf\n",
                "_IBMPOWER", hostname, chipid,
                pwrsys, pwrproc, pwrmem, pwrgpu,
                now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0);
#endif
    }
    else
    {
#ifdef LIBJUSTIFY_FOUND
        cfprintf(output,
                 "%s: %s, %s: %d, %s: %lu W, %s: %lu W, %s: %lu W, %s: %lu W, %s: %lf sec\n",
                 "_IBMPOWER Host", hostname, "Socket", chipid,
                 "PWRSYS", pwrsys, "PWRPROC", pwrproc, "PWRMEM", pwrmem,
                 "PWRGPU", pwrgpu,
                 "Timestamp", now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) /
                 1000000.0);
#else
        fprintf(output,
                "%s: %s, %s: %d, %s: %lu W, %s: %lu W, %s: %lu W, %s: %lu W, %s: %lf sec\n",
                "_IBMPOWER Host", hostname, "Socket", chipid,
                "PWRSYS", pwrsys, "PWRPROC", pwrproc, "PWRMEM", pwrmem,
                "PWRGPU", pwrgpu,
                "Timestamp", now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) /
                1000000.0);
#endif
    }
    //#ifdef LIBJUSTIFY_FOUND
    //    printf("CALLING FLUSH\n");
    //    cflush();
    //#endif
}

// TODO: Every place where there is no space before a conversion specifer needs to be fixed
// this isn't currently supported
void print_all_sensors_header(int chipid, FILE *output, const void *buf)
{
    struct occ_sensor_data_header *hb;
    struct occ_sensor_name *md;
    int i = 0;

    hb = (struct occ_sensor_data_header *)(uint64_t)buf;
    md = (struct occ_sensor_name *)((uint64_t)hb + be32toh(hb->names_offset));

#ifdef LIBJUSTIFY_FOUND //TODO: EVALUATE THIS AS WELL
    char lbl[50];
    sprintf(lbl, "_IBMPOWER %d", chipid);

    cfprintf(output, "%s %s %s %s", lbl, "Timestamp_sec", "Host", "Socket");
#else
    fprintf(output, "_IBMPOWER%d Timestamp_sec Host Socket", chipid);
#endif

    for (i = 0; i < be16toh(hb->nr_sensors); i++)
    {
        if (be16toh(md[i].type) == OCC_SENSOR_TYPE_POWER)
        {
#ifdef LIBJUSTIFY_FOUND //TODO: EVALUATE THIS, I don't think this will behave as intended
            cfprintf(output, " %s %s %s %s %s", md[i].name, "_Scale_", md[i].units,
                     md[i].name, "_Energy_J");
#else
            fprintf(output, " %s_Scale_%s %s_Energy_J", md[i].name, md[i].units,
                    md[i].name);
#endif
        }
        else
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, " %s_%s", md[i].name, md[i].units);
#else
            fprintf(output, " %s_%s", md[i].name, md[i].units);
#endif
        }
    }
#ifdef LIBJUSTIFY_FOUND
    cfprintf(output, "\n"); // Add end of line.
    //printf("CALLING FLUSH\n");
    //cflush();
#else
    fprintf(output, "\n"); // Add end of line.
#endif
}

void print_all_sensors(int chipid, FILE *output, const void *buf)
{
    struct occ_sensor_data_header *hb;
    struct occ_sensor_name *md;
    int i = 0;
    static int init = 0;
    char hostname[1024];
    static struct timeval start;
    struct timeval now;
    int energy_offset = 0;

    /* Note that the timestamps here will be different for each socket. We
     * don't have the equivalent of batching yet, if we read the whole buffer
     * at once, we can't easily separate out the sockets and we will have to
     * identify OCC_DATA_BLOCK boundaries in the code (which we want to avoid).
     * */

    gethostname(hostname, 1024);

    if (!init)
    {
        init = 1;
        gettimeofday(&start, NULL);
    }
    gettimeofday(&now, NULL);

    hb = (struct occ_sensor_data_header *)(uint64_t)buf;
    md = (struct occ_sensor_name *)((uint64_t)hb + be32toh(hb->names_offset));

#ifdef LIBJUSTIFY_FOUND //TODO: EVALUATE
    char lbl[50];
    sprintf(lbl, "_IBMPOWER%d", chipid);
    cfprintf(output, "%s %lf %s %d", lbl,
             now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0, hostname,
             chipid);
#else
    fprintf(output, "_IBMPOWER%d %lf %s %d", chipid,
            now.tv_sec - start.tv_sec + (now.tv_usec - start.tv_usec) / 1000000.0, hostname,
            chipid);
#endif

    for (i = 0; i < be16toh(hb->nr_sensors); i++)
    {
        uint32_t offset = be32toh(md[i].reading_offset);
        uint32_t scale = be32toh(md[i].scale_factor);
        uint64_t sample;

        if (md[i].structure_type == OCC_SENSOR_READING_FULL)
        {
            sample = read_sensor(hb, offset, SENSOR_SAMPLE);
        }
        else
        {
            sample = read_counter(hb, offset);
        }

        if (be16toh(md[i].type) == OCC_SENSOR_TYPE_POWER)
        {
            uint64_t energy = read_sensor(hb, offset, SENSOR_ACCUMULATOR);
            uint32_t freq = be32toh(md[i].freq);

            if (!energy_offset)
            {
                energy_offset = i;
            }

            // Note that we're not capturing timestamp here, the common timestamp printed
            // is the one from the beginning of the loop.
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, " %lu %lu", (uint64_t)(sample * TO_FP(scale)),
                     (uint64_t)(energy / TO_FP(freq)));
#else
            fprintf(output, " %lu %lu", (uint64_t)(sample * TO_FP(scale)),
                    (uint64_t)(energy / TO_FP(freq)));
#endif
        }
        else
        {
#ifdef LIBJUSTIFY_FOUND
            cfprintf(output, " %lu", (uint64_t)(sample * TO_FP(scale)));
#else
            fprintf(output, " %lu", (uint64_t)(sample * TO_FP(scale)));
#endif
        }
    }
#ifdef LIBJUSTIFY_FOUND
    cfprintf(output, "\n"); // Add end of line.
    //printf("CALLING FLUSH\n");
    //cflush();
#else
    fprintf(output, "\n"); // Add end of line.
#endif
}

void json_get_power_sensors(int chipid, json_t *node_obj, const void *buf)
{
    struct occ_sensor_data_header *hb;
    struct occ_sensor_name *md;
    int i = 0;
    // Power in watts.
    uint64_t pwrsys = 0;
    uint64_t pwrproc = 0;
    uint64_t pwrmem = 0;
    char socketID[12];

    sprintf(socketID, "socket_%d", chipid);

    hb = (struct occ_sensor_data_header *)(uint64_t)buf;
    md = (struct occ_sensor_name *)((uint64_t)hb + be32toh(hb->names_offset));

    for (i = 0; i < be16toh(hb->nr_sensors); i++)
    {
        uint32_t offset = be32toh(md[i].reading_offset);
        uint32_t scale = be32toh(md[i].scale_factor);
        uint64_t sample = 0;

        // We are not reading counters here because power data doesn't need counter.
        if (md[i].structure_type == OCC_SENSOR_READING_FULL)
        {
            sample = read_sensor(hb, offset, SENSOR_SAMPLE);
        }

        /* Ideally, we don't want to use strcmp and use offsets instead.
         * But this was not clear to us at the time of the implementation.
         * OCC_DATA_BLOCK contents are different because of master-slave design
         * across different processor sockets. So the same offset can refer to
         * a different sensor depending on the socket.
         *
         * Note that we're not capturing timestamp here, the common timestamp
         * printed is the one from the end of the loop, where all 336 sensors
         * have been scanned on P9.
         * */

        if (strcmp(md[i].name, "PWRSYS") == 0)
        {
            pwrsys = (uint64_t)(sample * TO_FP(scale));
        }
        if (strcmp(md[i].name, "PWRPROC") == 0)
        {
            pwrproc = (uint64_t)(sample * TO_FP(scale));
        }
        if (strcmp(md[i].name, "PWRMEM") == 0)
        {
            pwrmem = (uint64_t)(sample * TO_FP(scale));
        }
    }

    if (chipid == 0)
    {
        json_object_set_new(node_obj, "power_node_watts", json_real(pwrsys));
    }

    json_t *socket_obj = json_object();
    json_object_set_new(node_obj, socketID, socket_obj);

    json_object_set_new(socket_obj, "power_cpu_watts", json_real(pwrproc));
    json_object_set_new(socket_obj, "power_mem_watts", json_real(pwrmem));
}

void json_get_thermal_sensors(int chipid, json_t *node_obj, const void *buf)
{
    struct occ_sensor_data_header *hb;
    struct occ_sensor_name *md;
    int i;

    hb = (struct occ_sensor_data_header *)(uint64_t)buf;
    md = (struct occ_sensor_name *)((uint64_t)hb + be32toh(hb->names_offset));

    char socketid[12];
    snprintf(socketid, 12, "socket_%d", chipid);

    json_t *socket_obj = json_object_get(node_obj, socketid);
    if (socket_obj == NULL)
    {
        socket_obj = json_object();
        json_object_set_new(node_obj, socketid, socket_obj);
    }

    json_t *cpu_obj = json_object();
    json_object_set_new(socket_obj, "CPU", cpu_obj);

    json_t *core_obj = json_object();
    json_object_set_new(cpu_obj, "Core", core_obj);

    json_t *mem_obj = json_object();
    json_object_set_new(cpu_obj, "Mem", mem_obj);

    for (i = 0; i < be16toh(hb->nr_sensors); i++)
    {
        uint32_t offset = be32toh(md[i].reading_offset);
        uint32_t scale = be32toh(md[i].scale_factor);
        uint64_t sample = 0;

        if (md[i].structure_type == OCC_SENSOR_READING_FULL)
        {
            sample = read_sensor(hb, offset, SENSOR_SAMPLE);
        }

        if (strncmp(md[i].name, "TEMPPROCTHRMC", 13) == 0)
        {
            char core_temp[32];
            char core_temp_value[5];
            strncpy(core_temp_value, md[i].name + 13, 5);
            int core_temp_int = atoi(core_temp_value);
            snprintf(core_temp, 32, "temp_celsius_core_%d", core_temp_int);

            json_object_set_new(core_obj, core_temp, json_integer(sample * TO_FP(scale)));
        }
        else if (strncmp(md[i].name, "TEMPDIMM", 8) == 0)
        {
            char mem_temp[32];
            char mem_temp_value[5];
            strncpy(mem_temp_value, md[i].name + 8, 5);
            int mem_temp_int = atoi(mem_temp_value);
            snprintf(mem_temp, 32, "temp_celsius_dimm_%d", mem_temp_int);

            json_object_set_new(mem_obj, mem_temp, json_integer(sample * TO_FP(scale)));
        }
    }
}

void json_get_frequency_sensors(int chipid, json_t *node_obj, const void *buf)
{
    struct occ_sensor_data_header *hb;
    struct occ_sensor_name *md;
    int i = 0;

    hb = (struct occ_sensor_data_header *)(uint64_t)buf;
    md = (struct occ_sensor_name *)((uint64_t)hb + be32toh(hb->names_offset));

    char socketID[12];
    snprintf(socketID, 12, "socket_%d", chipid);

    json_t *socket_obj = json_object_get(node_obj, socketID);
    if (socket_obj == NULL)
    {
        socket_obj = json_object();
        json_object_set_new(node_obj, socketID, socket_obj);
    }

    json_t *cpu_obj = json_object();
    json_object_set_new(socket_obj, "CPU", cpu_obj);

    for (i = 0; i < be16toh(hb->nr_sensors); i++)
    {
        uint32_t offset = be32toh(md[i].reading_offset);
        uint64_t sample = 0;

        if (md[i].structure_type == OCC_SENSOR_READING_FULL)
        {
            sample = read_sensor(hb, offset, SENSOR_SAMPLE);
        }

        if (strcmp(md[i].name, "FREQA") == 0)
        {
            json_object_set_new(cpu_obj, "cpu_avg_freq_mhz", json_integer(sample));
        }
    }
}
