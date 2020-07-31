# Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

include(CMakeFindDependencyMacro)

########################################################################
# Setup HWLOC
########################################################################
# If ZZZ_DIR not set, use known install path for HWLOC
if(NOT HWLOC_INCLUDE_LIB AND NOT HWLOC_LIB)
    set(HWLOC_LIB ${VARIORUM_HWLOC_LIB})
    set(HWLOC_INCLUDE_LIB ${VARIORUM_HWLOC_INCLUDE_DIR})
endif()

########################################################################
# Check for HWLOC_DIR
########################################################################
if(NOT HWLOC_INCLUDE_LIB AND NOT HWLOC_LIB)
    message(FATAL_ERROR "Could not find HWLOC. HWLOC requires HWLOC_LIB and HWLOC_INCLUDE_LIB.")
endif()


########################################################################
# Setup JANSSON
########################################################################
# If ZZZ_DIR not set, use known install path for JANSSON
if(NOT JANSSON_INCLUDE AND NOT JANSSON_LIB)
    set(JANSSON_LIB ${VARIORUM_JANSSON_LIB})
    set(JANSSON_INCLUDE ${VARIORUM_JANSSON_INCLUDE_DIR})
endif()

########################################################################
# Check for JANSSON_DIR
########################################################################
if(NOT JANSSON_INCLUDE AND NOT JANSSON_LIB)
    message(FATAL_ERROR "Could not find JANSSON. JANSSON requires JANSSON_LIB and JANSSON_INCLUDE.")
endif()
