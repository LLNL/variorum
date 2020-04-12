# First check for user-specified HWLOC_DIR
if(HWLOC_DIR)
    message(STATUS "Looking for Hwloc using HWLOC_DIR = ${HWLOC_DIR}")

    set(HWLOC_FOUND TRUE CACHE INTERNAL "")
    set(VARIORUM_HWLOC_DIR ${HWLOC_DIR} CACHE INTERNAL "")
    set(HWLOC_DIR ${HWLOC_DIR} CACHE PATH "" FORCE)
    set(HWLOC_INCLUDE_DIRS ${HWLOC_DIR}/include CACHE PATH "" FORCE)
    set(HWLOC_LIBRARY ${HWLOC_DIR}/lib/libhwloc.so CACHE PATH "" FORCE)
    include_directories(${HWLOC_INCLUDE_DIRS})

    message(STATUS "FOUND hwloc")
    message(STATUS " [*] HWLOC_DIR = ${HWLOC_DIR}")
    message(STATUS " [*] HWLOC_INCLUDE_DIRS = ${HWLOC_INCLUDE_DIRS}")
    message(STATUS " [*] HWLOC_LIBRARY = ${HWLOC_LIBRARY}")
# If HWLOC_DIR not specified, then try to automatically find the HWLOC header
# and library
elseif(NOT HWLOC_FOUND)
    find_path(HWLOC_INCLUDE_DIRS
        NAMES hwloc.h
    )

    find_library(HWLOC_LIBRARY
        NAMES libhwloc.so
    )

    if(HWLOC_INCLUDE_DIRS AND HWLOC_LIBRARY)
        set(HWLOC_FOUND TRUE CACHE INTERNAL "")
        set(VARIORUM_HWLOC_DIR ${HWLOC_DIR} CACHE INTERNAL "")
        set(HWLOC_DIR ${HWLOC_DIR} CACHE PATH "" FORCE)
        set(HWLOC_INCLUDE_DIRS ${HWLOC_INCLUDE_DIRS} CACHE PATH "" FORCE)
        set(HWLOC_LIBRARY ${HWLOC_LIBRARY} CACHE PATH "" FORCE)
        include_directories(${HWLOC_INCLUDE_DIRS})

        message(STATUS "FOUND hwloc using find_library()")
        message(STATUS " [*] HWLOC_INCLUDE_DIRS = ${HWLOC_INCLUDE_DIRS}")
        message(STATUS " [*] HWLOC_LIBRARY = ${HWLOC_LIBRARY}")
    endif()
endif()

# Abort if all methods fail
if(NOT HWLOC_FOUND)
    message(FATAL_ERROR "Hwloc support needs explict HWLOC_DIR")
endif()
