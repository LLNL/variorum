# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(variorum_intel_gpu_headers
  ${CMAKE_CURRENT_SOURCE_DIR}/GPU.h
  ${CMAKE_CURRENT_SOURCE_DIR}/intel_gpu_power_features.h
  CACHE INTERNAL "")

set(variorum_intel_gpu_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/GPU.c
  ${CMAKE_CURRENT_SOURCE_DIR}/intel_gpu_power_features.c
  CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${variorum_includes})

add_library(variorum_intel_gpu OBJECT
            ${variorum_intel_gpu_sources}
            ${variorum_intel_gpu_headers})

### Shared libraries need PIC
set_property(TARGET ${variorum_intel_gpu} PROPERTY POSITION_INDEPENDENT_CODE 1)
