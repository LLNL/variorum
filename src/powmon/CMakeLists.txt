target_link_libraries(variorum ${variorum_deps})

set(CMAKE_C_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS "-pthread")

message(STATUS "Adding variorum demoapps")

set(powmon_sources
  highlander.c
  powmon.c
)
message(STATUS " [*] Adding demoapp: powmon")
add_executable(powmon ${powmon_sources})
target_link_libraries(powmon variorum ${variorum_deps})

set(power_wrapper_static_sources
  highlander.c
  power_wrapper_static.c
)
message(STATUS " [*] Adding demoapp: power_wrapper_static")
add_executable(power_wrapper_static ${power_wrapper_static_sources})
target_link_libraries(power_wrapper_static variorum ${variorum_deps})

set(power_wrapper_dynamic_sources
  highlander.c
  power_wrapper_dynamic.c
)
message(STATUS " [*] Adding demoapp: power_wrapper_dynamic")
add_executable(power_wrapper_dynamic ${power_wrapper_dynamic_sources})
target_link_libraries(power_wrapper_dynamic variorum ${variorum_deps})

include_directories(${CMAKE_SOURCE_DIR}/variorum
                    ${CMAKE_SOURCE_DIR}/variorum/Intel)

install(TARGETS powmon power_wrapper_static power_wrapper_dynamic
        DESTINATION bin)
