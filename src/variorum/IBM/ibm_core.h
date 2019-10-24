#ifndef IBM_CORE_HEADER_INCLUDE
#define IBM_CORE_HEADER_INCLUDE

/* Tapasya Note:
 * Headers and read functions here are borrowed from
 * Shilpasri's original code.
 * This is already available from IBM.
 * Need to check open source licensing information. */

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

/* Documentation for the OCC Sensors
 * and the structures in this header file
 * is available in README_IBM_OCC. */

#define MAX_OCCS                    8
#define MAX_CHARS_SENSOR_NAME       16
#define MAX_CHARS_SENSOR_UNIT       4

#define OCC_SENSOR_DATA_BLOCK_OFFSET 0x00580000
#define OCC_SENSOR_DATA_BLOCK_SIZE   0x00025800

#define TO_FP(f)    ((f >> 8) * pow(10, ((int8_t)(f & 0xFF))))

enum occ_sensor_type {
    OCC_SENSOR_TYPE_GENERIC     = 0x0001,
    OCC_SENSOR_TYPE_CURRENT     = 0x0002,
    OCC_SENSOR_TYPE_VOLTAGE     = 0x0004,
    OCC_SENSOR_TYPE_TEMPERATURE = 0x0008,
    OCC_SENSOR_TYPE_UTILIZATION = 0x0010,
    OCC_SENSOR_TYPE_TIME        = 0x0020,
    OCC_SENSOR_TYPE_FREQUENCY   = 0x0040,
    OCC_SENSOR_TYPE_POWER       = 0x0080,
    OCC_SENSOR_TYPE_PERFORMANCE = 0x0200,
};

enum occ_sensor_location {
    OCC_SENSOR_LOC_SYSTEM    = 0x0001,
    OCC_SENSOR_LOC_PROCESSOR = 0x0002,
    OCC_SENSOR_LOC_PARTITION = 0x0004,
    OCC_SENSOR_LOC_MEMORY    = 0x0008,
    OCC_SENSOR_LOC_VRM       = 0x0010,
    OCC_SENSOR_LOC_OCC       = 0x0020,
    OCC_SENSOR_LOC_CORE      = 0x0040,
    OCC_SENSOR_LOC_GPU       = 0x0080,
    OCC_SENSOR_LOC_QUAD      = 0x0100,
};

enum sensor_struct_type {
    OCC_SENSOR_READING_FULL    = 0x01,
    OCC_SENSOR_READING_COUNTER = 0x02,
};

enum sensor_attr {
    SENSOR_SAMPLE,
    SENSOR_ACCUMULATOR,
};

struct occ_sensor_data_header {
    uint8_t  valid;
    uint8_t  version;
    uint16_t nr_sensors;
    uint8_t  reading_version;
    uint8_t  pad[3];
    uint32_t names_offset;
    uint8_t  names_version;
    uint8_t  name_length;
    uint16_t reserved;
    uint32_t reading_ping_offset;
    uint32_t reading_pong_offset;
} __attribute__((__packed__));

struct occ_sensor_name {
    char     name[MAX_CHARS_SENSOR_NAME];
    char     units[MAX_CHARS_SENSOR_UNIT];
    uint16_t gsid;
    uint32_t freq;
    uint32_t scale_factor;
    uint16_t type;
    uint16_t location;
    uint8_t  structure_type;
    uint32_t reading_offset;
    uint8_t  sensor_data;
    uint8_t  pad[8];
} __attribute__((__packed__));

struct occ_sensor_record {
    uint16_t gsid;
    uint64_t timestamp;
    uint16_t sample;
    uint16_t sample_min;
    uint16_t sample_max;
    uint16_t csm_min;
    uint16_t csm_max;
    uint16_t profiler_min;
    uint16_t profiler_max;
    uint16_t job_scheduler_min;
    uint16_t job_scheduler_max;
    uint64_t accumulator;
    uint32_t update_tag;
    uint8_t  pad[8];
} __attribute__((__packed__));

struct occ_sensor_counter {
    uint16_t gsid;
    uint64_t timestamp;
    uint64_t accumulator;
    uint8_t  sample;
    uint8_t  pad[5];
} __attribute__((__packed__));

unsigned long read_counter(struct occ_sensor_data_header *hb, uint32_t offset)
{
    struct occ_sensor_counter *sping, *spong;
    struct occ_sensor_counter *sensor = NULL;
    uint8_t *ping, *pong;

    ping = (uint8_t *)((uint64_t)hb + be32toh(hb->reading_ping_offset));
    pong = (uint8_t *)((uint64_t)hb + be32toh(hb->reading_pong_offset));
    sping = (struct occ_sensor_counter *)((uint64_t)ping + offset);
    spong = (struct occ_sensor_counter *)((uint64_t)pong + offset);

    if (*ping && *pong) {
        if (be64toh(sping->timestamp) > be64toh(spong->timestamp))
            sensor = sping;
        else
            sensor = spong;
    } else if (*ping && !*pong) {
        sensor = sping;
    } else if (!*ping && *pong) {
        sensor = spong;
    } else if (!*ping && !*pong) {
        return 0;
    }

    return be64toh(sensor->accumulator);
}

unsigned long read_sensor(struct occ_sensor_data_header *hb, uint32_t offset, int attr)
{
    struct occ_sensor_record *sping, *spong;
    struct occ_sensor_record *sensor = NULL;
    uint8_t *ping, *pong;

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

void print_all_sensors(void *buf);

int main(int argc, char *argv[])
{
    void *buf;
    int fd;
    int rc;
    int bytes;
    int chipid;

    if (argc < 2)
    {
        chipid = 0;
    }
    else
    {
        chipid = atoi(argv[1]);
    }

    fd = open("/sys/firmware/opal/exports/occ_inband_sensors", O_RDONLY);
    if (fd < 0)
    {
        printf("Failed to open occ_inband_sensors file\n");
        return -1;
    }

    buf = malloc(OCC_SENSOR_DATA_BLOCK_SIZE);
    if (!buf)
    {
        printf("Failed to allocate\n");
        return -1;
    }

    lseek(fd, chipid * OCC_SENSOR_DATA_BLOCK_SIZE, SEEK_CUR);
    for (rc = bytes = 0; bytes < OCC_SENSOR_DATA_BLOCK_SIZE; bytes += rc)
    {
        rc = read(fd, buf + bytes, OCC_SENSOR_DATA_BLOCK_SIZE - bytes);
        //printf("rc %d (0x%lx)\n", rc, rc);
        //printf("buf 0x%lx\n", buf);
        //printf("bytes %d (0x%lx) < %d (0x%lx)\n", bytes, bytes, OCC_SENSOR_DATA_BLOCK_SIZE, OCC_SENSOR_DATA_BLOCK_SIZE);
        //printf("Reading at 0x%lx\n", buf+bytes);
        if (!rc || rc < 0)
        {
            break;
        }
    }
    //printf("FOR LOOP COUNTER: %d\n", idx);

    if (bytes != OCC_SENSOR_DATA_BLOCK_SIZE)
    {
        printf("Failed to read data\n");
        return -1;
    }
    //printf("size of buf %d\n", sizeof(buf));

    print_all_sensors(buf);
}

#endif
