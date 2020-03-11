// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef IBM_CORE_HEADER_INCLUDE
#define IBM_CORE_HEADER_INCLUDE

/* Tapasya Note:
 * Headers and read counter/sensor functions here are
 * borrowed from Shilpasri Bhat's original code on GitHub
 * */

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

/* Documentation for the OCC Sensors and the structures in this header file
 * is available in README_IBM_OCC.
 * */

#define MAX_OCCS                    8
#define MAX_CHARS_SENSOR_NAME       16
#define MAX_CHARS_SENSOR_UNIT       4

#define OCC_SENSOR_DATA_BLOCK_OFFSET 0x00580000
#define OCC_SENSOR_DATA_BLOCK_SIZE   0x00025800

#define TO_FP(f)  ((f >> 8) * pow(10, ((int8_t)(f & 0xFF))))

enum occ_sensor_type
{
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

enum occ_sensor_location
{
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

enum sensor_struct_type
{
    OCC_SENSOR_READING_FULL    = 0x01,
    OCC_SENSOR_READING_COUNTER = 0x02,
};

enum sensor_attr
{
    SENSOR_SAMPLE,
    SENSOR_ACCUMULATOR,
};

struct occ_sensor_data_header
{
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

struct occ_sensor_name
{
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

struct occ_sensor_record
{
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

struct occ_sensor_counter
{
    uint16_t gsid;
    uint64_t timestamp;
    uint64_t accumulator;
    uint8_t  sample;
    uint8_t  pad[5];
} __attribute__((__packed__));


void print_power_sensors(int chipid,
                         int long_ver,
                         FILE *output,
                         const void *buf);

void print_all_sensors_header(int chipid,
                              FILE *output,
                              const void *buf);

void print_all_sensors(int chipid,
                       FILE *output,
                       const void *buf);

unsigned long read_counter(const struct occ_sensor_data_header *hb,
                           uint32_t offset);

unsigned long read_sensor(const struct occ_sensor_data_header *hb,
                          uint32_t offset,
                          int attr);

#endif
