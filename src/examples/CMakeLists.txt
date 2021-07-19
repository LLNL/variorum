# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(BASIC_EXAMPLES
#    variorum-cap-and-verify-node-power-limit-example
#    variorum-cap-best-effort-node-power-limit-example
#    variorum-cap-each-core-frequency-limit-example
#    variorum-cap-gpu-power-ratio-example
#    variorum-cap-socket-clock-speed-example
#    variorum-cap-socket-power-limits-example
#    variorum-disable-turbo-example
#    variorum-enable-turbo-example
#    variorum-get-node-power-json-example
#    variorum-monitoring-to-file-example
#    variorum-poll-power-to-file-example
#    variorum-poll-power-to-stdout-example
#    variorum-print-available-frequencies-example
#    variorum-print-clock-speed-example
#    variorum-print-counters-example
#    variorum-print-features-example
#    variorum-print-gpu-utilization-example
#    variorum-print-hyperthreading-example
#    variorum-print-power-example
    variorum-print-power-limits-example
#    variorum-print-thermals-example
#    variorum-print-topology-example
#    variorum-print-turbo-example
#    variorum-print-verbose-clock-speed-example
#    variorum-print-verbose-counters-example
#    variorum-print-verbose-gpu-utilization-example
#    variorum-print-verbose-power-limits-example
#    variorum-print-verbose-power-example
#    variorum-print-verbose-thermals-example
)

message(STATUS "Adding variorum examples")
foreach(EXAMPLE ${BASIC_EXAMPLES})
    message(STATUS " [*] Adding example: ${EXAMPLE}")

    if(ENABLE_SERIAL)
        set(variorum_deps variorum)
        message(STATUS "RRR-serial ${variorum_deps}")
        add_executable(${EXAMPLE}_ser ${EXAMPLE}.c)
        target_compile_definitions(${EXAMPLE}_ser PUBLIC -DSECOND_RUN)
        target_link_libraries(${EXAMPLE}_ser variorum ${variorum_deps})
    endif()

    if(MPI_FOUND)
        set(variorum_par_deps variorum_mpi mpi)
        message(STATUS "RRR-par ${variorum_par_deps}")
        add_executable(${EXAMPLE}_par ${EXAMPLE}.c)
        target_compile_definitions(${EXAMPLE}_par PUBLIC -DSECOND_RUN -DPARALLEL)
        target_link_libraries(${EXAMPLE}_par variorum_mpi ${variorum_par_deps})
    endif()
endforeach()

if(ENABLE_SERIAL)
    include_directories(${CMAKE_SOURCE_DIR}/variorum)
endif()
if(MPI_FOUND)
    include_directories(${CMAKE_SOURCE_DIR}/variorum ${MPI_INCLUDE_PATH})
endif()

install(DIRECTORY using-with-make using-with-cmake
        DESTINATION examples)
