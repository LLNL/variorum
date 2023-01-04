from __future__ import print_function

import os
import sys

description = "Variorum debug utility"

def setup_parser(subparser):
    subparser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        default=True,
        dest="verbose",
        help="verbose mode",
    )


def debug(parser, args):
    print("debug mode")
    arm_debug(args.verbose)


def arm_debug(verbose):
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


    def _check_hwmon_files_presence(verbose):
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


    def _check_hwmon_files_access(verbose):
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


    def _check_cpufreq_files_presence(verbose):
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


    def _check_cpufreq_files_access(verbose):
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


    print("####################")
    print("# ARM hwmon access #")
    print("####################")

    err = _check_hwmon_files_presence(verbose)
    if err == 0:
        err = _check_hwmon_files_access(verbose)
        if err == 0:
            print("-- Valid hwmon access permissions found.")
        else:
            print("-- Error: Inadequate hwmon access permissions found.")
    else:
        print("-- Error: hwmon not loaded on the system.")

    print("######################")
    print("# ARM cpufreq access #")
    print("######################")

    err = _check_cpufreq_files_presence(verbose)
    if err == 0:
        err = _check_cpufreq_files_access(verbose)
        if err == 0:
            print("-- Valid cpufreq access permissions found.")
            sys.exit()
        else:
            print("-- Error: Inadequate cpufreq access permissions found.")
    else:
        print("-- Error: cpufreq not configured on the system.")
