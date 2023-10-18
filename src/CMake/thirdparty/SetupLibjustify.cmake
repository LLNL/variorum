# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# First check for user-specified LIBJUSTIFY_DIR
if(LIBJUSTIFY_DIR)
    message(STATUS "Looking for libjustify using LIBJUSTIFY_DIR = ${LIBJUSTIFY_DIR}")

    find_path(LIBJUSTIFY_INCLUDE_DIRS
        NAMES cprintf.h
        HINTS ${LIBJUSTIFY_DIR}/include/
    )
    if(NOT LIBJUSTIFY_INCLUDE_DIRS)
        MESSAGE(WARNING "Could not find cprintf.h in ${LIBJUSTIFY_DIR}/include")
    endif()

    find_library(LIBJUSTIFY_LIBRARY
        NAMES libcprintf.so
        HINTS ${LIBJUSTIFY_DIR}/lib/
    )
    if(NOT LIBJUSTIFY_LIBRARY)
        MESSAGE(WARNING "Could not find libcprintf.so in ${LIBJUSTIFY_DIR}/lib/")
    endif()

    set(LIBJUSTIFY_FOUND TRUE CACHE INTERNAL "")
    add_definitions(-DLIBJUSTIFY_FOUND)
    set(LIBJUSTIFY_DIR ${LIBJUSTIFY_DIR} CACHE PATH "" FORCE)
    include_directories(${LIBJUSTIFY_INCLUDE_DIRS})

    message(STATUS "FOUND libjustify")
    message(STATUS " [*] LIBJUSTIFY_DIR = ${LIBJUSTIFY_DIR}")
    message(STATUS " [*] LIBJUSTIFY_INCLUDE_DIRS = ${LIBJUSTIFY_INCLUDE_DIRS}")
    message(STATUS " [*] LIBJUSTIFY_LIBRARY = ${LIBJUSTIFY_LIBRARY}")

# If LIBJUSTIFY_DIR not specified, then try to automatically find the LIBJUSTIFY header
# and library
elseif(NOT LIBJUSTIFY_FOUND)
    message(STATUS "Looking for libjustify install on system")
    find_path(LIBJUSTIFY_INCLUDE_DIRS
        NAMES cprintf.h
    )

    find_library(LIBJUSTIFY_LIBRARY
        NAMES libcprintf.so
    )

    if(LIBJUSTIFY_INCLUDE_DIRS AND LIBJUSTIFY_LIBRARY)
        set(LIBJUSTIFY_FOUND TRUE CACHE INTERNAL "")
        set(LIBJUSTIFY_DIR ${LIBJUSTIFY_DIR} CACHE PATH "" FORCE)
        set(LIBJUSTIFY_INCLUDE_DIRS ${LIBJUSTIFY_INCLUDE_DIRS} CACHE PATH "" FORCE)
        set(LIBJUSTIFY_LIBRARY ${LIBJUSTIFY_LIBRARY} CACHE PATH "" FORCE)
        include_directories(${LIBJUSTIFY_INCLUDE_DIRS})

        message(STATUS "FOUND libjustify using find_library()")
        message(STATUS " [*] LIBJUSTIFY_FOUND = TRUE")
        message(STATUS " [*] LIBJUSTIFY_INCLUDE_DIRS = ${LIBJUSTIFY_INCLUDE_DIRS}")
        message(STATUS " [*] LIBJUSTIFY_LIBRARY = ${LIBJUSTIFY_LIBRARY}")
    endif()
endif()

# Abort if all methods fail
if(NOT LIBJUSTIFY_FOUND)
    message(FATAL_ERROR "Libjustify support needs explict LIBJUSTIFY_DIR")
endif()
