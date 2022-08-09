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


def runCapAPIexamples(v):
    print("\n=== Running Variorum Cap Best Effort Node Power Limit (input = 500W):")
    v.variorum_cap_best_effort_node_power_limit(500)
    print("\n######## Verifying Power Cap with Variorum Print Power Limit:")
    v.variorum_print_power_limit()


def runJSONAPIexamples(v):
    print("\n=== Running Variorum JSON Get Node Power:")
    jsonbuf = create_unicode_buffer(1000)
    bufptr = c_char_p(addressof(jsonbuf))
    v.variorum_get_node_power_json(bufptr)
    print(bufptr.value.decode("utf-8"))


if __name__ == "__main__":
    v = variorum()
    runPrintAPIexamples(v)
    runCapAPIexamples(v)
    runJSONAPIexamples(v)
