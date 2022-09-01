# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

target_link_libraries(variorum ${variorum_deps})

set(FORTRAN_EXAMPLES
    variorum-print-power-fortran-example
    variorum-print-thermals-fortran-example
)

message(STATUS "Adding variorum Fortran examples")
foreach(EXAMPLE ${FORTRAN_EXAMPLES})
    message(STATUS " [*] Adding Fortran example: ${EXAMPLE}")
    enable_language(Fortran)
    add_executable(${EXAMPLE} ${EXAMPLE}.f90)
    target_link_libraries(${EXAMPLE} variorum ${variorum_deps})
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)
