// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <inttypes.h>

#include "arm_util.h"
#include <variorum_error.h>
#include <variorum_timers.h>

unsigned m_num_package;
char m_hostname[1024];

int read_file_ui64(const int file, uint64_t *val)
{
    char buf[32];
    int bytes_read = read(file, buf, 32);
    if (bytes_read == 0)
    {
        return 0;
    }
    sscanf(buf, "%"SCNu64, val);

    return bytes_read;
}

int write_file_ui64(const int file, uint64_t val)
{
    char buf[32];
    sprintf(buf, "%"PRIu64, val);
    int bytes_written = write(file, buf, 32);
    if (bytes_written <= 0)
    {
        return 0;
    }

    return bytes_written;
}

int read_array_ui64(const int fd, uint64_t **array)
{
    int iter = 0;
    char buf[4096];
    int bytes_read = read(fd, buf, 4096);

    if (bytes_read == 0)
    {
        return 0;
    }

    int nfreq = 0;
    char *fptr = buf;

    for (nfreq = 0; fptr[nfreq] != '\0'; fptr[nfreq] == ' ' ? nfreq++ : * (fptr++));

    uint64_t *val_array = (uint64_t *) malloc(sizeof(uint64_t) * nfreq);
    char *tok = strtok(buf, " ");
    while (tok && iter < nfreq)
    {
        val_array[iter++] = atoll(tok);
        tok = strtok(NULL, " \n");
    }
    *array = val_array;
    return nfreq;
}

void init_arm(void)
{
    /* Collect number of packages and GPUs per package */
    variorum_get_topology(&m_num_package, NULL, NULL, P_ARM_CPU_IDX);

    /* Save hostname */
    gethostname(m_hostname, 1024);
}

void shutdown_arm(void)
{
    printf("Shutdown ARM\n");
}
