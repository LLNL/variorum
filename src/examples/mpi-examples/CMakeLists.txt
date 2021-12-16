# Copyright 2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

target_link_libraries(variorum ${variorum_deps})

set(MPI_EXAMPLES
    variorum-cap-socket-power-limit-mpi-example
    variorum-print-power-limit-mpi-example
    variorum-print-power-mpi-example
    variorum-print-verbose-power-mpi-example
    variorum-print-verbose-power-limit-mpi-example
)

message(STATUS "Adding variorum MPI examples")
foreach(EXAMPLE ${MPI_EXAMPLES})
    message(STATUS " [*] Adding MPI example: ${EXAMPLE}")
    add_executable(${EXAMPLE} ${EXAMPLE}.c)
    target_include_directories(${EXAMPLE} PUBLIC ${MPI_CXX_INCLUDE_PATH} ${RANKSTR_INCLUDE_DIRS})
    target_link_libraries(${EXAMPLE} variorum ${variorum_deps} ${MPI_CXX_LIBRARIES} ${MPI_CXX_LINK_FLAGS} ${RANKSTR_LIBRARY})
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)
