# First check for user-specified ESMI_DIR
if(ESMI_DIR)
    MESSAGE(STATUS "Looking for ESMI using ESMI_DIR = ${ESMI_DIR}")

    set(ESMI_FOUND TRUE)
    set(ESMI_INCLUDE_DIRS ${ESMI_DIR}/include)
    set(ESMI_LIBRARY ${ESMI_DIR}/lib/libe_smi64.so)

    set(ESMI_DIR ${ESMI_DIR} CACHE PATH "" FORCE)

    message(STATUS "FOUND ESMI at ${ESMI_DIR}")
    message(STATUS " [*] ESMI_INCLUDE_DIRS = ${ESMI_INCLUDE_DIRS}")
    message(STATUS " [*] ESMI_LIBRARY = ${ESMI_LIBRARY}")
# If ESMI_DIR not specified, then try to automatically find the HWLOC header
# and library
elseif(NOT ESMI_DIR)
    find_path(ESMI_INCLUDE_DIRS
        NAMES e_smi.h
    )

    find_library(ESMI_LIBRARY
        NAMES libe_smi64.so
    )

    if(ESMI_INCLUDE_DIRS AND ESMI_LIBRARY)
        set(ESMI_FOUND TRUE)
        message(STATUS "FOUND esmi using find_library()")
        message(STATUS " [*] ESMI_INCLUDE_DIRS = ${ESMI_INCLUDE_DIRS}")
        message(STATUS " [*] ESMI_LIBRARY = ${ESMI_LIBRARY}")
    endif()
endif()

# If HWLOC is still not found, then download and build HWLOC from source
#if(NOT ESMI_INCLUDE_DIRS AND ESMI_LIBRARY)
#    MESSAGE(STATUS "Downloading and building hwloc from source")
#
#    ### Necessary for ExternalProject_Add
#    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
#    include(../SetupExternalProjects.cmake)
#    message(STATUS "Adding external project: hwloc")
#    message(STATUS " [*] ESMI_INCLUDE_DIRS = ${ESMI_INCLUDE_DIRS}")
#    message(STATUS " [*] ESMI_LIBRARY = ${ESMI_LIBRARY}")
#    set(ESMI_FOUND TRUE)
#endif()

# Abort if all methods fail
if(NOT ESMI_FOUND)
    MESSAGE(FATAL_ERROR "Esmi support needed")
endif()
