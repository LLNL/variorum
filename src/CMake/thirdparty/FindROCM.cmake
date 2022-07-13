# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# First check for user-specified ROCM_DIR
if(ROCM_DIR)
    message(STATUS "Looking for ROCM using ROCM_DIR = ${ROCM_DIR}")

    set(ROCM_FOUND TRUE CACHE INTERNAL "")
    set(VARIORUM_ROCM_DIR ${ROCM_DIR} CACHE INTERNAL "")
    set(ROCM_DIR ${ROCM_DIR} CACHE PATH "" FORCE)
    set(ROCM_INCLUDE_DIRS ${ROCM_DIR}/include/ CACHE PATH "" FORCE)
    set(ROCM_LIBRARY ${ROCM_DIR}/lib/librocm_smi64.so.5 CACHE PATH "" FORCE)
    include_directories(${ROCM_INCLUDE_DIRS})

    message(STATUS "FOUND ROCM")
    message(STATUS " [*] ROCM_DIR = ${ROCM_DIR}")
    message(STATUS " [*] ROCM_INCLUDE_DIRS = ${ROCM_INCLUDE_DIRS}")
    message(STATUS " [*] ROCM_LIBRARY = ${ROCM_LIBRARY}")
# If ROCM_DIR not specified, then try to automatically find the ROCM header
# and library
elseif(NOT ROCM_FOUND)
    find_path(ROCM_INCLUDE_DIRS
        NAMES rocm_smi.h
    )

    find_library(ROCM_LIBRARY
        NAMES librocm_smi64.so.5
    )

    if(ROCM_INCLUDE_DIRS AND ROCM_LIBRARY)
        set(ROCM_FOUND TRUE CACHE INTERNAL "")
        set(VARIORUM_ROCM_DIR ${ROCM_DIR} CACHE INTERNAL "")
        set(ROCM_DIR ${ROCM_DIR} CACHE PATH "" FORCE)
        set(ROCM_INCLUDE_DIRS ${ROCM_INCLUDE_DIRS} CACHE PATH "" FORCE)
        set(ROCM_LIBRARY ${ROCM_LIBRARY} CACHE PATH "" FORCE)
        include_directories(${ROCM_INCLUDE_DIRS})

        message(STATUS "FOUND ROCM using find_library()")
        message(STATUS " [*] ROCM_INCLUDE_DIRS = ${ROCM_INCLUDE_DIRS}")
        message(STATUS " [*] ROCM_LIBRARY = ${ROCM_LIBRARY}")
    endif()
endif()

# Abort if all methods fail
if(NOT ROCM_FOUND)
    message(FATAL_ERROR "AMD GPU support needs explict ROCM_DIR")
endif()
