target_link_libraries(variorum ${variorum_deps})

add_definitions(-DSECOND_RUN)

set(BASIC_EXAMPLES
  ex-dump-power-limits
  ex-print-power-limits
  ex-dump-thermals
  ex-print-thermals
  ex-print-topology
  ex-print-features
  ex-set-power-limits
  ex-dump-counters
  ex-print-counters
  ex-dump-clock-speed
  ex-print-clock-speed
  ex-dump-power
  ex-print-power
  ex-dump-hyperthreading
  ex-dump-turbo
  ex-poll-power-to-file
  ex-poll-power-to-stdout
  ex-monitoring-to-file
)

message(STATUS "Adding variorum examples")
foreach(EXAMPLE ${BASIC_EXAMPLES})
    message(STATUS " [*] Adding example: ${EXAMPLE}")
    add_executable(${EXAMPLE} ${EXAMPLE}.c)
    target_link_libraries(${EXAMPLE} variorum ${variorum_deps})
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)
