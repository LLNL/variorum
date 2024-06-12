# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# First check for user-specified NVML_DIR
if(NVML_DIR)
    message(STATUS "Looking for NVML using NVML_DIR = ${NVML_DIR}")

    set(NVML_FOUND TRUE CACHE INTERNAL "")
    set(VARIORUM_NVML_DIR ${NVML_DIR} CACHE INTERNAL "")
    set(NVML_DIR ${NVML_DIR} CACHE PATH "" FORCE)
    set(NVML_INCLUDE_DIRS ${NVML_DIR}/include CACHE PATH "" FORCE)
    set(NVML_LIBRARY ${NVML_DIR}/lib/libnvml.so CACHE PATH "" FORCE)
    include_directories(${NVML_INCLUDE_DIRS})

    message(STATUS "FOUND NVML")
    message(STATUS " [*] NVML_DIR = ${NVML_DIR}")
    message(STATUS " [*] NVML_INCLUDE_DIRS = ${NVML_INCLUDE_DIRS}")
    message(STATUS " [*] NVML_LIBRARY = ${NVML_LIBRARY}")
# If NVML_DIR not specified, then try to automatically find the NVML header
# and library
elseif(NOT NVML_FOUND)
    find_path(NVML_INCLUDE_DIRS
        NAMES nvml.h
	REQUIRED
	HINTS ${NVML_ROOT}/include
    )

    find_library(NVML_LIBRARY
        NAMES nvml nvidia-ml
	REQUIRED
	HINTS ${NVML_ROOT}/lib/stubs
    )

    if(NVML_INCLUDE_DIRS AND NVML_LIBRARY)
        set(NVML_FOUND TRUE CACHE INTERNAL "")
        set(VARIORUM_NVML_DIR ${NVML_DIR} CACHE INTERNAL "")
        set(NVML_DIR ${NVML_DIR} CACHE PATH "" FORCE)
        set(NVML_INCLUDE_DIRS ${NVML_INCLUDE_DIRS} CACHE PATH "" FORCE)
        set(NVML_LIBRARY ${NVML_LIBRARY} CACHE PATH "" FORCE)
        include_directories(${NVML_INCLUDE_DIRS})

        message(STATUS "FOUND NVML using find_library()")
        message(STATUS " [*] NVML_INCLUDE_DIRS = ${NVML_INCLUDE_DIRS}")
        message(STATUS " [*] NVML_LIBRARY = ${NVML_LIBRARY}")
    endif()
endif()
