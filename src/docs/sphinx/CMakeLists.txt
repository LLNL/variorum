# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
               "${CMAKE_CURRENT_BINARY_DIR}/conf.py")

add_sphinx_target(variorum_sphinx)
