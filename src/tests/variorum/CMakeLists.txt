# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(BASIC_TESTS
    t_variorum_cap_best_effort_node_power_limit
    t_variorum_cap_gpu_power_ratio
    t_variorum_cap_socket_frequency_limit
    t_variorum_cap_socket_power_limit
    t_variorum_monitoring
    t_variorum_poll_data
    t_variorum_query_frequency
    t_variorum_query_counters
    t_variorum_query_gpu_utilization
    t_variorum_query_hyperthreading
    t_variorum_query_power
    t_variorum_query_power_limit
    t_variorum_query_thermals
    t_variorum_query_turbo
    t_variorum_toggle_turbo
)

set(UNIT_TEST_BASE_LIBS gtest_main gtest)

message(STATUS "Adding variorum unit tests")
foreach(TEST ${BASIC_TESTS})
    add_unit_test(TEST ${TEST} DEPENDS_ON variorum)
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)

# quick hack
if(VARIORUM_WITH_INTEL_GPU)
	set(CMAKE_EXE_LINKER_FLAGS "-lze_loader -lstdc++ -L${APMIDG_DIR}/lib64/ -lapmidg")
endif()
