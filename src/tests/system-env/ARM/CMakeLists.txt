# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(BASIC_TESTS
  t_system_env_ARM_hwmon
)

set(UNIT_TEST_BASE_LIBS gtest_main gtest)

message(STATUS "Adding system environment (ARM) unit tests")
foreach(TEST ${BASIC_TESTS})
    add_unit_test(TEST ${TEST} DEPENDS_ON variorum)
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/variorum)
