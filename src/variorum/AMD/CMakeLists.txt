# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(variorum_amd_headers
  ${CMAKE_CURRENT_SOURCE_DIR}/epyc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/amd_power_features.h
  CACHE INTERNAL "")

set(variorum_amd_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/epyc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/amd_power_features.c
  CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${variorum_includes})

add_library(variorum_amd OBJECT
            ${variorum_amd_sources}
            ${variorum_amd_headers})

### Shared libraries need PIC
set_property(TARGET ${variorum_amd} PROPERTY POSITION_INDEPENDENT_CODE 1)
