target_link_libraries(variorum ${variorum_deps})

add_definitions(-DSECOND_RUN)

set(BASIC_EXAMPLES
    ex-disable-turbo
    ex-enable-turbo
    ex-monitoring-to-file
    ex-poll-power-to-file
    ex-poll-power-to-stdout
    ex-print-clock-speed
    ex-print-counters
    ex-print-features
    ex-print-hyperthreading
    ex-print-power-limits
    ex-print-power
    ex-print-thermals
    ex-print-topology
    ex-print-turbo
    ex-print-verbose-clock-speed
    ex-print-verbose-counters
    ex-print-verbose-power-limits
    ex-print-verbose-power
    ex-print-verbose-thermals
    ex-set-cpu-clock-speed
    ex-set-power-limits
)

message(STATUS "Adding variorum examples")
foreach(EXAMPLE ${BASIC_EXAMPLES})
    message(STATUS " [*] Adding example: ${EXAMPLE}")
    add_executable(${EXAMPLE} ${EXAMPLE}.c)
    target_link_libraries(${EXAMPLE} variorum ${variorum_deps})
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)
