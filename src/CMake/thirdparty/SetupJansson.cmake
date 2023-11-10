# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# First check for user-specified JANSSON_DIR
if(JANSSON_DIR)
    message(STATUS "Looking for Jansson using JANSSON_DIR = ${JANSSON_DIR}")

    find_path(JANSSON_INCLUDE_DIRS
        NAMES jansson.h
        HINTS ${JANSSON_DIR}/include
    )
    if(NOT JANSSON_INCLUDE_DIRS)
        MESSAGE(FATAL_ERROR "Could not find jansson.h in ${JANSSON_DIR}/include")
    endif()

    find_library(JANSSON_LIBRARY
        NAMES libjansson.so
        HINTS ${JANSSON_DIR}/lib
    )
    if(NOT JANSSON_LIBRARY)
        MESSAGE(FATAL_ERROR "Could not find libjansson.so in ${JANSSON_DIR}/lib")
    endif()

    set(JANSSON_FOUND TRUE CACHE INTERNAL "")
    set(JANSSON_DIR ${JANSSON_DIR} CACHE PATH "" FORCE)
    include_directories(${JANSSON_INCLUDE_DIRS})

    message(STATUS "FOUND jansson")
    message(STATUS " [*] JANSSON_DIR = ${JANSSON_DIR}")
    message(STATUS " [*] JANSSON_INCLUDE_DIRS = ${JANSSON_INCLUDE_DIRS}")
    message(STATUS " [*] JANSSON_LIBRARY = ${JANSSON_LIBRARY}")
# If JANSSON_DIR not specified, then try to automatically find the JANSSON header
# and library
elseif(NOT JANSSON_FOUND)
    find_path(JANSSON_INCLUDE_DIRS
        NAMES jansson.h
    )

    find_library(JANSSON_LIBRARY
        NAMES libjansson.so
    )

    if(JANSSON_INCLUDE_DIRS AND JANSSON_LIBRARY)
        set(JANSSON_FOUND TRUE CACHE INTERNAL "")
        set(JANSSON_DIR ${JANSSON_DIR} CACHE PATH "" FORCE)
        set(JANSSON_INCLUDE_DIRS ${JANSSON_INCLUDE_DIRS} CACHE PATH "" FORCE)
        set(JANSSON_LIBRARY ${JANSSON_LIBRARY} CACHE PATH "" FORCE)
        include_directories(${JANSSON_INCLUDE_DIRS})

        message(STATUS "FOUND jansson using find_library()")
        message(STATUS " [*] JANSSON_INCLUDE_DIRS = ${JANSSON_INCLUDE_DIRS}")
        message(STATUS " [*] JANSSON_LIBRARY = ${JANSSON_LIBRARY}")
    endif()
endif()

# Abort if all methods fail
if(NOT JANSSON_FOUND)
    message(FATAL_ERROR "Jansson support needs explict JANSSON_DIR")
endif()
