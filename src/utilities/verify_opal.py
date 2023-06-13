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


def check_opal_files_presence(verbose):
    if verbose:
        print("-- Check if OPAL files exist")

    opal_files = [
        "/sys/firmware/opal/exports/occ_inband_sensors",
        "/sys/firmware/opal/powercap/system-powercap/powercap-current",
        "/sys/firmware/opal/powercap/system-powercap/powercap-max",
        "/sys/firmware/opal/powercap/system-powercap/powercap-min",
        "/sys/firmware/opal/psr/cpu_to_gpu_0",
        "/sys/firmware/opal/psr/cpu_to_gpu_8",
    ]

    ret = 0
    for i in opal_files:
        if os.path.isfile(i):
            if verbose:
                print("-- Check if", i, "is accessible by user:", i, "-- yes")
        else:
            if verbose:
                print("-- Check if", i, "is accessible by user:", i, "-- no")
            ret = 1

    return ret


def check_opal_files_access(verbose):
    if verbose:
        print("-- Check if OPAL files are accessible by user")

    ret = 0

    occ_file = "/sys/firmware/opal/exports/occ_inband_sensors"
    if os.access(occ_file, os.R_OK):
        if verbose:
            print("-- Check if OCC file is accessible by user:", occ_file, "-- yes")
    else:
        if verbose:
            print("-- Check if OCC file is accessible by user:", occ_file, "-- no")
        ret = 1

    pcap_curr_file = "/sys/firmware/opal/powercap/system-powercap/powercap-current"
    if os.access(pcap_curr_file, os.R_OK):
        if verbose:
            print(
                "-- Check if powercap-current file is accessible by user:",
                pcap_curr_file,
                "-- yes",
            )
    else:
        if verbose:
            print(
                "-- Check if powercap-current file is accessible by user:",
                pcap_curr_file,
                "-- no",
            )
        ret = 1

    pcap_max_file = "/sys/firmware/opal/powercap/system-powercap/powercap-max"
    if os.access(pcap_max_file, os.R_OK):
        if verbose:
            print(
                "-- Check if powercap-max file is accessible by user:",
                pcap_max_file,
                "-- yes",
            )
    else:
        if verbose:
            print(
                "-- Check if powercap-max file is accessible by user:",
                pcap_max_file,
                "-- no",
            )
        ret = 1

    pcap_min_file = "/sys/firmware/opal/powercap/system-powercap/powercap-min"
    if os.access(pcap_min_file, os.R_OK):
        if verbose:
            print(
                "-- Check if powercap-min file is accessible by user:",
                pcap_min_file,
                "-- yes",
            )
    else:
        if verbose:
            print(
                "-- Check if powercap-min file is accessible by user:",
                pcap_min_file,
                "-- no",
            )
        ret = 1

    cpu_to_gpu0_file = "/sys/firmware/opal/psr/cpu_to_gpu_0"
    if os.access(cpu_to_gpu0_file, os.R_OK):
        if verbose:
            print(
                "-- Check if cpu_to_gpu0 file is accessible by user:",
                cpu_to_gpu0_file,
                "-- yes",
            )
    else:
        if verbose:
            print(
                "-- Check if cpu_to_gpu0 file is accessible by user:",
                cpu_to_gpu0_file,
                "-- no",
            )
        ret = 1

    cpu_to_gpu8_file = "/sys/firmware/opal/psr/cpu_to_gpu_8"
    if os.access(cpu_to_gpu0_file, os.R_OK):
        if verbose:
            print(
                "-- Check if cpu_to_gpu8 file is accessible by user:",
                cpu_to_gpu8_file,
                "-- yes",
            )
    else:
        if verbose:
            print(
                "-- Check if cpu_to_gpu8 file is accessible by user:",
                cpu_to_gpu8_file,
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

    print("###################")
    print("# IBM OPAL Access #")
    print("###################")

    found_valid_interface = 0
    err = check_opal_files_presence(verbose)
    if err == 0:
        err = check_opal_files_access(verbose)
        if err == 0:
            print("-- Valid OPAL access")
            found_valid_kernel = 1
            sys.exit()
        else:
            print("-- Error: Invalid OPAL access")

    if found_valid_interface == 0:
        print("-- Error: No valid IBM OPAL device loaded")

#
