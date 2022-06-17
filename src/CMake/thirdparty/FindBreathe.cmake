# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

find_program(BREATHE_EXECUTABLE
             NAMES breathe
             DOC "Path to breathe executable")

# Handle REQUIRED and QUIET arguments
# this will also set BREATHE_FOUND to true if BREATHE_EXECUTABLE exists
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(breathe
                                  "Failed to locate breathe executable"
                                  BREATHE_EXECUTABLE)
