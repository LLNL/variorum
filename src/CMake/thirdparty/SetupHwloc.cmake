# First check for user-specified HWLOC_DIR
if(HWLOC_DIR)
    message(STATUS "Looking for Hwloc using HWLOC_DIR = ${HWLOC_DIR}")

    set(HWLOC_FOUND TRUE)
    set(HWLOC_INCLUDE_DIRS ${HWLOC_DIR}/include)
    set(HWLOC_LIBRARY ${HWLOC_DIR}/lib/libhwloc.so)

    set(VARIORUM_HWLOC_DIR ${HWLOC_DIR} CACHE STRING "" FORCE)

    message(STATUS "FOUND hwloc at ${HWLOC_DIR}")
    message(STATUS " [*] HWLOC_INCLUDE_DIRS = ${HWLOC_INCLUDE_DIRS}")
    message(STATUS " [*] HWLOC_LIBRARY = ${HWLOC_LIBRARY}")
# If HWLOC_DIR not specified, then try to automatically find the HWLOC header
# and library
elseif(NOT HWLOC_DIR)
    find_path(HWLOC_INCLUDE_DIRS
        NAMES hwloc.h
    )

    find_library(HWLOC_LIBRARY
        NAMES libhwloc.so
    )

    if(HWLOC_INCLUDE_DIRS AND HWLOC_LIBRARY)
        set(HWLOC_FOUND TRUE)
        set(VARIORUM_HWLOC_DIR ${HWLOC_DIR} CACHE STRING "" FORCE)
        message(STATUS "FOUND hwloc using find_library() in ${HWLOC_DIR}")
        message(STATUS " [*] HWLOC_INCLUDE_DIRS = ${HWLOC_INCLUDE_DIRS}")
        message(STATUS " [*] HWLOC_LIBRARY = ${HWLOC_LIBRARY}")
    endif()
endif()

# If HWLOC is still not found, then download and build HWLOC from source
if(NOT HWLOC_INCLUDE_DIRS AND HWLOC_LIBRARY)
    message(STATUS "Downloading and building hwloc from source")

    ### Necessary for ExternalProject_Add
    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
    include(../SetupExternalProjects.cmake)
    message(STATUS "Adding external project: hwloc in ${HWLOC_DIR}")
    message(STATUS " [*] HWLOC_INCLUDE_DIRS = ${HWLOC_INCLUDE_DIRS}")
    message(STATUS " [*] HWLOC_LIBRARY = ${HWLOC_LIBRARY}")
    set(HWLOC_FOUND TRUE)
    set(VARIORUM_HWLOC_DIR ${HWLOC_DIR} CACHE STRING "" FORCE)
endif()

# Abort if all methods fail
if(NOT HWLOC_FOUND)
    message(FATAL_ERROR "Hwloc support needed")
endif()
