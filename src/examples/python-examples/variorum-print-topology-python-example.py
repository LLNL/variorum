#!/usr/bin/env python
#
# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

from pyVariorum import variorum

if __name__ == "__main__":
    v = variorum.variorum()
    print("\n=== Running Variorum Print Topology:")
    v.variorum_print_topology()
