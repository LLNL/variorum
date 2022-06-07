# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(variorum_arm_headers
  ${CMAKE_CURRENT_SOURCE_DIR}/ARM.h
  ${CMAKE_CURRENT_SOURCE_DIR}/power_features.h
  CACHE INTERNAL "")

set(variorum_arm_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/ARM.c
  ${CMAKE_CURRENT_SOURCE_DIR}/power_features.c
  CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${variorum_includes})

add_library(variorum_arm OBJECT
            ${variorum_arm_sources}
            ${variorum_arm_headers})

### Shared libraries need PIC
set_property(TARGET ${variorum_arm} PROPERTY POSITION_INDEPENDENT_CODE 1)
