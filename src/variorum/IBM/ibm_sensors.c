// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <ibm_sensors.h>

unsigned long read_counter(const struct occ_sensor_data_header *hb, uint32_t offset)
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

unsigned long read_sensor(const struct occ_sensor_data_header *hb, uint32_t offset, int attr)
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

void print_power_sensors(int chipid, int long_ver, FILE *output, const void *buf)
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
            fprintf(output, "_IBMPOWER | Host | Socket | PWRSYS | PWRPROC | PWRMEM | PWRGPU | Timestamp\n");
        }
    }

    gettimeofday(&now, NULL);

    hb = (struct occ_sensor_data_header *)(uint64_t)buf;
    md = (struct occ_sensor_name *)((uint64_t)hb + be32toh(hb->names_offset));

    for (i = 0; i < be16toh(hb->nr_sensors); i++)
    {
        uint32_t offset = be32toh(md[i].reading_offset);
        uint32_t scale = be32toh(md[i].scale_factor);
        uint64_t sample;

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
            pwrsys = (uint64_t) (sample * TO_FP(scale));
        }
        if (strcmp(md[i].name, "PWRPROC") == 0)
        {
            pwrproc = (uint64_t) (sample * TO_FP(scale));
        }
        if (strcmp(md[i].name, "PWRMEM") == 0)
        {
            pwrmem = (uint64_t) (sample * TO_FP(scale));
        }
        if (strcmp(md[i].name, "PWRGPU") == 0)
        {
            pwrgpu = (uint64_t) (sample * TO_FP(scale));
        }
    }

    if (long_ver == 0)
    {
        fprintf(output, "_IBMPOWER | %s | %d | %lu | %lu | %lu | %lu | %lf\n",
                hostname, chipid, pwrsys, pwrproc, pwrmem, pwrgpu,
                now.tv_sec-start.tv_sec + (now.tv_usec-start.tv_usec)/1000000.0);
    }
    else
    {
        fprintf(output, "_IBMPOWER Hostname: %s Socket: %d PWRSYS: %lu Watts PWRPROC: %lu Watts"
                " PWRMEM: %lu Watts PWRGPU: %lu Watts Timestamp: %lf seconds\n",
                hostname, chipid, pwrsys, pwrproc, pwrmem, pwrgpu,
                now.tv_sec-start.tv_sec + (now.tv_usec-start.tv_usec)/1000000.0);
    }
}

void print_all_sensors_header(int chipid, FILE *output, const void *buf)
{
    struct occ_sensor_data_header *hb;
    struct occ_sensor_name *md;
    int i = 0;
    static struct timeval start;
    struct timeval now;

    hb = (struct occ_sensor_data_header *)(uint64_t)buf;
    md = (struct occ_sensor_name *)((uint64_t)hb + be32toh(hb->names_offset));

    fprintf(output, "_IBMPOWER%d | Timestamp_s | Hostname | Socket", chipid);

    for (i = 0; i < be16toh(hb->nr_sensors); i++)
    {
        uint32_t offset = be32toh(md[i].reading_offset);

        if (be16toh(md[i].type) == OCC_SENSOR_TYPE_POWER)
        {
            fprintf(output, " | %s_Scale_%s | %s_Energy_J", md[i].name, md[i].units, md[i].name);
        }
        else
        {
            fprintf(output, " | %s_%s", md[i].name, md[i].units);
        }
    }
    fprintf(output,"\n"); // Add end of line.
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

    fprintf(output, "_IBMPOWER%d | %lf | %s | %d", chipid, now.tv_sec-start.tv_sec + (now.tv_usec-start.tv_usec)/1000000.0, hostname, chipid);

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
            fprintf(output, " | %lu | %lu", (uint64_t)(sample * TO_FP(scale)), (uint64_t)(energy / TO_FP(freq)));
        }
        else
        {
            fprintf(output, " | %lu", (uint64_t)(sample * TO_FP(scale)));
        }
    }
    fprintf(output,"\n"); // Add end of line.
}
