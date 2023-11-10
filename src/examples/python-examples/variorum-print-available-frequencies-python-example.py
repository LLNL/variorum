#!/usr/bin/env python
#
# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

from pyVariorum import variorum

if __name__ == "__main__":
    v = variorum.variorum()
    print("\n=== Running Variorum Print Available Frequencies:")
    v.variorum_print_available_frequencies()
