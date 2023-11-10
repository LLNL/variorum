# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

target_link_libraries(variorum ${variorum_deps})

set(FORTRAN_EXAMPLES
    variorum-print-power-fortran-example
    variorum-print-thermals-fortran-example
)

enable_language(Fortran)

if(CMAKE_Fortran_COMPILER_VERSION VERSION_GREATER 10.2)
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fallow-invalid-boz")
endif()

message(STATUS "Adding variorum Fortran examples")
foreach(EXAMPLE ${FORTRAN_EXAMPLES})
    message(STATUS " [*] Adding Fortran example: ${EXAMPLE}")
    add_executable(${EXAMPLE} ${EXAMPLE}.f90)
    target_link_libraries(${EXAMPLE} variorum ${variorum_deps})
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)
