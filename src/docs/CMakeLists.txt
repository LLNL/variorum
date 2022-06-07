# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

if(SPHINX_FOUND)
    add_subdirectory(sphinx)
endif()

if(DOXYGEN_FOUND)
    add_subdirectory(dox)
    add_subdirectory(breathe)
endif()
