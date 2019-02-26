################################
# Standard CMake Options
################################


## Fail if someone tries to config an in-source build.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   message(FATAL_ERROR "In-source builds are not supported. Please remove "
                       "CMakeCache.txt from the 'src' dir and configure an "
                       "out-of-source build in another directory.")
endif()

### Always use position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMake build tool name: ${CMAKE_BUILD_TOOL}")

macro(ENABLE_WARNINGS)
    add_definitions(-Wall -Wextra)
endmacro()

if(VARIORUM_WITH_INTEL)
    message(STATUS "Building support for Intel architectures (VARIORUM_WITH_INTEL == ON)")
endif()
if(VARIORUM_WITH_IBM)
    message(STATUS "Building support for IBM architectures (VARIORUM_WITH_IBM == ON)")
endif()

if(BUILD_DOCS)
    message(STATUS "Building documentation (BUILD_DOCS == ON)")
endif()

################################
# Shared vs Static Libs
################################
if(BUILD_SHARED_LIBS)
    message(STATUS "Building variorum as a shared library (BUILD_SHARED_LIBS == ON)")
else()
    message(STATUS "Building variorum as a static library (BUILD_SHARED_LIBS == OFF)")
endif()

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
