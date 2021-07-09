# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(VARIORUM_INCLUDE_DIRS "${VARIORUM_INSTALL_PREFIX}/include")

if(VARIORUM_SERIAL_ENABLED)
    # create convenience target that bundles all reg variorum deps
    add_library(variorum::variorum INTERFACE IMPORTED)

    set_property(TARGET variorum::variorum
                 APPEND PROPERTY
                 INTERFACE_INCLUDE_DIRECTORIES "${VARIORUM_INSTALL_PREFIX}/include/")

    set_property(TARGET variorum::variorum
                 PROPERTY INTERFACE_LINK_LIBRARIES
                 variorum)
endif()

if(VARIORUM_MPI_ENABLED)
    # create convenience target that bundles all reg variorum deps
    add_library(variorum::variorum_mpi INTERFACE IMPORTED)

    set_property(TARGET variorum::variorum_mpi
                 APPEND PROPERTY
                 INTERFACE_INCLUDE_DIRECTORIES "${VARIORUM_INSTALL_PREFIX}/include/")

    set_property(TARGET variorum::variorum_mpi
                 PROPERTY INTERFACE_LINK_LIBRARIES
                 variorum)_mpi
endif()

if(TARGET hwloc::hwloc)
    set_property(TARGET variorum::variorum
                 APPEND PROPERTY
                 INTERFACE_LINK_LIBRARIES hwloc::hwloc)
else()
    # if not, bottle hwloc
    set_property(TARGET variorum::variorum
                 APPEND PROPERTY
                 INTERFACE_INCLUDE_DIRECTORIES ${VARIORUM_HWLOC_INCLUDE_DIR})
endif()

if(TARGET jansson::jansson)
    set_property(TARGET variorum::variorum
                 APPEND PROPERTY
                 INTERFACE_LINK_LIBRARIES jansson::jansson)
else()
    # if not, bottle jansson
    set_property(TARGET variorum::variorum
                 APPEND PROPERTY
                 INTERFACE_INCLUDE_DIRECTORIES ${VARIORUM_JANSSON_INCLUDE_DIR})
endif()

if(NOT Variorum_FIND_QUIETLY)
    message(STATUS "VARIORUM_VERSION        = ${VARIORUM_VERSION}")
    message(STATUS "VARIORUM_INSTALL_PREFIX = ${VARIORUM_INSTALL_PREFIX}")
    message(STATUS "VARIORUM_INCLUDE_DIRS   = ${VARIORUM_INCLUDE_DIRS}")
    message(STATUS "VARIORUM_SERIAL_ENABLED = ${VARIORUM_SERIAL_ENABLED}")
    message(STATUS "VARIORUM_MPI_ENABLED    = ${VARIORUM_MPI_ENABLED}")

    set(_print_targets "")
    if(VARIORUM_SERIAL_ENABLED)
        set(_print_targets "variorum::variorum ")
    endif()

    if(VARIORUM_MPI_ENABLED)
        set(_print_targets "${_print_targets}variorum::variorum_mpi ")
    endif()

    message(STATUS "Variorum imported targets: ${_print_targets}")
    unset(_print_targets)
endif()
