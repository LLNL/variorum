# Copyright 2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

target_link_libraries(variorum ${variorum_deps})

set(OPENMP_EXAMPLES
    variorum-cap-socket-power-limit-openmp-example
    variorum-print-power-limit-openmp-example
    variorum-print-power-openmp-example
    variorum-print-verbose-power-limit-openmp-example
    variorum-print-verbose-power-openmp-example
)

message(STATUS "Adding variorum OpenMP examples")
foreach(EXAMPLE ${OPENMP_EXAMPLES})
    message(STATUS " [*] Adding OpenMP example: ${EXAMPLE}")
    add_executable(${EXAMPLE} ${EXAMPLE}.c)
    target_include_directories(${EXAMPLE} PUBLIC)
    target_link_libraries(${EXAMPLE} variorum ${variorum_deps} ${OpenMP_CXX_FLAGS})
    target_compile_options(${EXAMPLE} PRIVATE ${OpenMP_CXX_FLAGS})
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)
