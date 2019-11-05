// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef VARIORUM_ERROR_H_INCLUDE
#define VARIORUM_ERROR_H_INCLUDE

#ifndef NAME_MAX
#define NAME_MAX 1024
#endif

enum variorum_error_e
{
    VARIORUM_ERROR_RUNTIME                = -1,
    VARIORUM_ERROR_RAPL_INIT              = -2,
    VARIORUM_ERROR_MSR_CLOSE              = -3,
    VARIORUM_ERROR_MSR_READ               = -4,
    VARIORUM_ERROR_ARRAY_BOUNDS           = -5,
    VARIORUM_ERROR_PLATFORM_ENV           = -6,
    VARIORUM_ERROR_MSR_MODULE             = -7,
    VARIORUM_ERROR_MSR_WRITE              = -8,
    VARIORUM_ERROR_MSR_BATCH              = -9,
    VARIORUM_ERROR_INVAL                  = -10,
    VARIORUM_ERROR_UNSUPPORTED_PLATFORM   = -11,
    VARIORUM_ERROR_UNSUPPORTED_ARCH       = -12,
    VARIORUM_ERROR_UNINITIALIZED_FUNC_PTR = -13
};

void variorum_error_handler(const char *desc,
                            int err,
                            const char *host,
                            const char *file,
                            const char *func,
                            int line);

char *get_variorum_error_message(enum variorum_error_e err);

void variorum_error_message(enum variorum_error_e err,
                            char *msg,
                            size_t size);

#endif
