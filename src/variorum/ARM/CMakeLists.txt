# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(variorum_arm_headers
  ${CMAKE_CURRENT_SOURCE_DIR}/ARM_Juno_r2.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ARM_Neoverse_N1.h
  ${CMAKE_CURRENT_SOURCE_DIR}/juno_r2_power_features.h
  ${CMAKE_CURRENT_SOURCE_DIR}/neoverse_N1_power_features.h
  ${CMAKE_CURRENT_SOURCE_DIR}/arm_util.h
  CACHE INTERNAL "")

set(variorum_arm_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/ARM_Juno_r2.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ARM_Neoverse_N1.c
  ${CMAKE_CURRENT_SOURCE_DIR}/juno_r2_power_features.c
  ${CMAKE_CURRENT_SOURCE_DIR}/neoverse_N1_power_features.c
  ${CMAKE_CURRENT_SOURCE_DIR}/arm_util.c
  CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${variorum_includes})

add_library(variorum_arm OBJECT
            ${variorum_arm_sources}
            ${variorum_arm_headers})

### Shared libraries need PIC
set_property(TARGET ${variorum_arm} PROPERTY POSITION_INDEPENDENT_CODE 1)
