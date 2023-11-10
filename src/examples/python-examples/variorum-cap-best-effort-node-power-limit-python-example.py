#!/usr/bin/env python
#
# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

from pyVariorum import variorum

if __name__ == "__main__":
    v = variorum.variorum()
    print("\n=== Running Variorum Cap Best Effort Node Power Limit (input = 500W):")
    v.variorum_cap_best_effort_node_power_limit(500)
    print("\n######## Verifying Power Cap with Variorum Print Power Limit:")
    v.variorum_print_power_limit()
