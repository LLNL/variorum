# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(BASIC_TESTS
    t_variorum_gpu_power_ratio
    t_variorum_monitoring
    t_variorum_best_effort_node_power_limit
    t_variorum_poll_data
    t_variorum_query_clock_speed
    t_variorum_query_counters
    t_variorum_query_gpu_utilization
    t_variorum_query_hyperthreading
    t_variorum_query_power
    t_variorum_query_power_limits
    t_variorum_query_thermals
    t_variorum_query_turbo
    t_variorum_socket_power_limit
    t_variorum_toggle_turbo
    t_variorum_set_socket_frequency
)

set(UNIT_TEST_BASE_LIBS gtest_main gtest)

message(STATUS "Adding variorum unit tests")
foreach(TEST ${BASIC_TESTS})
    add_unit_test(TEST ${TEST} DEPENDS_ON variorum)
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)
