# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(variorum_amd_gpu_headers
  ${CMAKE_CURRENT_SOURCE_DIR}/instinctGPU.h
  ${CMAKE_CURRENT_SOURCE_DIR}/amd_gpu_power_features.h
  CACHE INTERNAL "")

set(variorum_amd_gpu_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/instinctGPU.c
  ${CMAKE_CURRENT_SOURCE_DIR}/amd_gpu_power_features.c
  CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${variorum_includes})

add_library(variorum_amd_gpu OBJECT
            ${variorum_amd_gpu_sources}
            ${variorum_amd_gpu_headers})

### Shared libraries need PIC
set_property(TARGET ${variorum_amd_gpu} PROPERTY POSITION_INDEPENDENT_CODE 1)
