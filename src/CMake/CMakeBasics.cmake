###########################
# Standard CMake Options #
###########################


## Fail if someone tries to config an in-source build.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   message(FATAL_ERROR "In-source builds are not supported. Please remove "
                       "CMakeCache.txt from the 'src' dir and configure an "
                       "out-of-source build in another directory.")
endif()

### Always use position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMake build tool name: ${CMAKE_BUILD_TOOL}")

### Enable warnings
macro(ENABLE_WARNINGS)
    add_definitions(-Wall -Wextra)
endmacro()

##########################
# Platform Architectures #
##########################
if(VARIORUM_WITH_INTEL)
    message(STATUS "Building support for Intel architectures (VARIORUM_WITH_INTEL == ON)")
else()
    message(STATUS "Building without support for Intel architectures (VARIORUM_WITH_INTEL == OFF)")
endif()
if(VARIORUM_WITH_IBM)
    message(STATUS "Building support for IBM architectures (VARIORUM_WITH_IBM == ON)")
else()
    message(STATUS "Building without support for IBM architectures (VARIORUM_WITH_IBM == OFF)")
endif()
if(VARIORUM_WITH_AMD)
    message(STATUS "Building support for AMD architectures (VARIORUM_WITH_AMD == ON)")
else()
    message(STATUS "Building without support for AMD architectures (VARIORUM_WITH_AMD == OFF)")
endif()
if(VARIORUM_WITH_NVIDIA)
    message(STATUS "Building support for Nvidia architectures (VARIORUM_WITH_NVIDIA == ON)")
else()
    message(STATUS "Building without support for Nvidia architectures (VARIORUM_WITH_NVIDIA == OFF)")
endif()

#############
# DEBUGGING #
#############
if (VARIORUM_DEBUG)
    message(STATUS "Building with debug statements (VARIORUM_DEBUG = ON)")
else()
    message(STATUS "Building without debug statements (VARIORUM_DEBUG = OFF)")
endif()
if (VARIORUM_LOG)
    message(STATUS "Building with logging statements (VARIORUM_LOG = ON)")
else()
    message(STATUS "Building without logging statements (VARIORUM_LOG = OFF)")
endif()

#################
# Documentation #
#################
if(BUILD_DOCS)
    message(STATUS "Building documentation (BUILD_DOCS == ON)")
else()
    message(STATUS "Building without documentation (BUILD_DOCS == OFF)")
endif()

#########################
# Shared vs Static Libs #
#########################
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
