# Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(BASIC_TESTS
  t_variorum_test
  t_msr_driver
  t_msr_safe_driver
  t_msr_whitelist
  )

message(STATUS "Adding variorum unit tests")
foreach(TEST ${BASIC_TESTS})
    message(STATUS " [*] Adding unit test: ${TEST}")
    add_executable(${TEST} ${TEST}.cpp)
    ### Command target_compile_options() is available since CMake v2.8.12
    if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} EQUAL 2.8.12 OR
        ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} GREATER 2.8.12)
        target_compile_options(${TEST} PRIVATE "-std=c++11")
    ### For older CMake versions, use command set_target_properties()
    else (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} EQUAL 2.8.12 OR
          ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} GREATER 2.8.12)
        set_target_properties(${TEST} PROPERTIES
                              CXX_STANDARD 11
                              CXX_STANDARD_REQUIRED YES
                              CXX_EXTENSIONS NO)
    endif(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} EQUAL 2.8.12 OR
          ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} GREATER 2.8.12)
    target_link_libraries(${TEST} ${UNIT_TEST_BASE_LIBS})
    target_link_libraries(${TEST} variorum ${variorum_deps})
    add_test(${TEST} ${TEST})
endforeach()
