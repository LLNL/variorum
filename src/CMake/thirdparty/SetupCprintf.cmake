# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# First check for user-specified CPRINTF_DIR
if(CPRINTF_DIR)
    message(STATUS "Looking for Cprintf using CPRINTF_DIR = ${CPRINTF_DIR}")


    find_path(CPRINTF_INCLUDE_DIRS
        NAMES cprintf.h
        HINTS ${CPRINTF_DIR}/src/
    )
    if(NOT CPRINTF_INCLUDE_DIRS)
        MESSAGE(WARNING "Could not find cprintf.h in ${CPRINTF_DIR}/include")
    endif()

    find_library(CPRINTF_LIBRARY
        NAMES libcprintf.so
        HINTS ${CPRINTF_DIR}/build/libjustify
    )
    if(NOT CPRINTF_LIBRARY)
        MESSAGE(WARNING "Could not find libcprintf.so in ${CPRINTF_DIR}/build/libjustify")
    endif()

    set(CPRINTF_FOUND TRUE CACHE INTERNAL "")
    set(CPRINTF_DIR ${CPRINTF_DIR} CACHE PATH "" FORCE)
    include_directories(${CPRINTF_INCLUDE_DIRS})

    message(STATUS "FOUND Cprintf")
    message(STATUS " [*] CPRINTF_DIR = ${CPRINTF_DIR}")
    message(STATUS " [*] CPRINTF_INCLUDE_DIRS = ${CPRINTF_INCLUDE_DIRS}")
    message(STATUS " [*] CPRINTF_LIBRARY = ${CPRINTF_LIBRARY}")
# If CPRINTF_DIR not specified, then try to automatically find the CPRINTF header
# and library
elseif(NOT CPRINTF_FOUND)
    message(STATUS "Looking for Cprintf install on system")
    find_path(CPRINTF_INCLUDE_DIRS
        NAMES cprintf.h
    )

    find_library(CPRINTF_LIBRARY
        NAMES cprintf.so
    )

    if(CPRINTF_INCLUDE_DIRS AND CPRINTF_LIBRARY)
        set(CPRINTF_FOUND TRUE CACHE INTERNAL "")
        set(CPRINTF_DIR ${CPRINTF_DIR} CACHE PATH "" FORCE)
        set(CPRINTF_INCLUDE_DIRS ${CPRINTF_INCLUDE_DIRS} CACHE PATH "" FORCE)
        set(CPRINTF_LIBRARY ${CPRINTF_LIBRARY} CACHE PATH "" FORCE)
        include_directories(${CPRINTF_INCLUDE_DIRS})

        message(STATUS "FOUND cprintf using find_library()")
        message(STATUS " [*] CPRINTF_INCLUDE_DIRS = ${CPRINTF_INCLUDE_DIRS}")
        message(STATUS " [*] CPRINTF_LIBRARY = ${CPRINTF_LIBRARY}")
    endif()
endif()

# Abort if all methods fail
if(NOT CPRINTF_FOUND)
    message(FATAL_ERROR "Cprintf support needs explict CPRINTF_DIR")
endif()