// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <variorum_error.h>

void variorum_error_message(enum variorum_error_e err, char *msg, size_t size)
{
    switch(err)
    {
        case VARIORUM_ERROR_RUNTIME:
            strncpy(msg, "_ERROR_VARIORUM_RUNTIME", size);
            break;
        case VARIORUM_ERROR_RAPL_INIT:
            strncpy(msg, "_ERROR_VARIORUM_RAPL_INIT", size);
            break;
        case VARIORUM_ERROR_MSR_CLOSE:
            strncpy(msg, "_ERROR_VARIORUM_MSR_CLOSE", size);
            break;
        case VARIORUM_ERROR_MSR_READ:
            strncpy(msg, "_ERROR_VARIORUM_MSR_READ", size);
            break;
        case VARIORUM_ERROR_MSR_WRITE:
            strncpy(msg, "_ERROR_VARIORUM_MSR_WRITE", size);
            break;
        case VARIORUM_ERROR_MSR_BATCH:
            strncpy(msg, "_ERROR_VARIORUM_MSR_BATCH", size);
            break;
        case VARIORUM_ERROR_ARRAY_BOUNDS:
            strncpy(msg, "_ERROR_VARIORUM_ARRAY_BOUNDS", size);
            break;
        case VARIORUM_ERROR_PLATFORM_ENV:
            strncpy(msg, "_ERROR_VARIORUM_PLATFORM_ENV", size);
            break;
        case VARIORUM_ERROR_MSR_MODULE:
            strncpy(msg, "_ERROR_VARIORUM_MSR_MODULE", size);
            break;
        case VARIORUM_ERROR_INVAL:
            strncpy(msg, "_ERROR_VARIORUM_INVAL", size);
            break;
        case VARIORUM_ERROR_UNSUPPORTED_PLATFORM:
            strncpy(msg, "_ERROR_VARIORUM_UNSUPPORTED_PLATFORM", size);
            break;
        case VARIORUM_ERROR_UNSUPPORTED_ARCH:
            strncpy(msg, "_ERROR_VARIORUM_UNSUPPORTED_ARCH", size);
            break;
        case VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR:
            strncpy(msg, "_ERROR_VARIORUM_UNINITIALIZED_FUNC_PTR", size);
            break;
        default:
            strncpy(msg, "Unknown variorum error code.", size);
            break;
    }
    if (size > 0)
    {
        msg[size-1] = '\0';
    }
}

char *get_variorum_error_message(enum variorum_error_e err)
{
    char *brief_msg = (char *) malloc(NAME_MAX * sizeof(char));
    if (err)
    {
        err = err;
    }
    else
    {
        err = VARIORUM_ERROR_RUNTIME;
    }
    variorum_error_message(err, brief_msg, NAME_MAX);
    return brief_msg;
}

void variorum_error_handler(const char *desc, int err, const char *host, const char *file, const char *func, int line)
{
    char *str = get_variorum_error_message(err);
    fprintf(stderr, "%s:%s:%s():%d: %s: %s\n", host, file, func, line, str, desc);
    free(str);
}
