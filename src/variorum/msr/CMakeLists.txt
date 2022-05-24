# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(variorum_msr_headers
  ${CMAKE_CURRENT_SOURCE_DIR}/msr_core.h
  CACHE INTERNAL "")

set(variorum_msr_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/msr_core.c
  CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${variorum_includes})

