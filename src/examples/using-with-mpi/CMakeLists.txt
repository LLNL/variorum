# Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
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
#   ./ex-print-power
#
# If run in-sub directory below using-with-cmake in an variorum install,
# VARIORUM_DIR will be defaulted to ../../..
#
#   mkdir build
#   cd build
#   cmake ..
#   make
#   ./ex-print-power
###############################################################################

cmake_minimum_required(VERSION 3.0)

project(using_with_cmake)

find_package(MPI REQUIRED)

#
# Provide default for VARIORUM_DIR that works for an variorum install
#
if(NOT VARIORUM_DIR)
    set(VARIORUM_DIR "../..")
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
