#!/usr/bin/env python
#
# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

from ctypes import create_unicode_buffer, addressof, c_char_p

from pyVariorum import variorum

if __name__ == "__main__":
    v = variorum.variorum()
    print("\n=== Running Variorum JSON Get Node Power:")
    jsonbuf = create_unicode_buffer(1000)
    bufptr = c_char_p(addressof(jsonbuf))
    v.variorum_get_node_power_json(bufptr)
    print(bufptr.value.decode("utf-8"))
