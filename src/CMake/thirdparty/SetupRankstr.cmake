# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# First check for user-specified RANKSTR_DIR
if(RANKSTR_DIR)
    message(STATUS "Looking for rankstr using RANKSTR_DIR = ${RANKSTR_DIR}")

    set(RANKSTR_FOUND TRUE CACHE INTERNAL "")
    set(VARIORUM_RANKSTR_DIR ${RANKSTR_DIR} CACHE INTERNAL "")
    set(RANKSTR_DIR ${RANKSTR_DIR} CACHE PATH "" FORCE)
    find_path(RANKSTR_INCLUDE_DIRS
        NAMES rankstr_mpi.h
        HINTS ${RANKSTR_DIR}/include
    )

    find_library(RANKSTR_LIBRARY
        NAMES librankstr.so
        HINTS ${RANKSTR_DIR}/lib
    )

    include_directories(${RANKSTR_INCLUDE_DIRS})

    message(STATUS "FOUND rankstr")
    message(STATUS " [*] RANKSTR_DIR = ${RANKSTR_DIR}")
    message(STATUS " [*] RANKSTR_INCLUDE_DIRS = ${RANKSTR_INCLUDE_DIRS}")
    message(STATUS " [*] RANKSTR_LIBRARY = ${RANKSTR_LIBRARY}")
# If RANKSTR_DIR not specified, then try to automatically find the RANKSTR header
# and library
elseif(NOT RANKSTR_FOUND)
    find_path(RANKSTR_INCLUDE_DIRS
        NAMES rankstr_mpi.h
    )

    find_library(RANKSTR_LIBRARY
        NAMES librankstr.so
    )

    if(RANKSTR_INCLUDE_DIRS AND RANKSTR_LIBRARY)
        set(RANKSTR_FOUND TRUE CACHE INTERNAL "")
        set(VARIORUM_RANKSTR_DIR ${RANKSTR_DIR} CACHE INTERNAL "")
        set(RANKSTR_DIR ${RANKSTR_DIR} CACHE PATH "" FORCE)
        set(RANKSTR_INCLUDE_DIRS ${RANKSTR_INCLUDE_DIRS} CACHE PATH "" FORCE)
        set(RANKSTR_LIBRARY ${RANKSTR_LIBRARY} CACHE PATH "" FORCE)
        include_directories(${RANKSTR_INCLUDE_DIRS})

        message(STATUS "FOUND rankstr using find_library()")
        message(STATUS " [*] RANKSTR_INCLUDE_DIRS = ${RANKSTR_INCLUDE_DIRS}")
        message(STATUS " [*] RANKSTR_LIBRARY = ${RANKSTR_LIBRARY}")
    endif()
endif()

# Abort if all methods fail
if(NOT RANKSTR_FOUND)
    message(FATAL_ERROR "Rankstr support needs explict RANKSTR_DIR")
endif()
