# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

from ctypes import create_unicode_buffer, addressof, c_char_p
from pyVariorum import variorum


def runPrintAPIexamples(v):
    print("\n=== Running Variorum Print Power:")
    v.variorum_print_power()

    print("\n=== Running Variorum Print Power (Verbose):")
    v.variorum_print_verbose_power()

    print("\n=== Running Variorum Print Power Limit:")
    v.variorum_print_power_limit()

    print("\n=== Running Variorum Print Power Limit (Verbose):")
    v.variorum_print_verbose_power_limit()

    print("\n=== Running Variorum Print Thermals:")
    v.variorum_print_thermals()

    print("\n=== Running Variorum Print Thermals (Verbose):")
    v.variorum_print_verbose_thermals()

    print("\n=== Running Variorum Print Counters:")
    v.variorum_print_counters()

    print("\n=== Running Variorum Print Counters (Verbose):")
    v.variorum_print_verbose_counters()

    print("\n=== Running Variorum Print Frequency:")
    v.variorum_print_frequency()

    print("\n=== Running Variorum Print Frequency (Verbose):")
    v.variorum_print_verbose_frequency()

    print("\n=== Running Variorum Print GPU Utilization:")
    v.variorum_print_gpu_utilization()

    print("\n=== Running Variorum Print GPU Utilization (Verbose):")
    v.variorum_print_verbose_gpu_utilization()

    print("\n=== Running Variorum Print Hyperthreading:")
    v.variorum_print_hyperthreading()

    print("\n=== Running Variorum Print Topology:")
    v.variorum_print_topology()

    print("\n=== Running Variorum Print Features:")
    v.variorum_print_features()

    print("\n=== Running Variorum Print Turbo:")
    v.variorum_print_turbo()

    print("\n=== Running Variorum Print Available Frequencies:")
    v.variorum_print_available_frequencies()

    print("\n=== Running Variorum Get Current Version:")
    buf = create_unicode_buffer(64)
    buf = v.variorum_get_current_version()
    print("Variorum version is: " + buf.decode("utf-8"))


def runCapAPIexamples(v):
    print("\n=== Running Variorum Cap Best Effort Node Power Limit (input = 500W):")
    v.variorum_cap_best_effort_node_power_limit(500)
    print("\n######## Verifying Power Cap with Variorum Print Power Limit:")
    v.variorum_print_power_limit()

    print("\n=== Running Variorum Cap Each Socket Power Limit (input = 100W):")
    v.variorum_cap_each_socket_power_limit(100)

    print("\n=== Running Variorum Cap GPU Power Ratio (input = 80%):")
    v.variorum_cap_gpu_power_ratio(80)


def runJSONAPIexamples(v):
    print("\n=== Running Variorum JSON Get Node Power:")
    jsonbuf = create_unicode_buffer(1000)
    bufptr = c_char_p(addressof(jsonbuf))
    v.variorum_get_node_power_json(bufptr)
    print(bufptr.value.decode("utf-8"))

    print("\n=== Running Variorum JSON Get Node Power Domain Info:")
    jsonbuf = create_unicode_buffer(1000)
    bufptr = c_char_p(addressof(jsonbuf))
    v.variorum_get_node_power_domain_info_json(bufptr)
    print(bufptr.value.decode("utf-8"))


if __name__ == "__main__":
    v = variorum.variorum()
    runPrintAPIexamples(v)
    runCapAPIexamples(v)
    runJSONAPIexamples(v)
