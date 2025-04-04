#!/usr/bin/env python3
#
# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

from __future__ import print_function
from packaging import version

import os
import sys
import pathlib
import getopt


# check msr_safe kernel is loaded
def check_msr_safe_kernel_loaded(verbose):
    if verbose:
        print("-- Check if msr_safe kernel is loaded")

    lsmod = os.popen("lsmod | grep msr_safe")
    is_msr_safe_kernel_loaded = lsmod.read()

    if is_msr_safe_kernel_loaded:
        if verbose:
            print("-- Check if msr_safe kernel is loaded -- yes")
        return 0
    else:
        if verbose:
            print("-- Check if msr_safe kernel is loaded -- no")
        return 1


# check msr_safe kernel files are character devices
def check_msr_safe_files_character_devices(verbose):
    if verbose:
        print("-- Check if msr_safe kernel files are character devices")

    nproc = os.popen("nproc")
    nprocs_tmp = nproc.read().strip()
    ncpus = int(nprocs_tmp)
    ret = 0

    for i in range(0, ncpus):
        cpu_file = "/dev/cpu/" + str(i) + "/msr_safe"

        p = pathlib.Path(cpu_file)
        if p.is_char_device():
            if verbose:
                print(
                    "-- Check if msr_safe kernel files are character devices:",
                    cpu_file,
                    "-- yes",
                )
        else:
            if verbose:
                print(
                    "-- Check if msr_safe kernel files are character devices:",
                    cpu_file,
                    "-- no",
                )
            ret = 1

    msr_safe_version = open("/sys/module/msr_safe/version").readline().strip()
    if version.parse(msr_safe_version) < version.parse("1.4"):
        wl_file = "/dev/cpu/msr_whitelist"
    else:
        wl_file = "/dev/cpu/msr_allowlist"

    p = pathlib.Path(wl_file)
    if p.is_char_device():
        if verbose:
            print(
                "-- Check if msr_safe kernel files are character devices:",
                wl_file,
                "-- yes",
            )
    else:
        if verbose:
            print(
                "-- Check if msr_safe kernel files are character devices:",
                wl_file,
                "-- no",
            )
        ret = 1

    batch_file = "/dev/cpu/msr_batch"
    p = pathlib.Path(batch_file)
    if p.is_char_device():
        if verbose:
            print(
                "-- Check if msr_safe kernel files are character devices:",
                batch_file,
                "-- yes",
            )
    else:
        if verbose:
            print(
                "-- Check if msr_safe kernel files are character devices:",
                batch_file,
                "-- no",
            )
        ret = 1

    return ret


# check msr_safe kernel files are accessible
def check_msr_safe_files_access(verbose):
    if verbose:
        print("-- Check if msr_safe kernel files are accessible by user")

    nproc = os.popen("nproc")
    nprocs_tmp = nproc.read().strip()
    ncpus = int(nprocs_tmp)
    ret = 0

    for i in range(0, ncpus):
        cpu_file = "/dev/cpu/" + str(i) + "/msr_safe"

        if os.access(cpu_file, os.R_OK and os.W_OK):
            if verbose:
                print(
                    "-- Check if msr_safe kernel files are accessible by user:",
                    cpu_file,
                    "-- yes",
                )
        else:
            if verbose:
                print(
                    "-- Check if msr_safe kernel files are accessible by user:",
                    cpu_file,
                    "-- no",
                )
            ret = 1

    msr_safe_version = open("/sys/module/msr_safe/version").readline().strip()
    if version.parse(msr_safe_version) < version.parse("1.4"):
        wl_file = "/dev/cpu/msr_whitelist"
    else:
        wl_file = "/dev/cpu/msr_allowlist"

    if os.access(wl_file, os.R_OK):
        if verbose:
            print(
                "-- Check if msr_safe kernel files are accessible by user:",
                wl_file,
                "-- yes",
            )
    else:
        if verbose:
            print(
                "-- Check if msr_safe kernel files are accessible by user:",
                wl_file,
                "-- no",
            )
        ret = 1

    batch_file = "/dev/cpu/msr_batch"
    if os.access(batch_file, os.R_OK and os.W_OK):
        if verbose:
            print(
                "-- Check if msr_safe kernel files are accessible by user:",
                batch_file,
                "-- yes",
            )
    else:
        if verbose:
            print(
                "-- Check if msr_safe kernel files are accessible by user:",
                batch_file,
                "-- no",
            )
        ret = 1

    return ret


