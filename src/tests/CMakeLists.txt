# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# add variorum tests
add_subdirectory("variorum")

# add system environment tests
add_subdirectory("system-env")
