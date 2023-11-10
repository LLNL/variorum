#!/usr/bin/env python
#
# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

from __future__ import print_function

import os
import sys

import getopt

hwmon_files = [
    "/sys/class/hwmon/hwmon0/power1_input",
    "/sys/class/hwmon/hwmon0/power2_input",
    "/sys/class/hwmon/hwmon0/power3_input",
    "/sys/class/hwmon/hwmon0/power4_input",
    "/sys/class/hwmon/hwmon0/temp1_input",
    "/sys/class/hwmon/hwmon0/temp2_input",
    "/sys/class/hwmon/hwmon0/temp3_input",
    "/sys/class/hwmon/hwmon0/temp4_input",
]

cpufreq_read_files = [
    "/sys/devices/system/cpu/cpufreq/policy0/scaling_cur_freq",
    "/sys/devices/system/cpu/cpufreq/policy0/scaling_available_frequencies",
    "/sys/devices/system/cpu/cpufreq/policy1/scaling_cur_freq",
    "/sys/devices/system/cpu/cpufreq/policy1/scaling_available_frequencies",
]

cpufreq_write_files = [
    "/sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed",
    "/sys/devices/system/cpu/cpufreq/policy1/scaling_setspeed",
]


def check_hwmon_files_presence(verbose):
    if verbose:
        print("-- Check if ARM hwmon files exist")

    ret = 0
    for i in hwmon_files:
        if os.path.isfile(i):
            if verbose:
                print("-- Check if", i, "is present on the system -- yes")
        else:
            if verbose:
                print("-- Check if", i, "is present on the system -- no")
            ret = 1

    return ret


def check_hwmon_files_access(verbose):
    if verbose:
        print("-- Check if the hwmon files are accessible by user")

    ret = 0

    for fname in hwmon_files:
        if os.access(fname, os.R_OK):
            if verbose:
                print(
                    "-- Check if the hwmon interface is accessible by user:",
                    fname,
                    "-- yes",
                )
        else:
            if verbose:
                print(
                    "-- Check if the hwmon interface is accessible by user:",
                    fname,
                    "-- no",
                )
            ret = 1

    return ret


def check_cpufreq_files_presence(verbose):
    if verbose:
        print("-- Check if ARM cpufreq files exist")

    ret = 0
    for i in cpufreq_read_files:
        if os.path.isfile(i):
            if verbose:
                print("-- Check if", i, "is present on the system -- yes")
        else:
            if verbose:
                print("-- Check if", i, "is present on the system -- no")
            ret = 1

    for i in cpufreq_write_files:
        if os.path.isfile(i):
            if verbose:
                print("-- Check if", i, "is present on the system -- yes")
        else:
            if verbose:
                print("-- Check if", i, "is present on the system -- no")
            ret = 1

    return ret


def check_cpufreq_files_access(verbose):
    if verbose:
        print("-- Check if the cpufreq files are accessible by user")

    ret = 0

    for fname in cpufreq_read_files:
        if os.access(fname, os.R_OK):
            if verbose:
                print(
                    "-- Check if the cpufreq interface is accessible by user:",
                    fname,
                    "-- yes",
                )
        else:
            if verbose:
                print(
                    "-- Check if the cpufreq interface is accessible by user:",
                    fname,
                    "-- no",
                )
            ret = 1

    for fname in cpufreq_write_files:
        if os.access(fname, os.W_OK):
            if verbose:
                print(
                    "-- Check if the cpufreq interface is writable by user:",
                    fname,
                    "-- yes",
                )
        else:
            if verbose:
                print(
                    "-- Check if the cpufreq interface is writable by user:",
                    fname,
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

    print("####################")
    print("# ARM hwmon access #")
    print("####################")

    err = check_hwmon_files_presence(verbose)
    if err == 0:
        err = check_hwmon_files_access(verbose)
        if err == 0:
            print("-- Valid hwmon access permissions found.")
        else:
            print("-- Error: Inadequate hwmon access permissions found.")
    else:
        print("-- Error: hwmon not loaded on the system.")

    print("######################")
    print("# ARM cpufreq access #")
    print("######################")

    err = check_cpufreq_files_presence(verbose)
    if err == 0:
        err = check_cpufreq_files_access(verbose)
        if err == 0:
            print("-- Valid cpufreq access permissions found.")
            sys.exit()
        else:
            print("-- Error: Inadequate cpufreq access permissions found.")
    else:
        print("-- Error: cpufreq not configured on the system.")

#
