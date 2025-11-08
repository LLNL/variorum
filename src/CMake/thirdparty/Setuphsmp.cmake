# First check for user-specified HSMP_DIR
if(HSMP_DIR)
    MESSAGE(STATUS "Looking for HSMP using HSMP_DIR = ${HSMP_DIR}")

    set(HSMP_FOUND TRUE)
    set(HSMP_INCLUDE_DIRS ${HSMP_DIR})

    set(HSMP_DIR ${HSMP_DIR} CACHE PATH "" FORCE)

    message(STATUS "FOUND HSMP at ${HSMP_DIR}")
    message(STATUS " [*] HSMP_INCLUDE_DIRS = ${HSMP_INCLUDE_DIRS}")
# If HSMP_DIR not specified, then try to automatically find the HWLOC header
# and library
elseif(NOT HSMP_DIR)
    find_path(HSMP_INCLUDE_DIRS
        NAMES amd_hsmp.h
    )

    if(HSMP_INCLUDE_DIRS)
        set(HSMP_FOUND TRUE)
        message(STATUS "FOUND HSMP using find_library()")
        message(STATUS " [*] HSMP_INCLUDE_DIRS = ${HSMP_INCLUDE_DIRS}")
        message(STATUS " [*] HSMP_LIBRARY = ${HSMP_LIBRARY}")
    endif()
endif()

# Abort if all methods fail
if(NOT HSMP_FOUND)
    MESSAGE(FATAL_ERROR "HSMP support needed")
endif()
