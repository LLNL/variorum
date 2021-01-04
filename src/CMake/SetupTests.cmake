# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(UNIT_TEST_BASE_LIBS gtest_main gtest)

function(add_unit_test)
    set(options)
    set(singleValueArgs TEST)
    set(multiValueArgs DEPENDS_ON)

    # parse our arguments
    cmake_parse_arguments(arg
                         "${options}"
                         "${singleValueArgs}"
                         "${multiValueArgs}" ${ARGN})

    message(STATUS " [*] Adding unit test: ${arg_TEST}")

    add_executable(${arg_TEST} ${arg_TEST}.cpp)
    target_link_libraries(${arg_TEST} ${UNIT_TEST_BASE_LIBS} variorum ${variorum_deps})

    add_test(NAME ${arg_TEST} COMMAND ${arg_TEST})
endfunction()
