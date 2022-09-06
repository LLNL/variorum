# Copyright 2022 Lawrence Livermore National Security, LLC, and other
# Variorum Project Developers. See the top-level LICENSE file for
# details.  SPDX-License-Identifier: MIT

# First check for user-specified APMIDG_DIR
if(APMIDG_DIR)
    message(STATUS "Looking for APMIDG using APMIDG_DIR = ${APMIDG_DIR}")

    set(APMIDG_FOUND TRUE CACHE INTERNAL "")
    set(VARIORUM_APMIDG_DIR ${APMIDG_DIR} CACHE INTERNAL "")
    set(APMIDG_DIR ${APMIDG_DIR} CACHE PATH "" FORCE)
    set(APMIDG_INCLUDE_DIRS ${APMIDG_DIR}/include CACHE PATH "" FORCE)
    set(APMIDG_LIBRARY ${APMIDG_DIR}/lib64/libapmidg.so CACHE PATH "" FORCE)
    include_directories(${APMIDG_INCLUDE_DIRS})

    message(STATUS "FOUND APMIDG")
    message(STATUS " [*] APMIDG_DIR = ${APMIDG_DIR}")
    message(STATUS " [*] APMIDG_INCLUDE_DIRS = ${APMIDG_INCLUDE_DIRS}")
    message(STATUS " [*] APMIDG_LIBRARY = ${APMIDG_LIBRARY}")
# If APMIDG_DIR not specified, then try to automatically find the APMIDG header
# and library
elseif(NOT APMIDG_FOUND)
    find_path(APMIDG_INCLUDE_DIRS
        NAMES libapmidg.h
    )

    find_library(APMIDG_LIBRARY
        NAMES libapmidg.so
    )

    if(APMIDG_INCLUDE_DIRS AND APMIDG_LIBRARY)
        set(APMIDG_FOUND TRUE CACHE INTERNAL "")
        set(VARIORUM_APMIDG_DIR ${APMIDG_DIR} CACHE INTERNAL "")
        set(APMIDG_DIR ${APMIDG_DIR} CACHE PATH "" FORCE)
        set(APMIDG_INCLUDE_DIRS ${APMIDG_INCLUDE_DIRS} CACHE PATH "" FORCE)
        set(APMIDG_LIBRARY ${APMIDG_LIBRARY} CACHE PATH "" FORCE)
        include_directories(${APMIDG_INCLUDE_DIRS})

        message(STATUS "FOUND APMIDG using find_library()")
        message(STATUS " [*] APMIDG_INCLUDE_DIRS = ${APMIDG_INCLUDE_DIRS}")
        message(STATUS " [*] APMIDG_LIBRARY = ${APMIDG_LIBRARY}")
    endif()
endif()

# Abort if all methods fail
if(NOT APMIDG_FOUND)
    message(FATAL_ERROR "Intel discrete GPU support needs explict APMIDG_DIR")
endif()
