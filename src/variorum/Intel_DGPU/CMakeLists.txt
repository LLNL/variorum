# Copyright 2019-2021 Lawrence Livermore National Security, LLC, and
# other Variorum Project Developers. See the top-level LICENSE file
# for details.  SPDX-License-Identifier: MIT

set(variorum_intel_dgpu_headers
  ${CMAKE_CURRENT_SOURCE_DIR}/DGPU.h
  ${CMAKE_CURRENT_SOURCE_DIR}/power_features.h
  CACHE INTERNAL "")

set(variorum_intel_dgpu_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/DGPU.c
  ${CMAKE_CURRENT_SOURCE_DIR}/power_features.c
  CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${variorum_includes})

add_library(variorum_intel_dgpu OBJECT
            ${variorum_intel_dgpu_sources}
            ${variorum_intel_dgpu_headers})

### Shared libraries need PIC
set_property(TARGET ${variorum_intel_dgpu} PROPERTY POSITION_INDEPENDENT_CODE 1)
