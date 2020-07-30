# Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# avoid warnings for project commands w/o VERSION
cmake_policy(SET CMP0048 NEW)

project(variorum VERSION "0.3.0" LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

option(BUILD_SHARED_LIBS    "Build shared libraries"       ON)
option(BUILD_DOCS           "Build documentation"          ON)
option(BUILD_TESTS          "Build tests"                  ON)

option(ENABLE_FORTRAN       "Build Fortran support"        ON)

option(VARIORUM_WITH_INTEL  "Support Intel architectures"  ON)
option(VARIORUM_WITH_AMD    "Support AMD architectures"    OFF)
option(VARIORUM_WITH_IBM    "Support IBM architectures"    OFF)
option(VARIORUM_WITH_NVIDIA "Support Nvidia architectures" OFF)
option(VARIORUM_DEBUG       "Enable debug statements"      OFF)
option(VARIORUM_LOG         "Enable logging statements"    ON)

set(HWLOC_DIR "" CACHE PATH "path to hwloc installation")
set(JANSSON_DIR "" CACHE PATH "path to jansson installation")

if(ENABLE_FORTRAN)
    enable_language(Fortran)
endif()

include(CMake/CMakeBasics.cmake)

include(CMake/SetupFortran.cmake)

include(CMake/VariorumBuildType.cmake)

include(CMake/SetupCompilerOptions.cmake)

include(CMake/Setup3rdParty.cmake)

include(CMake/SetupTests.cmake)

add_subdirectory(thirdparty_builtin)

### Add documentation targets
if(BUILD_DOCS)
    include(CMake/SetupDocs.cmake)
    add_subdirectory(docs)
endif()

### Add our libs
add_subdirectory(variorum)

### Invoke CMake Fortran setup
### if ENABLE_FORTRAN == ON
if(ENABLE_FORTRAN)
    enable_language(Fortran)
endif()

### Add our tests
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

### Add our examples
add_subdirectory(examples)

### Add powmon sampler
add_subdirectory(powmon)

### Add config helpers
add_subdirectory(config)

### Create CMake importable exports
install(EXPORT variorum DESTINATION lib/cmake)
