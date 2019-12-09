cmake_minimum_required(VERSION 2.8)

project(variorum C CXX)

option(BUILD_SHARED_LIBS    "Build shared libraries"       ON)
option(BUILD_DOCS           "Build documentation"          ON)
option(BUILD_TESTS          "Build tests"                  ON)
option(VARIORUM_WITH_INTEL  "Support Intel architectures"  ON)
option(VARIORUM_WITH_AMD    "Support AMD architectures"    ON)
option(VARIORUM_WITH_IBM    "Support IBM architectures"    ON)
option(VARIORUM_WITH_NVIDIA "Support Nvidia architectures" ON)
option(VARIORUM_DEBUG       "Enable debug statements"      OFF)
option(VARIORUM_LOG         "Enable logging statements"    ON)

include(CMake/CMakeBasics.cmake)

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

### Add our tests
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

### Add our examples
add_subdirectory(examples)

### Add powmon sampler
add_subdirectory(powmon)

### Create CMake importable exports
install(EXPORT variorum DESTINATION cmake)
