# Copyright 2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

###############################################################################
# Example that shows how to use an installed instance of Variorum in another
# CMake-based build system.
#
# To build:
#   mkdir build
#   cd build
#   cmake -DVARIORUM_DIR={variorum install path} ../
#   make
#   ./variorum-print-power-mpi-example
#
# If run in subdirectory of using-with-mpi contained in a variorum install,
# VARIORUM_DIR will be defaulted to ../../..
#
#   mkdir build
#   cd build
#   cmake ..
#   make
#   ./variorum-print-power-mpi-example
###############################################################################

cmake_minimum_required(VERSION 3.0)

project(using_with_mpi)

find_package(MPI REQUIRED)

#
# Provide default for VARIORUM_DIR that works for an variorum install
#
if(NOT VARIORUM_DIR)
    set(VARIORUM_DIR "../../..")
endif()

#
# Check for valid Variorum install
#
get_filename_component(VARIORUM_DIR ${VARIORUM_DIR} ABSOLUTE)
if(NOT EXISTS ${VARIORUM_DIR}/lib/cmake/VariorumConfig.cmake)
    message(FATAL_ERROR "Could not find Variorum CMake include file (${VARIORUM_DIR}/lib/cmake/VariorumConfig.cmake)")
endif()

#
# Use CMake's find_package to import variorum's targets
#
find_package(Variorum REQUIRED
             NO_DEFAULT_PATH
             PATHS ${VARIORUM_DIR}/lib/cmake)

add_definitions(-DSECOND_RUN)

# create our example
add_executable(variorum-print-power-mpi-example variorum-print-power-mpi-example.c)

target_include_directories(variorum-print-power-mpi-example PUBLIC ${MPI_CXX_INCLUDE_PATH})

# link to variorum
target_link_libraries(variorum-print-power-mpi-example ${MPI_CXX_LIBRARIES} ${MPI_CXX_LINK_FLAGS} variorum::variorum)