# check msr kernel is loaded
def check_msr_kernel_loaded(verbose):
    if verbose:
        print("-- Check if msr kernel is loaded")
    lsmod = os.popen("lsmod | grep msr$")
    is_msr_kernel_loaded = lsmod.read()

    if is_msr_kernel_loaded:
        if verbose:
            print("-- Check if msr kernel is loaded -- yes")
        return 0
    else:
        if verbose:
            print("-- Check if msr kernel is loaded -- no")
        return 1


# check msr kernel files are character devices
def check_msr_files_character_devices(verbose):
    if verbose:
        print("-- Check if msr kernel files are character devices")

    nproc = os.popen("nproc")
    nprocs_tmp = nproc.read().strip()
    ncpus = int(nprocs_tmp)
    ret = 0

    for i in range(0, ncpus - 1):
        cpu_file = "/dev/cpu/" + str(i) + "/msr"

        p = pathlib.Path(cpu_file)
        if p.is_char_device():
            if verbose:
                print(
                    "-- Check if msr kernel files are character devices:",
                    cpu_file,
                    "-- yes",
                )
        else:
            if verbose:
                print(
                    "-- Check if msr kernel files are character devices:",
                    cpu_file,
                    "-- no",
                )
            ret = 1

    return ret


# check msr kernel files are accessible
def check_msr_files_access(verbose):
    if verbose:
        print("-- Check if msr kernel files are accessible by user")

    nproc = os.popen("nproc")
    nprocs_tmp = nproc.read().strip()
    ncpus = int(nprocs_tmp)
    ret = 0

    for i in range(0, ncpus - 1):
        cpu_file = "/dev/cpu/" + str(i) + "/msr"

        if os.access(cpu_file, os.R_OK and os.W_OK):
            if verbose:
                print(
                    "-- Check if msr kernel files are accessible by user:",
                    cpu_file,
                    "-- yes",
                )
        else:
            if verbose:
                print(
                    "-- Check if msr kernel files are accessible by user:",
                    cpu_file,
                    "-- no",
                )
            ret = 1

    return ret


if __name__ == "__main__":
    usage = """

  Options:
  -h  This man page.
  -v  Enable verbose printout.
"""
    verbose = False

    try:
        argv = sys.argv[1:]
        opts, args = getopt.getopt(argv, "hv")
    except getopt.GetoptError:
        print("Usage:", sys.argv[0], usage)
        sys.exit(2)

    for opt, arg in opts:
        if opt == "-h":
            print("Usage:", sys.argv[0], usage)
            sys.exit()
        elif opt == "-v":
            verbose = True

    print("######################")
    print("# x86 CPU MSR Access #")
    print("######################")

    found_valid_kernel = 0

    # if msr-safe kernel is loaded
    err = check_msr_safe_kernel_loaded(verbose)
    if err == 0:
        err = check_msr_safe_files_character_devices(
            verbose
        ) or check_msr_safe_files_access(verbose)
        if err == 0:
            print("-- Valid kernel loaded: msr-safe")
            found_valid_kernel = 1
            sys.exit()
        else:
            print("-- Error: Invalid msr-safe kernel loaded")

    # if msr kernel is loaded
    err = check_msr_kernel_loaded(verbose)
    if err == 0:
        err = check_msr_files_character_devices(
            verbose
        ) or check_msr_files_access(verbose)
        if err == 0:
            print("-- Valid kernel loaded: msr")
            found_valid_kernel = 1
            sys.exit()
        else:
            print("-- Error: Invalid msr kernel loaded")

    if found_valid_kernel == 0:
        print(
            "-- Error: No valid x86 CPU MSR access device loaded (e.g., msr, msr-safe)"
        )

#
