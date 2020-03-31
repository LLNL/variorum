target_link_libraries(variorum ${variorum_deps})

add_definitions(-DSECOND_RUN)

set(BASIC_EXAMPLES
    variorum-disable-turbo-example
    variorum-enable-turbo-example
    variorum-monitoring-to-file-example
    variorum-poll-power-to-file-example
    variorum-poll-power-to-stdout-example
    variorum-print-clock-speed-example
    variorum-print-counters-example
    variorum-print-features-example
    variorum-print-hyperthreading-example
    variorum-print-power-limits-example
    variorum-print-power-example
    variorum-print-thermals-example
    variorum-print-topology-example
    variorum-print-turbo-example
    variorum-print-gpu-utilization-example
    variorum-print-verbose-clock-speed-example
    variorum-print-verbose-counters-example
    variorum-print-verbose-power-limits-example
    variorum-print-verbose-power-example
    variorum-print-verbose-thermals-example
    variorum-print-verbose-gpu-utilization-example
    variorum-set-and-verify-node-power-limit-example
    variorum-set-cpu-clock-speed-example
    variorum-set-gpu-power-ratio-example
    variorum-set-node-power-limit-example
    variorum-set-socket-power-limits-example
)

message(STATUS "Adding variorum examples")
foreach(EXAMPLE ${BASIC_EXAMPLES})
    message(STATUS " [*] Adding example: ${EXAMPLE}")
    add_executable(${EXAMPLE} ${EXAMPLE}.c)
    target_link_libraries(${EXAMPLE} variorum ${variorum_deps})
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)

install(DIRECTORY using-with-make using-with-cmake
        DESTINATION examples)
