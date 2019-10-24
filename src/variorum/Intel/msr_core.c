// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

// Necessary for pread & pwrite.
#define _XOPEN_SOURCE 500

#include <errno.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <msr_core.h>
#include <config_architecture.h>
#include <variorum_error.h>

static uint64_t devidx(int socket, int core, int thread)
{
    int nsockets, ncores, nthreads;
    variorum_set_topology(&nsockets, &ncores, &nthreads);
    return (thread * nsockets * (ncores/nsockets)) + (socket * ncores/nsockets) + core;
}

static int batch_storage(struct msr_batch_array **batchsel, const int batchnum, unsigned **opssize)
{
    static struct msr_batch_array *batch = NULL;
    static unsigned arrsize = 1;
    static unsigned *size = NULL;
    int i;

    if (batch == NULL)
    {
#ifdef BATCH_DEBUG
        fprintf(stderr, "BATCH: initializing batch ops\n");
#endif
        arrsize = (batchnum + 1 > arrsize ? batchnum + 1 : arrsize);
//        printf("QQQ arrsize %d\n", arrsize);
        batch = (struct msr_batch_array *) calloc(arrsize, sizeof(struct msr_batch_array));
        size = (unsigned *) calloc(arrsize, sizeof(unsigned));
        for (i = 0; i < arrsize; i++)
        {
            size[i] = 0;
            batch[i].ops = NULL;
            batch[i].numops = 0;
        }
    }
    else if (batchnum + 1 > arrsize)
    {
#ifdef BATCH_DEBUG
        fprintf(stderr, "BATCH: reallocating array of batches for batch %d\n", batchnum);
#endif
        unsigned oldsize = arrsize;
        arrsize = batchnum + 1;
        batch = (struct msr_batch_array *) realloc(batch, arrsize * sizeof(struct msr_batch_array));
        size = (unsigned *) realloc(size, arrsize * sizeof(unsigned));
        for (; oldsize < arrsize; oldsize++)
        {
            batch[oldsize].ops = NULL;
            batch[oldsize].numops = 0;
            size[oldsize] = 0;
        }
    }
    if (batchsel == NULL)
    {
        variorum_error_handler("Loading uninitialized batch", VARIORUM_ERROR_MSR_BATCH, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
    }
    *batchsel = &batch[batchnum];
    if (opssize != NULL)
    {
        *opssize = &size[batchnum];
//        printf("opssize = %d\n", **opssize);
    }
    return 0;
}

static int compatibility_batch(int batchnum, int type)
{
    struct msr_batch_array *batch = NULL;
    int i;

    fprintf(stderr, "Warning: <variorum> No /dev/cpu/msr_batch, using compatibility batch: compatibility_batch(): %s: %s:%s::%d\n", strerror(errno), getenv("HOSTNAME"), __FILE__, __LINE__);
    if (batch_storage(&batch, batchnum, NULL))
    {
        return -1;
    }
    for (i = 0; i < batch->numops; i++)
    {
        if (type == BATCH_READ)
        {
            read_msr_by_idx(batch->ops[i].cpu, batch->ops[i].msr, (uint64_t *) &batch->ops[i].msrdata);
        }
        else
        {
            write_msr_by_idx(batch->ops[i].cpu, batch->ops[i].msr, (uint64_t)batch->ops[i].msrdata);
        }
    }
    return 0;
}

/// @brief Retrieve file descriptor per logical processor.
///
/// @param [in] dev_idx Unique logical processor identifier.
///
/// @return Unique file descriptor, else NULL.
static int *core_fd(const int dev_idx)
{
    static int init_core_fd = 0;
    static int *file_descriptors = NULL;
    static int nthreads;
    char *variorum_error_msg = (char *) malloc(NAME_MAX * sizeof(char));

    if (!init_core_fd)
    {
        init_core_fd = 1;
        variorum_set_topology(NULL, NULL, &nthreads);
        file_descriptors = (int *) malloc(nthreads * sizeof(int));
    }
    if (dev_idx < nthreads)
    {
        free(variorum_error_msg);
        return &(file_descriptors[dev_idx]);
    }
//    if (dev_idx == nthreads)
//    {
//        free(variorum_error_msg);
//        return file_descriptors;
//    }
    sprintf(variorum_error_msg, "Array reference %d out of bounds (max: %d)", dev_idx, nthreads);
    variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_ARRAY_BOUNDS, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
    free(variorum_error_msg);
    return NULL;
}

static int do_batch_op(int batchnum, int type)
{
    static int batchfd = 0;
    struct msr_batch_array *batch = NULL;
    int res, i, j;

    if (batchfd == 0)
    {
        if ((batchfd = open(MSR_BATCH_DIR, O_RDWR)) < 0)
        {
            perror(MSR_BATCH_DIR);
            batchfd = -1;
        }
    }
#ifdef USE_NO_BATCH
    return compatibility_batch(batchnum, type);
#endif
    if (batchfd < 0)
    {
        return compatibility_batch(batchnum, type);
    }
    if (batch_storage(&batch, batchnum, NULL))
    {
        return -1;
    }
#ifdef BATCH_DEBUG
    fprintf(stderr, "BATCH %d: %s MSRs, numops %u\n", batchnum, (type == BATCH_READ ? "reading" : "writing"), batch->numops);
#endif
    if (batch->numops <= 0)
    {
        variorum_error_handler("Using empty batch", VARIORUM_ERROR_MSR_BATCH, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    /* If current flag is the opposite type, switch the flags. */
    if ((type == BATCH_WRITE && batch->ops[0].isrdmsr) || (type == BATCH_READ && !batch->ops[0].isrdmsr))
    {
        __u8 readflag = (__u8) (type == BATCH_READ ? 1 : 0);
        for (j = 0; j < batch->numops; j++)
        {
            batch->ops[j].isrdmsr = readflag;
        }
    }
    res = ioctl(batchfd, X86_IOC_MSR_BATCH, batch);
    if (res < 0)
    {
        variorum_error_handler("IOctl failed, does /dev/cpu/msr_batch exist?", VARIORUM_ERROR_MSR_BATCH, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        for (i = 0; i < batch->numops; i++)
        {
            if (batch->ops[i].err)
            {
                fprintf(stderr, "CPU %d, RDMSR 0x%x, ERR (%s)\n", batch->ops[i].cpu, batch->ops[i].msr, strerror(batch->ops[i].err));
            }
        }
    }
#ifdef BATCH_DEBUG
    int k;
    for (k = 0; k < batch->numops; k++)
    {
        fprintf(stderr, "BATCH %d: msr 0x%x cpu %u data 0x%lx (at %p)\n", batchnum, batch->ops[k].msr, batch->ops[k].cpu, (uint64_t)batch->ops[k].msrdata, &batch->ops[k].msrdata);
    }
#endif
    return 0;
}

int sockets_assert(const unsigned *socket, const int location, const char *file)
{
    char *variorum_error_msg = malloc(NAME_MAX * sizeof(char));
    int nsockets;
    variorum_set_topology(&nsockets, NULL, NULL);

    if (*socket > nsockets)
    {
        sprintf(variorum_error_msg, "Requested invalid socket %d (max: %d)", *socket, nsockets);
        variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        free(variorum_error_msg);
        return VARIORUM_ERROR_PLATFORM_ENV;
    }
    free(variorum_error_msg);
    return 0;
}

int threads_assert(const unsigned *thread, const int location, const char *file)
{
    char *variorum_error_msg = malloc(NAME_MAX * sizeof(char));
    int nthreads;
    variorum_set_topology(NULL, NULL, &nthreads);

    if (*thread > nthreads)
    {
        sprintf(variorum_error_msg, "Requested invalid thread %d (max: %d)", *thread, nthreads);
        variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        free(variorum_error_msg);
        return VARIORUM_ERROR_PLATFORM_ENV;
    }
    free(variorum_error_msg);
    return 0;
}

int cores_assert(const unsigned *core, const int location, const char *file)
{
    char *variorum_error_msg = malloc(NAME_MAX * sizeof(char));
    int ncores;
    variorum_set_topology(NULL, &ncores, NULL);

    if (*core > ncores)
    {
        sprintf(variorum_error_msg, "Requested invalid core %d (max: %d)", *core, ncores);
        variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_PLATFORM_ENV, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        free(variorum_error_msg);
        return VARIORUM_ERROR_PLATFORM_ENV;
    }
    free(variorum_error_msg);
    return 0;
}

int stat_module(char *filename, int *kerneltype, int *dev_idx)
{
    struct stat statbuf;
    char *variorum_error_msg = malloc(NAME_MAX * sizeof(char));

    if (*kerneltype == 3)
    {
        if (stat(filename, &statbuf))
        {
            fprintf(stderr, "Warning: <variorum> Could not stat %s: stat_module(): %s: %s:%s::%d\n", filename, strerror(errno), getenv("HOSTNAME"), __FILE__, __LINE__);
            *kerneltype = 1;
            free(variorum_error_msg);
            return -1;
        }
        if (!(statbuf.st_mode & S_IRUSR) || !(statbuf.st_mode & S_IWUSR))
        {
            fprintf(stderr, "Warning: <variorum> Incorrect permissions on msr_whitelist: stat_module(): %s:%s::%d\n", getenv("HOSTNAME"), __FILE__, __LINE__);
            *kerneltype = 1;
            free(variorum_error_msg);
            return -1;
        }
        *kerneltype = 0;
        free(variorum_error_msg);
        return 0;
    }
    if (stat(filename, &statbuf))
    {
        fprintf(stderr, "Warning: <variorum> Incorrect permissions on msr_whitelist: stat_module(): %s:%s::%d\n", getenv("HOSTNAME"), __FILE__, __LINE__);
        if (*kerneltype)
        {
            /* Could not find any msr module so exit. */
            sprintf(variorum_error_msg, "Could not stat file %s on dev %d", filename, *dev_idx);
            variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_MSR_MODULE, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
            free(variorum_error_msg);
            return VARIORUM_ERROR_RAPL_INIT;
        }
        /* Could not find msr_safe module so try the msr module. */
        sprintf(variorum_error_msg, "Could not stat file %s on dev %d", filename, *dev_idx);
        variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_MSR_MODULE, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        *kerneltype = 1;
        /* Restart loading file descriptors for each device. */
        *dev_idx = -1;
        free(variorum_error_msg);
        return 0;
    }
    if (!(statbuf.st_mode & S_IRUSR) || !(statbuf.st_mode & S_IWUSR))
    {
        sprintf(variorum_error_msg, "Read/write permissions denied for file %s on dev %d", filename, *dev_idx);
        variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_MSR_MODULE, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        *kerneltype = 1;
        *dev_idx = -1;
        if (kerneltype != NULL)
        {
            /* Could not find any msr module with RW permissions, so exit. */
            variorum_error_handler("Could not find any valid MSR module with correct permissions", VARIORUM_ERROR_MSR_MODULE, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
            free(variorum_error_msg);
            return VARIORUM_ERROR_MSR_MODULE;
        }
    }
    free(variorum_error_msg);
    return 0;
}

int finalize_msr(void)
{
    int ret = 0;
    int dev_idx;
    int *file_descriptor = NULL;
    char *variorum_error_msg = (char *) malloc(NAME_MAX * sizeof(char));
    int nthreads;
    variorum_set_topology(NULL, NULL, &nthreads);

    for (dev_idx = 0; dev_idx < nthreads; dev_idx++)
    {
        file_descriptor = core_fd(dev_idx);
        if (file_descriptor != NULL)
        {
            ret = close(*file_descriptor);
            if (ret)
            {
                sprintf(variorum_error_msg, "Could not close file for device %d", dev_idx);
                variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_MSR_CLOSE, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
            }
            else
            {
                *file_descriptor = 0;
            }
        }
    }
    //file_descriptor = core_fd(nthreads);
    //free(file_descriptor);
    free(variorum_error_msg);
    return ret;
}

int init_msr(void)
{
    int dev_idx;
    int ret;
    int *file_descriptor = NULL;
    char filename[FILENAME_SIZE];
    int kerneltype = 3; // 0 is msr_safe, 1 is msr
    char *variorum_error_msg = malloc(NAME_MAX * sizeof(char));
    int nsockets, ncores, nthreads;

    variorum_set_topology(&nsockets, &ncores, &nthreads);
    snprintf(filename, FILENAME_SIZE, "/dev/cpu/msr_whitelist");
    stat_module(filename, &kerneltype, 0);
    /* Open the file descriptor for each device's msr interface. */
    for (dev_idx = 0; dev_idx < nthreads; dev_idx++)
    {
        /* Use the msr_safe module, or default to the msr module. */
        if (kerneltype)
        {
            snprintf(filename, FILENAME_SIZE, "/dev/cpu/%d/msr", dev_idx);
        }
        else
        {
            snprintf(filename, FILENAME_SIZE, "/dev/cpu/%d/msr_safe", dev_idx);
        }
        if (stat_module(filename, &kerneltype, &dev_idx) < 0)
        {
            ret = -1;
            free(variorum_error_msg);
            //free(file_descriptor);
            return ret;
        }
        if (dev_idx < 0)
        {
            continue;
        }
        /* Open the msr module, else return the appropriate error message. */
        file_descriptor = core_fd(dev_idx);
        *file_descriptor = open(filename, O_RDWR);
        if (*file_descriptor == -1)
        {
            sprintf(variorum_error_msg, "Could not open file for device %d", dev_idx);
            variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_RAPL_INIT, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
            if (kerneltype)
            {
                sprintf(variorum_error_msg, "Could not open any valid MSR module for device %d", dev_idx);
                variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_RAPL_INIT, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
                /* Could not open any msr module, so exit. */
                free(variorum_error_msg);
                //free(file_descriptor);
                return VARIORUM_ERROR_RAPL_INIT;
            }
            kerneltype = 1;
            dev_idx--;
        }
    }
    free(variorum_error_msg);
    return 0;
}

int write_msr_by_coord(unsigned socket, unsigned core, unsigned thread, off_t msr, uint64_t val)
{
    sockets_assert(&socket, __LINE__, __FILE__);
    cores_assert(&core, __LINE__, __FILE__);
    threads_assert(&thread, __LINE__, __FILE__);

    return write_msr_by_idx(devidx(socket, core, thread), msr, val);
}

int read_msr_by_coord(unsigned socket, unsigned core, unsigned thread, off_t msr, uint64_t *val)
{
#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d (read_msr_by_coord) socket=%d core=%d thread=%d msr=%lu (0x%lx)\n", getenv("HOSTNAME"), __FILE__, __LINE__, socket, core, thread, msr, msr);
#endif
    sockets_assert(&socket, __LINE__, __FILE__);
    cores_assert(&core, __LINE__, __FILE__);
    threads_assert(&thread, __LINE__, __FILE__);
    if (val == NULL)
    {
        variorum_error_handler("Received NULL pointer for val", VARIORUM_ERROR_MSR_READ, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return VARIORUM_ERROR_MSR_READ;
    }
    return read_msr_by_idx(devidx(socket, core, thread), msr, val);
}

int read_msr_by_idx(int dev_idx, off_t msr, uint64_t *val)
{
    int rc;
    int *file_descriptor = NULL;
    char *variorum_error_msg = malloc(NAME_MAX * sizeof(char));

    file_descriptor = core_fd(dev_idx);
    if (file_descriptor == NULL)
    {
        free(variorum_error_msg);
        return -1;
    }
#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d (read_msr_by_idx) msr=%lu (0x%lx)\n", getenv("HOSTNAME"), __FILE__, __LINE__, msr, msr);
#endif
    rc = pread(*file_descriptor, (void*)val, (size_t)sizeof(uint64_t), msr);
    if (rc != sizeof(uint64_t))
    {
        sprintf(variorum_error_msg, "Pread failed on dev_idx %d", dev_idx);
        variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_MSR_READ, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        free(variorum_error_msg);
        return VARIORUM_ERROR_MSR_READ;
    }
    free(variorum_error_msg);
    return 0;
}

int write_msr_by_idx(int dev_idx, off_t msr, uint64_t val)
{
    int rc;
    int *file_descriptor = NULL;
    char *variorum_error_msg = malloc(NAME_MAX * sizeof(char));

    file_descriptor = core_fd(dev_idx);
    if (file_descriptor == NULL)
    {
        free(variorum_error_msg);
        return -1;
    }
#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d (write_msr_by_idx) msr=%lu (0x%lx)\n", getenv("HOSTNAME"), __FILE__, __LINE__, msr, msr);
#endif
    rc = pwrite(*file_descriptor, &val, (size_t)sizeof(uint64_t), msr);
    if (rc != sizeof(uint64_t))
    {
        sprintf(variorum_error_msg, "Pwrite failed on dev_idx %d", dev_idx);
        variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_MSR_WRITE, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        free(variorum_error_msg);
        return VARIORUM_ERROR_MSR_WRITE;
    }
    free(variorum_error_msg);
    return 0;
}

int allocate_batch(int batchnum, size_t bsize)
{
    unsigned *size = NULL;
    struct msr_batch_array *batch = NULL;
    int i;

#ifdef BATCH_DEBUG
    fprintf(stderr, "BATCH: allocating batch %d\n", batchnum);
#endif
    //printf("First call to batch_storage\n");
    if (batch_storage(&batch, batchnum, &size))
    {
        return -1;
    }
#ifdef BATCH_DEBUG
    fprintf(stderr, "BATCH: batch %d is at %p\n", batchnum, batch);
#endif
    *size = bsize;
#ifdef BATCH_DEBUG
    printf("allocate_batch batchnum %d has %d ops\n", batchnum, *size);
#endif
    if (batch->ops != NULL)
    {
        variorum_error_handler("Conflicting batch pointers", VARIORUM_ERROR_MSR_BATCH, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
    }
    batch->ops = (struct msr_batch_op *) calloc(*size, sizeof(struct msr_batch_op));
    for (i = batch->numops; i < *size; i++)
    {
        batch->ops[i].err = 0;
    }
    return 0;
}

int load_socket_batch(off_t msr, uint64_t **val, int batchnum)
{
    int dev_idx, val_idx;
    int nsockets, ncores, nthreads;
    variorum_set_topology(&nsockets, &ncores, &nthreads);

    if (val == NULL)
    {
        variorum_error_handler("Given uninitialized array", VARIORUM_ERROR_MSR_BATCH, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return VARIORUM_ERROR_MSR_BATCH;
    }

    for (dev_idx = 0, val_idx = 0; dev_idx < ncores; dev_idx += ncores/nsockets, val_idx++)
    {
        create_batch_op(msr, dev_idx, &val[val_idx%(nsockets*(nthreads/ncores))], batchnum);
    }
    return 0;
}

int load_thread_batch(off_t msr, uint64_t **val, int batchnum)
{
    int dev_idx, val_idx;
    int nsockets, ncores, nthreads;
    variorum_set_topology(&nsockets, &ncores, &nthreads);

    if (val == NULL)
    {
        variorum_error_handler("Given uninitialized array", VARIORUM_ERROR_MSR_BATCH, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return VARIORUM_ERROR_MSR_BATCH;
    }

#ifdef VARIORUM_DEBUG
    fprintf(stderr, "%s %s::%d (read_all_threads) msr=%lu (0x%lx)\n", getenv("HOSTNAME"), __FILE__, __LINE__, msr, msr);
#endif
    for (dev_idx = 0, val_idx = 0; dev_idx < nthreads; dev_idx++, val_idx++)
    {
        create_batch_op(msr, dev_idx, &val[val_idx], batchnum);
    }
    return 0;
}

int read_batch(const int batchnum)
{
    return do_batch_op(batchnum, BATCH_READ);
}

int write_batch(const int batchnum)
{
    return do_batch_op(batchnum, BATCH_WRITE);
}

int create_batch_op(off_t msr, uint64_t cpu, uint64_t **dest, const int batchnum)
{
    struct msr_batch_array *batch = NULL;
    unsigned *size = NULL;

#ifdef BATCH_DEBUG
    fprintf(stderr, "BATCH: creating new batch operation\n");
#endif
    if (batch_storage(&batch, batchnum, &size))
    {
        return -1;
    }

#ifdef BATCH_DEBUG
    fprintf(stderr, "BATCH: batch %d is at %p\n", batchnum, batch);
#endif
    if (batch->numops > *size)
    {
        char *variorum_error_msg = (char *) malloc(NAME_MAX * sizeof(char));
        sprintf(variorum_error_msg, "Batch %d is full, you likely used the wrong size (max: %d)", batchnum, batch->numops);
        variorum_error_handler(variorum_error_msg, VARIORUM_ERROR_MSR_BATCH, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        free(variorum_error_msg);
        return VARIORUM_ERROR_MSR_BATCH;
    }

    batch->numops++;
#ifdef BATCH_DEBUG
    printf("create_batch_op: batchnum = %d numops = %d size = %d\n", batchnum, batch->numops, *size);
#endif
    batch->ops[batch->numops-1].msr = msr;
    batch->ops[batch->numops-1].cpu = (__u16) cpu;
    batch->ops[batch->numops-1].isrdmsr = (__u8) 1;
    batch->ops[batch->numops-1].err = 0;
    *dest = (uint64_t *) &batch->ops[batch->numops - 1].msrdata;
#ifdef BATCH_DEBUG
    fprintf(stderr, "BATCH: destination of msr %lx on core %lx (at %p) is %p\n", msr, cpu, dest, &batch->ops[batch->numops - 1].msrdata);
    fprintf(stderr, "\tbatch numops is %d\n", batch->numops);
#endif
    return 0;
}
