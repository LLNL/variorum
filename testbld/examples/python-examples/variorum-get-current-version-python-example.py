#!/usr/bin/env python
#
# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

from ctypes import create_unicode_buffer

from pyVariorum import variorum

if __name__ == "__main__":
    v = variorum.variorum()
    print("\n=== Running Variorum Get Current Version:")
    buf = create_unicode_buffer(64)
    buf = v.variorum_get_current_version()
    print("Variorum version is: " + buf.decode("utf-8"))
